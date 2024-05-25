#pragma once

#include <iostream>
#include <optional>
#include <winapi20/detail/export.h>
#include <winapi20/impl/memoryapi_impl.h>
#include <winapi20/wrappers/detail/pointerlike.h>

namespace winapi
{
  class WINAPI20_EXPORT MemoryAddress : public detail::PointerLike
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
       * \details Example values for different operations: <br>
       * - Read: memory::MemoryProtection::Readonly | memory::MemoryProtection::ExecuteRead | memory::MemoryProtection::ReadWrite | memory::MemoryProtection::ExecuteReadWrite | memory::MemoryProtection::WriteCopy | memory::MemoryProtection::ExecuteWriteCopy <br>
       * - Write: memory::MemoryProtection::ExecuteReadWrite | memory::MemoryProtection::ExecuteWriteCopy | memory::MemoryProtection::ReadWrite | memory::MemoryProtection::WriteCopy <br>
       * - Execute: memory::MemoryProtection::ExecuteRead | memory::MemoryProtection::ExecuteReadWrite | memory::MemoryProtection::ExecuteWriteCopy | memory::MemoryProtection::Execute <br>
       * \param protection Flags specifying the desired memory protection.
       * \return True if the specified memory protection is valid for the specified address.
       * \see https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory
       */
      [[nodiscard]] auto permissions(memory::MemoryProtection protection) const -> bool;

      friend std::ostream& operator<<(std::ostream& os, MemoryAddress const& addr);
  };

  inline std::ostream& operator<<(std::ostream& os, MemoryAddress const& addr) {
    os << fmt::format("MemoryAddress(0x{:x})", addr.as_integer());
    return os;
  }
} // namespace winapi

template <> struct fmt::formatter<winapi::MemoryAddress> : winapi::utility::ostream_formatter {};