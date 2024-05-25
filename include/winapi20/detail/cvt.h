#pragma once

#include <winapi20/detail/export.h>

#include <string>
#include <string_view>
#include <codecvt>
#include <locale>
#include <type_traits>

namespace winapi
{
  namespace detail
  {
    template <typename U, typename T>
    [[maybe_unused]] [[nodiscard]] auto into(T&& value) -> U = delete;

    template <>
    [[nodiscard]] inline auto into(std::wstring& value) -> std::string {
      return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(value);
    }

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

    template <>
    [[nodiscard]] inline auto into(std::wstring_view const& value) -> std::string {
      return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(std::wstring(value));
    }

    template <>
    [[nodiscard]] inline auto into(std::wstring_view& value) -> std::string {
      return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(std::wstring(value));
    }

    template <typename U = std::string, size_t N>
    [[nodiscard]] inline auto into_utf8(wchar_t const (&value)[N]) -> U {
      return into<U>(std::wstring(value));
    }

    template <>
    [[nodiscard]] inline auto into(std::string&& value) -> std::wstring {
      return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(value);
    }

    template <>
    [[nodiscard]] inline auto into(std::string const& value) -> std::wstring {
      return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(value);
    }

    template <>
    [[nodiscard]] inline auto into(std::string_view&& value) -> std::wstring {
      return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(std::string(value));
    }

    template <>
    [[nodiscard]] inline auto into(std::string_view const& value) -> std::wstring {
      return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(std::string(value));
    }

    template <>
    [[nodiscard]] inline auto into(std::string_view& value) -> std::wstring {
      return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(std::string(value));
    }
  }

  inline namespace enums
  {
    template <typename T>
    requires(std::is_enum_v<T>)
    constexpr auto operator|(T const lhs, T const rhs) noexcept {
      using ut = std::underlying_type_t<T>;
      return static_cast<T>(static_cast<ut>(lhs) | static_cast<ut>(rhs));
    }

    template <typename T>
    requires(std::is_enum_v<T>)
    constexpr auto operator&(T const lhs, T const rhs) noexcept {
      using ut = std::underlying_type_t<T>;
      return static_cast<T>(static_cast<ut>(lhs) & static_cast<ut>(rhs));
    }
  }
}