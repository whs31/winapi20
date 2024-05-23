#pragma once

#include <cstdint>
#include <string>
#include <winapi20/detail/export.h>

namespace winapi
{
  [[nodiscard]] WINAPI20_EXPORT auto last_error() noexcept -> uint32_t;
  [[nodiscard]] WINAPI20_EXPORT auto last_error_string() noexcept -> std::string;
} // namespace winapi