#pragma once

#include <iostream>
#include <winapi20/detail/export.h>
#include <winapi20/wrappers/detail/pointerlike.h>

namespace winapi
{
  class Function : public detail::PointerLike
  {
    public:
      using func_type = long long int(__stdcall*)();
      using detail::PointerLike::PointerLike;

      explicit Function(func_type func)
        : detail::PointerLike(reinterpret_cast<pointer_type>(func))
      {}

      template <typename R = void, typename... T>
      inline auto __fastcall operator()(T&&... args) const noexcept -> R {
        return ((R (__fastcall*) (T...)) this->m_)(args...);
      }

      template <typename R = void, typename... T>
      inline auto __fastcall call(T&&... args) const noexcept -> R {
        return this->operator()<R, T...>(std::forward<T>(args)...);
      }

      friend std::ostream& operator<<(std::ostream& os, Function const& fn);
  };

  inline std::ostream& operator<<(std::ostream& os, Function const& fn) {
    os << fmt::format("Function(0x{:x})", fn.as_integer());
    return os;
  }
} // namespace winapi

template <> struct fmt::formatter<winapi::Function> : winapi::utility::ostream_formatter {};