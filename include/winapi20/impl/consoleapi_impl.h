#pragma once

#include <array>
#include <cstdint>
#include <winapi20/detail/export.h>
#include <winapi20/detail/cvt.h>
#include <winapi20/detail/exception.h>
#include <winapi20/wrappers/pid.h>

namespace winapi
{
  class WINAPI20_EXPORT ConsoleHost
  {
   public:
    enum class Mode
    {
      Stdout = 1 << 0,
      Stderr = 1 << 1,
      Stdin  = 1 << 2,
      All    = Stdout | Stderr | Stdin
    };

    explicit ConsoleHost(Mode mode) noexcept(false);
    explicit ConsoleHost(Mode mode, PID pid) noexcept(false);
    ~ConsoleHost() noexcept;

    ConsoleHost(ConsoleHost const&) = delete;
    ConsoleHost& operator=(ConsoleHost const&) = delete;
    ConsoleHost(ConsoleHost&&) = delete;
    ConsoleHost& operator=(ConsoleHost&&) = delete;

    [[nodiscard]] auto is_attached() const noexcept -> bool;

    [[nodiscard]] auto title() const noexcept(false) -> std::string;
    [[nodiscard]] auto original_title() const noexcept(false) -> std::string;
    auto set_title(std::string_view title) const noexcept(false) -> void;

   public:
    static auto free() noexcept -> void;

   private:
    bool m_attached;
    std::array<std::FILE*, 3> m_handles;
    std::array<void*, 3> m_original_handles;
  };
} // namespace winapi