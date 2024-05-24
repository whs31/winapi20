#pragma once

#include <winapi20/detail/windows_headers.h>

#ifdef WINAPI20_ENABLED

#include <string>
#include <algorithm>

using std::string;
using std::string_view;

namespace winapi::utility
{
  inline auto to_lowercase(string_view str) -> string
  {
    string result;
    result.resize(str.size());
    std::transform(str.begin(), str.end(), result.begin(), ::tolower);
    return result;
  }
}

#endif