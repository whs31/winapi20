#pragma once

#include <cstdint>
#include <iostream>
#include <winapi20/detail/formatters.h>

namespace winapi
{
  struct Handle
  {
    using pointer_type = void*;
    using integer_type = uintptr_t;

    Handle() = default;
    constexpr explicit Handle(pointer_type handle);
    explicit Handle(integer_type handle);

    [[nodiscard]] constexpr inline auto valid() const noexcept -> bool;

    template <typename T>
    requires std::is_integral_v<T> or std::is_pointer_v<T>
    [[nodiscard]] constexpr inline auto as() const noexcept -> T;

    [[nodiscard]] constexpr inline auto as_pointer() const noexcept -> pointer_type;
    [[nodiscard]] inline auto as_integer() const noexcept -> integer_type;

    constexpr inline explicit operator bool() const noexcept;
    constexpr inline explicit operator pointer_type() const noexcept;
    inline explicit operator integer_type() const noexcept;
    constexpr inline auto operator->() const noexcept -> pointer_type;
    constexpr inline auto operator*() const noexcept -> pointer_type;

    friend std::ostream& operator<<(std::ostream& os, const Handle& handle);

    private:
      static inline auto invalid_handle_ptr = reinterpret_cast<pointer_type>(-1);
      pointer_type m_handle = nullptr;
  };
}

namespace winapi
{
  constexpr inline Handle::Handle(Handle::pointer_type handle)
    : m_handle(handle)
  {}

  inline Handle::Handle(Handle::integer_type handle)
    : m_handle(reinterpret_cast<pointer_type>(handle))
  {}

  constexpr inline auto Handle::valid() const noexcept -> bool {
    return this->m_handle != nullptr and this->m_handle != invalid_handle_ptr;
  }

  template<typename T>
  requires std::is_integral_v<T> or std::is_pointer_v<T>
  constexpr inline auto Handle::as() const noexcept -> T {
    if constexpr (std::is_same_v<T, pointer_type>)
      return this->m_handle;
    if constexpr (std::is_pointer_v<T>)
      return static_cast<T>(this->m_handle);
    if constexpr (std::is_integral_v<T>)
      return reinterpret_cast<T>(this->m_handle);
  }

  constexpr inline auto Handle::as_pointer() const noexcept -> Handle::pointer_type {
    return this->as<pointer_type>();
  }

  inline auto Handle::as_integer() const noexcept -> Handle::integer_type {
    return this->as<integer_type>();
  }

  constexpr inline Handle::operator bool() const noexcept {
    return this->valid();
  }

  constexpr inline Handle::operator pointer_type() const noexcept {
    return this->as_pointer();
  }

  inline Handle::operator integer_type() const noexcept {
    return this->as_integer();
  }

  constexpr inline auto Handle::operator->() const noexcept -> Handle::pointer_type {
    return this->as_pointer();
  }

  constexpr inline auto Handle::operator*() const noexcept -> Handle::pointer_type {
    return this->as_pointer();
  }

  inline std::ostream& operator<<(std::ostream& os, Handle const& handle) {
    os << "Handle(0x" << std::hex << handle.as_integer() << std::dec << ")";
    return os;
  }
}

template <> struct fmt::formatter<winapi::Handle> : winapi::utility::ostream_formatter {};