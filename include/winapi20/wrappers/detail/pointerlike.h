#pragma once

#include <cstdint>
#include <winapi20/detail/formatters.h>
#include <winapi20/detail/definitions.h>

namespace winapi::detail
{
  struct PointerLike
  {
    using pointer_type = void*;
    using integer_type = uintptr_t;

    constexpr explicit PointerLike(pointer_type ptr) noexcept : m_(ptr) {}
    explicit PointerLike(integer_type ptr) noexcept : m_(reinterpret_cast<pointer_type>(ptr)) {}

    virtual ~PointerLike() = default;

    [[nodiscard]] constexpr virtual auto valid() const noexcept -> bool {
      return this->m_ != nullptr;
    };

    template <typename T>
    requires std::is_integral_v<T> or std::is_pointer_v<T>
    [[nodiscard]] constexpr inline auto as() const noexcept -> T {
      if constexpr(std::is_same_v<T, pointer_type>)
        return this->m_;
      if constexpr(std::is_pointer_v<T>)
        return static_cast<T>(this->m_);
      if constexpr(std::is_integral_v<T>)
        return reinterpret_cast<T>(this->m_);
      else
        std::terminate();
    }

    [[nodiscard]] constexpr inline auto as_pointer() const noexcept -> pointer_type { return this->as<pointer_type>(); }
    [[nodiscard]] inline auto as_integer() const noexcept -> integer_type { return this->as<integer_type>(); }

    constexpr inline explicit operator bool() const noexcept { return this->valid(); }
    constexpr inline explicit operator pointer_type() const noexcept { return this->as_pointer(); }
    inline explicit operator integer_type() const noexcept { return this->as_integer(); }
    constexpr inline auto operator->() const noexcept -> pointer_type { return this->as_pointer(); }
    constexpr inline auto operator*() const noexcept -> pointer_type { return this->as_pointer(); }

    protected:
      pointer_type m_ = nullptr;
  };
} // namespace winapi::detail