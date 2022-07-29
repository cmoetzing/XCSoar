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

#ifndef XCSOAR_KOBO_BLUE_FLY_VARIO_DIALOG_CHOICES
#define XCSOAR_KOBO_BLUE_FLY_VARIO_DIALOG_CHOICES

#include "BlueFlyVarioOptions.hpp"

static const StaticEnumChoice serial_port_choices[] = {
  { 0, "/dev/ttyxmmc0", _T("usually internal serial port for Kobo")},
  { 0 }
};

static const StaticEnumChoice baud_rate_choices[] = {
  { 0, _T("115200"), _T("used by devices from V11 and onwards")},
  { 1, _T("57600"), _T("used by devices before V11")},
  { 0 }
};

static const StaticEnumChoice command_choices[] = {
  { 0, blue_fly_vario_options[0].name, blue_fly_vario_options[0].description },
  { 1, blue_fly_vario_options[1].name, blue_fly_vario_options[1].description },
  { 2, blue_fly_vario_options[2].name, blue_fly_vario_options[2].description },
  { 3, blue_fly_vario_options[3].name, blue_fly_vario_options[3].description },
  { 4, blue_fly_vario_options[4].name, blue_fly_vario_options[4].description },
  { 5, blue_fly_vario_options[5].name, blue_fly_vario_options[5].description },
  { 6, blue_fly_vario_options[6].name, blue_fly_vario_options[6].description },
  { 7, blue_fly_vario_options[7].name, blue_fly_vario_options[7].description },
  { 8, blue_fly_vario_options[8].name, blue_fly_vario_options[8].description },
  { 9, blue_fly_vario_options[9].name, blue_fly_vario_options[9].description },
  { 10, blue_fly_vario_options[10].name, blue_fly_vario_options[10].description },
  { 11, blue_fly_vario_options[11].name, blue_fly_vario_options[11].description },
  { 12, blue_fly_vario_options[12].name, blue_fly_vario_options[12].description },
  { 13, blue_fly_vario_options[13].name, blue_fly_vario_options[13].description },
  { 14, blue_fly_vario_options[14].name, blue_fly_vario_options[14].description },
  { 15, blue_fly_vario_options[15].name, blue_fly_vario_options[15].description },
  { 16, blue_fly_vario_options[16].name, blue_fly_vario_options[16].description },
  { 17, blue_fly_vario_options[17].name, blue_fly_vario_options[17].description },
  { 18, blue_fly_vario_options[18].name, blue_fly_vario_options[18].description },
  { 0 }
};

#endif
