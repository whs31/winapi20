#pragma once

#include <iostream>
#include <optional>
#include <winapi20/detail/export.h>
#include <winapi20/impl/memoryapi_impl.h>
#include <winapi20/wrappers/detail/pointerlike.h>

namespace winapi
{
  class MemoryAddress : public detail::PointerLike
  {
    public:
      using offset_type = std::ptrdiff_t;
      using detail::PointerLike::PointerLike;

      [[nodiscard]] inline auto offset(offset_type const v) const noexcept -> MemoryAddress {
        return MemoryAddress(this->as_integer() + v); // NOLINT(*-return-braced-init-list)
      }

      [[nodiscard]] inline auto absolute_address(
          offset_type address_offset,
          std::optional<size_t> opcode_size = std::nullopt
      ) const noexcept -> MemoryAddress
      {
        return MemoryAddress(this->as_integer()  // NOLINT(*-return-braced-init-list)
          + *reinterpret_cast<int*>(this->as_integer() + address_offset)
          + opcode_size.value_or(address_offset + sizeof(uint32_t))
        );
      }

      /**
       * \brief Returns true if the specified memory protection is valid for the specified address.
       * \details
       * Executes query to determine if the specified memory protection is valid for the specified address.
       * Do not invoke this method frequently.
       * Example values for different operations: <br>
       * - Read: Readonly | ExecuteRead | ReadWrite | ExecuteReadWrite | WriteCopy | ExecuteWriteCopy <br>
       * - Write: ExecuteReadWrite | ExecuteWriteCopy | ReadWrite | WriteCopy <br>
       * - Execute: ExecuteRead | ExecuteReadWrite | ExecuteWriteCopy | Execute <br>
       * \param protection Flags specifying the desired memory protection.
       * \return True if the specified memory protection is valid for the specified address.
       * \see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory
       */
      [[nodiscard]] WINAPI20_EXPORT auto permissions(memory::MemoryProtection protection) const -> bool;

      friend std::ostream& operator<<(std::ostream& os, MemoryAddress const& addr);
  };

  inline std::ostream& operator<<(std::ostream& os, MemoryAddress const& addr) {
    os << fmt::format("MemoryAddress(0x{:x})", addr.as_integer());
    return os;
  }
} // namespace winapi

template <> struct fmt::formatter<winapi::MemoryAddress> : winapi::utility::ostream_formatter {};