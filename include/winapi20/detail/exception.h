#pragma once

#include <string>
#include <stdexcept>
#include <winapi20/detail/cvt.h>

namespace winapi
{
  class windows_exception : public std::runtime_error
  {
    public:
      explicit windows_exception(std::string const& message)
        : std::runtime_error(std::string("winapi error: ") + message)
      {}

      explicit windows_exception(std::wstring const& message)
        : std::runtime_error(detail::into<std::string>(message))
      {}

      explicit windows_exception(wchar_t const* message)
        : windows_exception(std::wstring(message))
      {}

      explicit windows_exception(char const* message)
        : windows_exception(std::string(message))
      {}

      ~windows_exception() noexcept override = default;

      windows_exception(windows_exception const&) = default;
      windows_exception(windows_exception&&) = default;

      windows_exception& operator=(windows_exception const&) = default;
      windows_exception& operator=(windows_exception&&) = default;

      [[nodiscard]] inline auto what() const noexcept -> char const* override {
        return std::runtime_error::what();
      }
  };
}