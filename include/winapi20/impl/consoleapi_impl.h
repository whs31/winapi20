#pragma once

#include <cstdint>
#include <winapi20/detail/export.h>
#include <winapi20/detail/cvt.h>
#include <winapi20/detail/exception.h>

namespace winapi
{
  class WINAPI20_EXPORT ConsoleHost
  {
   public:
    explicit ConsoleHost() noexcept(false);
    explicit ConsoleHost(uint32_t pid) noexcept(false);
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
    static auto free() noexcept(false) -> void;

   private:
    bool m_attached;
    std::FILE* m_stdout;
    std::FILE* m_stdin;
  };
} // namespace winapi