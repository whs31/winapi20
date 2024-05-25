#pragma once

#include <iostream>
#include <winapi20/detail/export.h>
#include <winapi20/wrappers/detail/pointerlike.h>

namespace winapi
{
  class MemoryAddress : public detail::PointerLike
  {
    public:
      using integer_type = uintptr_t;
      using pointer_type = void*;

      using detail::PointerLike::PointerLike;

    private:
      pointer_type m_ = nullptr;
  };
} // namespace winapi