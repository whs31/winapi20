#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <iosfwd>
#include <winapi20/detail/export.h>
#include <winapi20/detail/definitions.h>
#include <winapi20/detail/template_util.h>
#include <winapi20/detail/cvt.h>
#include <winapi20/wrappers/handle.h>
#include <winapi20/wrappers/pid.h>
#include <winapi20/wrappers/memaddr.h>
#include <winapi20/wrappers/function.h>

/// \brief Namespace for functions and types originating from <tt>libloaderapi.h</tt> header file in the Windows API.
namespace winapi::dll
{
  class WINAPI20_EXPORT Library
  {
    public:
      /**
       * \brief
       * \see https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandleexa
       */
      enum class HandleFlags
      {
        None          = 0x00000000,
        Pin           = 0x00000001,
        WeakReference = 0x00000002
      };

      virtual ~Library();

      [[nodiscard]] inline auto name() const noexcept -> std::string const& {
        return this->m_name;
      }

      [[nodiscard]] inline auto handle() const noexcept -> Handle const& {
        return this->m_handle;
      }

      [[nodiscard]] inline auto handle_mut() noexcept -> Handle& {
        return this->m_handle;
      }

      [[nodiscard]] auto exported_function_address(std::string_view name) const -> std::optional<Function>;

      [[nodiscard]] inline auto operator[](std::string_view name) const -> std::optional<Function> {
        return this->exported_function_address(name);
      }

    public:
      [[nodiscard("do not discard result of factory function call")]]
      static auto view(std::string name, PID pid, HandleFlags flags = HandleFlags::None) -> std::optional<Library>;

      [[nodiscard("do not discard result of factory function call")]]
      static auto view(MemoryAddress const& address, PID pid, HandleFlags flags = HandleFlags::None) -> std::optional<Library>;

      // load library

    private:
      Library() = default;

    private:
      std::string m_name;
      Handle m_handle;
      bool m_cleanup = false;
  };
}