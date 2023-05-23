// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "TTYEnumerator.hpp"
#include "util/CharUtil.hxx"
#include "util/StringCompare.hxx"
#include "system/FileUtil.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <tchar.h>
#include <libgen.h>

static bool
IsDirectory(const char *path) noexcept
{
  struct stat st;
  return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

[[gnu::pure]]
static bool
CheckTTYName(const char *name) noexcept
{
  /*
   * rfcomm devices do not have a 'device/driver' file that is used
   * to exclude virtual/pseudo terminals
   */
  if (StringStartsWith(name, "rfcomm"))
    return true;

  /*
   * we assume only entries with existing 'device/driver' folder
   * aren't virtual/pseudo terminals, as they are associated with a
   * kernel driver
   */
  const char *driver_format = "/sys/class/tty/%s/device/driver";
  char driver[128];
  if (snprintf(driver, sizeof(driver), driver_format, name) >= (int)sizeof(driver))
    return false; // ignore truncated file path

  if (IsDirectory(driver))
    return true;

  return false;
}

const char *
TTYEnumerator::Next() noexcept
{
  struct dirent *ent;
  while ((ent = readdir(dir)) != nullptr) {
    if (!CheckTTYName(ent->d_name))
      continue;

    if (snprintf(path, sizeof(path), "/dev/%s", ent->d_name) >= (int)sizeof(path))
      /* truncated - ignore */
      continue;

    if (File::IsCharDev(Path{path}) && access(path, R_OK|W_OK) == 0)
      return path;
  }

  return nullptr;
}
