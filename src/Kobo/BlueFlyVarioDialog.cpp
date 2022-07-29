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

  void SendConfigurationToDevice() {}

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
