#pragma once

#include <iostream>
#include <winapi20/detail/export.h>
#include <winapi20/wrappers/detail/pointerlike.h>

namespace winapi
{
  class MemoryAddress : public detail::PointerLike
  {
    public:
      using detail::PointerLike::PointerLike;

      friend std::ostream& operator<<(std::ostream& os, MemoryAddress const& handle);
  };

  inline std::ostream& operator<<(std::ostream& os, MemoryAddress const& handle) {
    os << fmt::format("MemoryAddress(0x{:x})", handle.as_integer());
    return os;
  }
} // namespace winapi

template <> struct fmt::formatter<winapi::MemoryAddress> : winapi::utility::ostream_formatter {};