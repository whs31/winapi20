#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <iosfwd>
#include <filesystem>
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
      using enum OwnershipMode;

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

      explicit Library(
          OwnershipMode mode,
          std::string name,
          PID pid,
          HandleFlags flags = HandleFlags::None
      ) noexcept(false);

      explicit Library(
          OwnershipMode mode,
          MemoryAddress const& address,
          PID pid,
          HandleFlags flags = HandleFlags::None
      ) noexcept(false);

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

      /**
       * \brief Retrieves the fully qualified path for the file that contains the specified module.
       * \note Querying the path of module that belongs to another process is expensive. Do not call this method frequently.
       * \throws winapi::windows_exception If the operation fails.
       * \return The fully qualified path for the file that contains the specified module.
       * \see https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
       */
      [[nodiscard]] auto file_path() const noexcept(false) -> std::filesystem::path;
      [[nodiscard]] auto exported_function_address(std::string_view name) const -> std::optional<Function>;

      [[nodiscard]] inline auto operator[](std::string_view name) const -> std::optional<Function> {
        return this->exported_function_address(name);
      }

    protected:
      Library() = default;
      Library(std::string name, Handle&& handle, PID pid, bool cleanup);

    private:
      std::string m_name;
      Handle m_handle;
      PID m_pid;
      bool m_cleanup = false;
  };
}