#pragma once

#include <winapi20/detail/export.h>

#ifdef WINAPI20_ENABLED

#include <cstdint>
#include <string>

namespace winapi
{
  [[nodiscard]] WINAPI20_EXPORT auto last_error() noexcept -> uint32_t;
  [[nodiscard]] WINAPI20_EXPORT auto last_error_string() noexcept -> std::string;
} // namespace winapi

#endif // WINAPI20_ENABLED