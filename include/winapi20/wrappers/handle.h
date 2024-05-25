#pragma once

#include <iostream>
#include <winapi20/detail/export.h>
#include <winapi20/wrappers/detail/pointerlike.h>

namespace winapi
{
  struct WINAPI20_EXPORT Handle : public detail::PointerLike
  {
    constexpr inline Handle();
    constexpr explicit Handle(pointer_type handle, Cleanup cleanup);
    explicit Handle(integer_type handle, Cleanup cleanup);

    virtual ~Handle() override;
    Handle(Handle const&) = default;
    Handle(Handle&&) noexcept = default;

    Handle& operator=(Handle const&) = default;
    Handle& operator=(Handle&&) noexcept = default;

    [[nodiscard]] constexpr inline auto valid() const noexcept -> bool override;

    friend std::ostream& operator<<(std::ostream& os, const Handle& handle);

    private:
      static inline auto invalid_handle_ptr = reinterpret_cast<pointer_type>(-1);
      Cleanup m_cleanup = Cleanup::Manual;
  };
}

namespace winapi
{
  constexpr inline Handle::Handle()
    : detail::PointerLike(nullptr)
    , m_cleanup(Cleanup::Manual)
  {}

  constexpr inline Handle::Handle(Handle::pointer_type handle, Cleanup cleanup)
    : detail::PointerLike(handle)
    , m_cleanup(cleanup)
  {}

  constexpr inline auto Handle::valid() const noexcept -> bool {
    return this->m_ != nullptr and this->m_ != invalid_handle_ptr;
  }

  inline std::ostream& operator<<(std::ostream& os, Handle const& handle) {
    os << fmt::format("Handle(0x{:x})", handle.as_integer());
    return os;
  }
}

template <> struct fmt::formatter<winapi::Handle> : winapi::utility::ostream_formatter {};