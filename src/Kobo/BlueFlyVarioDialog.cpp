/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Message.hpp"
#include "UIGlobals.hpp"
#include "Language/Language.hpp"
#include "Widget/RowFormWidget.hpp"
#include "Form/DataField/Enum.hpp"
#include "Form/DataField/Integer.hpp"
#include "Form/DataField/Boolean.hpp"
#include "Form/DataField/Listener.hpp"
#include "LogFile.hpp"
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include "BlueFlyVarioOptions.hpp"
#include "BlueFlyVarioDialogChoices.hpp"
#include "BlueFlyVarioDialog.hpp"


class BlueFlyVarioWidget final
  : public RowFormWidget, DataFieldListener {
  enum BlueFlyWidgets {
    SERIAL_PORT_FIELD,
    BAUD_RATE_FIELD,
    COMMAND_FIELD,
    DESCRIPTION_FIELD,
    ON_OFF_FIELD,
    MIN_FIELD,
    MAX_FIELD,
    FACTOR_FIELD,
    NUMBER_FIELD,
    SEND_TO_BUTTON,
  };

  WidgetDialog &dialog;

public:
  BlueFlyVarioWidget(const DialogLook &look, WidgetDialog &_dialog):RowFormWidget(look), dialog(_dialog) {}

  void OnModified(DataField &df) noexcept override {
    if(IsDataField(COMMAND_FIELD, df)) {
      DataFieldEnum &enum_df = (DataFieldEnum &) df;
      int i = enum_df.GetValue();
      BlueFlyVarioOption option = blue_fly_vario_options[i];

      SetRowVisible(ON_OFF_FIELD, option.max == 1);
      SetRowVisible(MIN_FIELD, option.max != 1);
      SetRowVisible(MAX_FIELD, option.max != 1);
      SetRowVisible(FACTOR_FIELD, option.max != 1);
      SetRowVisible(NUMBER_FIELD, option.max != 1);

      LoadValue(DESCRIPTION_FIELD, option.description);
      LoadValue(ON_OFF_FIELD, option.std != 0);
      LoadValue(MIN_FIELD, option.min);
      LoadValue(MAX_FIELD, option.max);
      LoadValue(FACTOR_FIELD, option.factor);
      LoadValues(NUMBER_FIELD, option.std, option.min, option.max, option.step);
    }
  }

private:

  void ConfigureTTY(int *serial_port, speed_t baud_rate) {
    struct termios tty;

    if(tcgetattr(*serial_port, &tty) != 0)
    {
      LogFormat(_T("Failed to read serial port attributes with tcgetattr: %s"), strerror(errno));
      close(*serial_port);
      return;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    cfsetispeed(&tty, baud_rate);
    cfsetospeed(&tty, baud_rate);

    if (tcsetattr(*serial_port, TCSANOW, &tty) != 0)
    {
      LogFormat(_T("Failed to set serial port attributes with tcsetattr: %s"), strerror(errno));
      close(*serial_port);
      return;
    }
  }

  void SendConfigurationToDevice(const char *device, speed_t baud_rate, const char *command, int value) {
    int serial_port = open(device, O_RDWR);

    if (serial_port < 0)
    {
      LogFormat(_T("Failed to open device %s: %s"), device, strerror(errno));
      return;
    }

    ConfigureTTY(&serial_port, baud_rate);

    char buffer[1024];
    sprintf(buffer, "$%s %d*", command, value);

    write(serial_port, buffer, sizeof(buffer));

    close(serial_port);
  }

  void SendConfigurationToDevice() {
    WndProperty &serial_port_control = GetControl(SERIAL_PORT_FIELD);
    const char *device = serial_port_control.GetDataField()->GetAsString();

    WndProperty &commandControl = GetControl(COMMAND_FIELD);
    DataFieldEnum *enum_df = (DataFieldEnum *) commandControl.GetDataField();
    int i = enum_df->GetValue();
    BlueFlyVarioOption option = blue_fly_vario_options[i];

    int value = GetValue(option);

    speed_t baud_rate = GetBaudRate();

    SendConfigurationToDevice(device, baud_rate, option.command, value);
  }

  /* virtual methods from class Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override {
    RowFormWidget::Prepare(parent, rc);

    AddEnum(_T("Serial port"), _T("serial port to connect to"), serial_port_choices, 0);
    AddEnum(_T("Baud rate"), _T("baud rate for serial port communication"), baud_rate_choices, 0);

    unsigned default_option_index = 0;
    AddEnum(_T("Command"), _T("configuration command"), command_choices, default_option_index, this);
    BlueFlyVarioOption option = blue_fly_vario_options[default_option_index];

    WndProperty *control= AddText(_T("Description"), _T("description of the command"), option.description);
    control->SetReadOnly(true);

    AddBoolean(_T("Value"), _T("argument for the command"), option.std != 0);

    control = AddInteger(_T("Min"), _("allowed minimum"), _T("%d"), _T("%d"),
                         0, 10000, 1, option.min);
    control->SetReadOnly(true);

    control = AddInteger(_T("Max"), _("allowed maximum"), _T("%d"), _T("%d"),
                         0, 10000, 1, option.max);
    control->SetReadOnly(true);

    control = AddInteger(_T("Factor"), _("value * factor equals the actual configuration value"), _T("%d"), _T("%d"),
                           0, 10000, 1, option.factor);
    control->SetReadOnly(true);

    AddInteger(_T("Value"), _("argument for the command"), _T("%d"), _T("%d"),
                       option.min, option.max, option.step, option.std);

    SetRowVisible(ON_OFF_FIELD, option.max == 1);
    SetRowVisible(MIN_FIELD, option.max != 1);
    SetRowVisible(MAX_FIELD, option.max != 1);
    SetRowVisible(FACTOR_FIELD, option.max != 1);
    SetRowVisible(NUMBER_FIELD, option.max != 1);

    AddButton("Send to Device", [this](){ SendConfigurationToDevice(); });
  }

  int
  GetValueFromOnOff()
  {
    WndProperty &onOffControl = GetControl(ON_OFF_FIELD);
    DataFieldBoolean *df = (DataFieldBoolean*)(onOffControl.GetDataField());
    return df->GetValue() ? 1 : 0;
  }

  int
  getValueFromNumber(const BlueFlyVarioOption &option)
  {
    WndProperty &numberControl = GetControl(NUMBER_FIELD);
    DataFieldInteger *df = (DataFieldInteger*)(numberControl.GetDataField());
    int value = df->GetValue();
    value = std::max(value, option.min);
    value = std::min(value, option.max);
    return value;
  }

  int
  GetValue(const BlueFlyVarioOption &option)
  {
    if (option.max == 1)
    {
      return GetValueFromOnOff();
    }
    else
    {
      return getValueFromNumber(option);
    }
  }

  speed_t
  GetBaudRate()
  {
    WndProperty &baud_rate_control = GetControl(BAUD_RATE_FIELD);
    DataFieldEnum *enum_df = (DataFieldEnum *) baud_rate_control.GetDataField();
    int baud_rate_index = enum_df->GetValue();
    if (baud_rate_index == 0)
    {
      return B115200;
    }
    else
    {
      return B57600;
    }
  }
};

void
ShowBlueFlyVarioDialog()
{
  const DialogLook &look = UIGlobals::GetDialogLook();

  WidgetDialog
    dialog(WidgetDialog::Full{}, UIGlobals::GetMainWindow(),
           look, _T("BlueFlyVario TTL GPS - Send Configuration Commands"),
           new BlueFlyVarioWidget(look, dialog));
  dialog.AddButton(_("Close"), mrOK);
  dialog.ShowModal();
}
