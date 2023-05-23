// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include <dirent.h>

/**
 * A class that can enumerate TTY devices on Linux and other POSIX
 * systems, by reading directory entries in /sys/class/tty.
 */
class TTYEnumerator {
  DIR *dir;
  char path[64];

public:
  TTYEnumerator() noexcept
    :dir(opendir("/sys/class/tty")) {}

  ~TTYEnumerator() noexcept {
    if (dir != nullptr)
      closedir(dir);
  }

  /**
   * Has the constructor failed?
   */
  bool HasFailed() const noexcept {
    return dir == nullptr;
  }

  /**
   * Find the next device (or the first one, if this method hasn't
   * been called so far).
   *
   * @return the absolute path of the device, or nullptr if there are
   * no more devices
   */
  const char *Next() noexcept;
};
