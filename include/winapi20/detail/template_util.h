#pragma once

#include <winapi20/detail/export.h>
#include <winapi20/detail/definitions.h>

#include <string>
#include <algorithm>

namespace winapi::utility
{
  inline auto to_lowercase(std::string_view str) -> std::string {
    auto result = std::string();
    result.resize(str.size());
    std::transform(str.begin(), str.end(), result.begin(), ::tolower);
    return result;
  }

  template <typename T, typename U>
  requires std::is_same_v<std::decay_t<T>, std::string>
      or std::is_same_v<std::decay_t<T>, std::string_view>
  inline auto sensitive_compare(T const& lhs, U const& rhs, CaseSensitivity cs) -> bool {
    auto lhs_ = std::string(lhs);
    if(cs == CaseSensitivity::CaseInsensitive)
      lhs_ = to_lowercase(lhs_);
    return cs == CaseSensitivity::CaseSensitive ? lhs_ == rhs : lhs_ == to_lowercase(rhs);
  }
}