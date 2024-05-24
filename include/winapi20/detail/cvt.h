#pragma once

#include <winapi20/detail/export.h>

#include <string>
#include <string_view>
#include <codecvt>
#include <locale>

namespace winapi::detail
{
  template <typename U, typename T>
  [[maybe_unused]] [[nodiscard]] auto into(T&& value) -> U = delete;

  template <>
  [[nodiscard]] inline auto into(std::wstring&& value) -> std::string {
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(value);
  }

  template <>
  [[nodiscard]] inline auto into(std::wstring const& value) -> std::string {
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(value);
  }

  template <>
  [[nodiscard]] inline auto into(std::wstring_view&& value) -> std::string {
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(std::wstring(value));
  }

  template <typename U = std::string, size_t N>
  [[nodiscard]] inline auto into_utf8(wchar_t const (&value)[N]) -> U {
    return into<U>(std::wstring(value));
  }
}