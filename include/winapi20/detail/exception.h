#pragma once

#include <string>
#include <stdexcept>
#include <locale>
#include <codecvt>

namespace winapi
{
  class windows_exception : public std::runtime_error
  {
    public:
      explicit windows_exception(std::wstring const& message)
        : std::runtime_error(std::string("winapi error: ") + std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(message))
      {}

      explicit windows_exception(wchar_t const* message)
        : windows_exception(std::wstring(message))
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