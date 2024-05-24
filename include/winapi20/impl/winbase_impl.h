#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <winapi20/detail/export.h>
#include <winapi20/detail/exception.h>

namespace winapi
{
  /**
   * \brief Contains information about a hardware profile.
   * \details Method <tt>HWProfileInfo::get</tt> retrieves the current hardware profile for the local computer.
   * \see https://learn.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-hw_profile_infoa
   */
  struct WINAPI20_EXPORT HWProfileInfo
  {
    /// \brief The reported docking state of the computer.
    enum class DockInfo
    {
      /// \brief The computer is docked.
      Docked        = 0x2,

      /// \brief The computer is undocked.
      Undocked      = 0x1,

      /**
       * \brief User-supplied docking state information.
       * \details If this flag is set, <tt>HWProfileInfo::get</tt> retrieved the current docking state from
       * information provided by the user in the Hardware Profiles page of the System control panel application.
       * \note If there is no such value or the value is set to 0, this flag is set.
       */
      UserSupplied  = 0x4,

      /**
       * \brief The computer is docked, according to information provided by the user.
       * \details This is combination of UserSupplied and Docked flags.
       */
      UserDocked    = UserSupplied | Docked,

      /**
       * \brief The computer is undocked, according to information provided by the user.
       * \details This value is a combination of the UserSupplied and Undocked flags.
       */
      UserUndocked  = UserSupplied | Undocked
    };

    HWProfileInfo(HWProfileInfo const&) = default;
    HWProfileInfo(HWProfileInfo&&) = default;
    HWProfileInfo& operator=(HWProfileInfo const&) = default;
    HWProfileInfo& operator=(HWProfileInfo&&) = default;

    ~HWProfileInfo() = default;

    /**
     * \brief Retrieves the current hardware profile for the local computer.
     * \details Function can fail with <tt>winapi::windows_exception</tt> if it failed to retrieve the hardware profile.
     * The GetCurrentHwProfile function retrieves the display name and globally unique identifier (GUID) string for
     * the hardware profile. The function also retrieves the reported docking state for portable computers with
     * docking stations. <br>
     * The system generates a GUID for each hardware profile and stores it as a string in the registry.
     * You can use GetCurrentHwProfile to retrieve the GUID string to use as a registry subkey under your application's
     * configuration settings key in HKEY_CURRENT_USER. This enables you to store each user's settings for each hardware
     * profile. For example, the Colors control panel application could use the subkey to store each user's color
     * preferences for different hardware profiles, such as profiles for the docked and undocked states. <br>
     * Applications that use this functionality can check the current hardware profile when they start up, and update
     * their settings accordingly.
     * \see https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcurrenthwprofilea
     * \throws winapi::windows_exception if it failed to retrieve the hardware profile.
     * \return The current hardware profile struct.
     */
    [[nodiscard]] static auto get() noexcept(false) -> HWProfileInfo;

    /// \brief The reported docking state of the computer
    DockInfo dock_info{};

    /**
     * \brief The globally unique identifier (GUID) string for the current hardware profile.
     * \details This string is enclosed in curly braces ({...}) and contains GUID of the current hardware profile.
     * E.g. <tt>{12340001-4980-1920-6788-123456789012}</tt>.
     * You can use this string as a registry subkey under your application's configuration settings key in HKEY_CURRENT_USER.
     * This enables you to store settings for each hardware profile.
     */
    std::string guid;

    /// \brief The display name for the current hardware profile.
    std::string name;

   private:
    HWProfileInfo() = default;
  };
} // namespace winapi