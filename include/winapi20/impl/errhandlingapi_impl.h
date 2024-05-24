#pragma once

#include <cstdint>
#include <string>
#include <winapi20/detail/export.h>

namespace winapi
{
  WINAPI20_EXPORT auto fatal_application_exit(std::string_view message) noexcept -> void;

  [[nodiscard]] WINAPI20_EXPORT auto last_error() noexcept -> uint32_t;
  [[nodiscard]] WINAPI20_EXPORT auto last_error_string() noexcept -> std::string;
} // namespace winapi
