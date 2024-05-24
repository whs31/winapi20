#include <winapi20/impl/consoleapi_impl.h>

#include <array>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <winapi20/detail/exception.h>
#include <winapi20/detail/windows_headers.h>

using std::array;
using std::string;
using std::string_view;
using std::wstring;

namespace {
  auto try_alloc() noexcept(false) -> void {
    if(not AllocConsole())
      throw winapi::windows_exception(winapi::last_error_string());
  }

  auto try_attach(uint32_t pid) noexcept(false) -> void {
    if(not AttachConsole(pid))
      throw winapi::windows_exception(winapi::last_error_string());
  }
}

namespace winapi
{
  ConsoleHost::ConsoleHost() noexcept(false)
    : m_attached(false)
    , m_stdout(nullptr)
    , m_stdin(nullptr)
  {
    try {
      ::try_alloc();
    } catch(windows_exception const&) {
      try {
        ConsoleHost::free();
        ::try_alloc();
      } catch(windows_exception const& e) {
        throw e;
      }
    }
    ::freopen_s(&this->m_stdout, "CONOUT$", "w", stdout);
    ::freopen_s(&this->m_stdin, "CONIN$", "r", stdin);
    this->m_attached = true;
  }

  ConsoleHost::ConsoleHost(uint32_t pid) noexcept(false)
      : m_attached(false)
      , m_stdout(nullptr)
      , m_stdin(nullptr)
  {
    try {
      ::try_attach(pid);
    } catch(windows_exception const&) {
      try {
        ConsoleHost::free();
        ::try_attach(pid);
      } catch(windows_exception const& e) {
        throw e;
      }
    }
    ::freopen_s(&this->m_stdout, "CONOUT$", "w", stdout);
    ::freopen_s(&this->m_stdin, "CONIN$", "r", stdin);
    this->m_attached = true;
  }

  auto ConsoleHost::free() noexcept(false) -> void {
    if(not FreeConsole())
      throw windows_exception(winapi::last_error_string());
  }

  ConsoleHost::~ConsoleHost() noexcept {
    ConsoleHost::free();
    ::fclose(this->m_stdout);
    ::fclose(this->m_stdin);
  }

  auto ConsoleHost::is_attached() const noexcept -> bool { return this->m_attached; }
  auto ConsoleHost::title() const noexcept(false) -> std::string {
    if(not this->is_attached())
      throw windows_exception("Console is not attached");
    auto buf = array<wchar_t, 256>();
    if(not GetConsoleTitleW(buf.data(), buf.size()))
      throw windows_exception(winapi::last_error_string());
    auto str = wstring(buf.cbegin(), buf.cend());
    str.resize(wcslen(str.c_str()));
    return detail::into<string>(str);
  }

  auto ConsoleHost::original_title() const noexcept(false) -> std::string {
    if(not this->is_attached())
      throw windows_exception("Console is not attached");
    auto buf = array<wchar_t, 256>();
    if(not GetConsoleOriginalTitleW(buf.data(), buf.size()))
      throw windows_exception(winapi::last_error_string());
    auto str = wstring(buf.cbegin(), buf.cend());
    str.resize(wcslen(str.c_str()));
    return detail::into<string>(str);
  }

  auto ConsoleHost::set_title(std::string_view title) const noexcept(false) -> void {
    if(not this->is_attached())
      throw windows_exception("Console is not attached");
    if(not SetConsoleTitleW(detail::into<wstring>(title).data()))
      throw windows_exception(winapi::last_error_string());
  }
}