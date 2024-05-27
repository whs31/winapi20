#pragma once

#include <iostream>
#include <winapi20/detail/export.h>
#include <winapi20/wrappers/detail/pointerlike.h>

namespace winapi
{
  struct HandleView : public detail::PointerLike
  {
    using detail::PointerLike::PointerLike;
    [[nodiscard]] constexpr inline auto valid() const noexcept -> bool override;

    friend std::ostream& operator<<(std::ostream& os, HandleView const& handle);

    private:
      static inline auto invalid_handle_ptr = reinterpret_cast<pointer_type>(-1);
  };

  struct Handle : public detail::PointerLike
  {
    constexpr inline Handle();
    constexpr explicit Handle(pointer_type handle, Cleanup cleanup);
    WINAPI20_EXPORT explicit Handle(integer_type handle, Cleanup cleanup);

    WINAPI20_EXPORT virtual ~Handle() override;
    Handle(Handle const&) = delete;
    Handle(Handle&&) noexcept = default;

    Handle& operator=(Handle const&) = delete;
    Handle& operator=(Handle&&) noexcept = default;

    [[nodiscard]] constexpr inline auto valid() const noexcept -> bool override;
    [[nodiscard]] constexpr inline auto view() const noexcept -> HandleView;

    friend std::ostream& operator<<(std::ostream& os, Handle const& handle);

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

  constexpr auto Handle::view() const noexcept -> HandleView { return HandleView(this->m_); } // NOLINT(*-return-braced-init-list)

  constexpr inline auto HandleView::valid() const noexcept -> bool {
    return this->m_ != nullptr and this->m_ != invalid_handle_ptr;
  }

  inline std::ostream& operator<<(std::ostream& os, HandleView const& handle) {
    os << fmt::format("HandleView(0x{:x})", handle.as_integer());
    return os;
  }
}

template <> struct fmt::formatter<winapi::Handle> : winapi::utility::ostream_formatter {};
template <> struct fmt::formatter<winapi::HandleView> : winapi::utility::ostream_formatter {};