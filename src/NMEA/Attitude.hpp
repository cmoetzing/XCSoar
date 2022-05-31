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

#ifndef XCSOAR_ATTITUDE_HPP
#define XCSOAR_ATTITUDE_HPP

#include "Math/Angle.hpp"
#include "Validity.hpp"

struct AttitudeState
{
  /** Estimated bank angle */
  Angle bank_angle;

  /** Estimated pitch angle */
  Angle pitch_angle;

  /** Estimated heading */
  Angle heading;

  Validity bank_angle_available;
  Validity pitch_angle_available;
  Validity heading_available;

  bool bank_angle_computed;
  bool pitch_angle_computed;
  bool heading_computed;

  constexpr void Reset() noexcept {
    bank_angle_available.Clear();
    bank_angle_computed = false;
    pitch_angle_available.Clear();
    pitch_angle_computed = false;
    heading_available.Clear();
    heading_computed = false;
  }

  constexpr bool IsBankAngleUseable() const noexcept {
    return bank_angle_available || bank_angle_computed;
  }

  constexpr bool IsPitchAngleUseable() const noexcept {
    return pitch_angle_available || pitch_angle_computed;
  }

  constexpr bool IsHeadingUseable() const noexcept {
    return heading_available || heading_computed;
  }

  /**
   * Adds data from the specified object, unless already present in
   * this one.
   */
  void Complement(const AttitudeState &add) noexcept;

  void Expire(TimeStamp now) noexcept;
};

#endif
