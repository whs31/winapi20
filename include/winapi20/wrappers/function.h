#pragma once

#include <iostream>
#include <winapi20/detail/export.h>
#include <winapi20/wrappers/detail/pointerlike.h>

namespace winapi
{
  class Function : public detail::PointerLike
  {
    public:
      using detail::PointerLike::PointerLike;

      template <typename R = void*, typename... T>
      inline auto __fastcall operator()(T&&... args) const noexcept -> pointer_type {
        return ((R (__fastcall*) (T...)) this->m_)(args...);
      }

      friend std::ostream& operator<<(std::ostream& os, Function const& fn);
  };

  inline std::ostream& operator<<(std::ostream& os, Function const& fn) {
    os << fmt::format("MemoryAddress(0x{:x})", fn.as_integer());
    return os;
  }
} // namespace winapi

template <> struct fmt::formatter<winapi::Function> : winapi::utility::ostream_formatter {};