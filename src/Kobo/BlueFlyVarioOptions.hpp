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

#ifndef XCSOAR_KOBO_BLUE_FLY_VARIO_OPTIONS_HPP
#define XCSOAR_KOBO_BLUE_FLY_VARIO_OPTIONS_HPP

#include <cstdint>

struct
BlueFlyVarioOption
{
  const uint8_t id;
  const char *name;
  const char *command;
  const int min;
  const int max;
  const int std;
  const int factor;
  const int step;
  const char *description;
};

static constexpr BlueFlyVarioOption
blue_fly_vario_options[19] = {
   { 0, "use audio when connected", "BAC", 0, 1, 0, 1, 1, "Check to enable hardware audio when connected."},
   { 1, "position noise", "BFK", 10, 10000, 100, 1000, 10, "Kalman filter position noise." },
   { 2, "lift threshold", "BFL", 0, 1000, 20, 100, 10, "The value in m/s of lift when the audio beeping will start." },
   { 3, "lift off threshold", "BOL", 0, 1000, 5, 100, 5, "The value in m/s of lift when the audio beeping will stop." },
   { 4, "lift freq. base", "BFQ", 500, 2000, 1000, 1, 100, "The audio frequency for lift beeps in Hz of 0 m/s." },
   { 5, "lift freq. increment", "BFI", 0, 1000, 100, 10, 100, "The increase in audio frequency for lift beeps in Hz for each 1 m/s." },
   { 6, "sink threshold", "BFS", 0, 1000, 20, 100, 10, "The value in -m/s of sink when the sink tone will start." },
   { 7, "sink off threshold", "BOS", 0, 1000, 5, 100, 5, "The value in -m/s of sink when the sink tone will stop." },
   { 8, "sink freq. base", "BSQ", 250, 1000, 400, 1, 10, "The audio frequency for the sink tone in Hz of 0 m/s." },
   { 9, "sink freq. increment", "BSI", 0, 1000, 100, 1, 10, "The decrease in audio frequency for sink tone in Hz for each -1 m/s." },
   { 10, "rate multiplier", "BRM", 10, 100, 100, 100, 5, "The lift beep cadence -> 0.5 = beeping twice as fast as normal." },
   { 11, "volume", "BVL", 1, 1000, 1000, 1000, 1, "The volume of beeps -> 0.1 is only about 1/2 as loud as 1.0." },
   { 12, "output frequency", "BOF", 1, 50, 1, 1, 1, "The output frequency divisor -> 1-every 20ms ... 50-every 20msx50=1000ms." },
   { 13, "green LED", "BLD", 0, 1, 1, 1, 1, "Turn on the green LED with each lift beep." },
   { 14, "height sensitivity dm", "BHV", 0, 10000, 20, 1, 10, "How far you have to move in dm to reset the idle timeout" },
   { 15, "height seconds", "BHT", 0, 10000, 600, 1, 100, "Idle timeout." },
   { 16, "use audio buzzer", "BBZ", 0, 1, 0, 1, 1, "Use the experimental audio buzzer." },
   { 17, "buzzer threshold", "BZT", 0, 1000, 40, 100, 10, "The value in m/s below the liftThreshold when the buzzer will start." },
   { 18, "speed multiplier", "BSM", 10, 1000, 100, 100, 10, "The lift beep cadence as related to speed -> 2.0 = half as sensitive as normal." }
};

#endif
