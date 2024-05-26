#pragma once

#include <iostream>
#include <winapi20/detail/export.h>
#include <winapi20/detail/formatters.h>

namespace winapi
{
  /**
   * \brief Wrapper for process ID (PID) integer.
   */
  struct PID
  {
    /// \brief Underlying process ID integer type. Defaults to <tt>uint32_t</tt>.
    using integer_type = uint32_t;

    /// \brief Creates a PID object representing the current process.
    constexpr PID() = default;

    /**
     * \brief Creates a PID object representing the specified process ID.
     * \param pid Process ID.
     */
    constexpr explicit inline PID(integer_type pid) noexcept
      : m_pid(pid)
    {}

    /**
     * \brief Creates a PID object representing the current process.
     * \note This is not actually a current process ID, but just an empty PID object.
     * Most Win32API functions will treat it as such.
     * If you need the current process ID, use <tt>current_abs()</tt> function.
     * \return Current process ID, syntactically equivalent to <tt>0</tt>.
     */
    [[nodiscard("do not discard result of factory function")]]
    static constexpr inline auto current() noexcept -> PID {
      return {};
    }

    /**
     * \brief Returns true if the PID object represents the current process.
     * \return True if the PID object represents the current process.
     */
    [[nodiscard]] constexpr inline bool is_current() const noexcept {
      return this->m_pid == 0;
    }

    /**
     * \brief Returns the underlying process ID integer.
     * \return The underlying process ID integer of <tt>PID::integer_type</tt> type.
     */
    [[nodiscard]] constexpr inline auto value() const noexcept -> integer_type {
      return **this;
    }

    /**
     * \brief Returns the underlying process ID integer.
     * \return The underlying process ID integer of <tt>PID::integer_type</tt> type.
     */
    constexpr inline auto operator*() const noexcept -> integer_type {
      return this->m_pid;
    }

    /// \brief Comparison universal operator.
    constexpr inline auto operator<=>(PID const&) const = default;

    /**
     * \brief Returns a string representation of the PID.
     * \details PID is represented as a decimal number. Example:
     * \code
       PID(4455)
     * \endcode
     * \param os Output stream.
     * \param pid PID object.
     * \return Reference to the output stream.
     */
    inline friend std::ostream& operator<<(std::ostream& os, PID const& pid) {
      os << fmt::format("PID({})", pid.m_pid);
      return os;
    }

    private:
      integer_type m_pid = 0;
  };
}

/**
 * \brief Custom formatter for <tt>PID</tt> type (<i>fmt</i> library).
 * \details PID is represented as a decimal number. Example:
 * \code
   PID(4455)
 * \endcode
 */
template <> struct fmt::formatter<winapi::PID> : winapi::utility::ostream_formatter {};