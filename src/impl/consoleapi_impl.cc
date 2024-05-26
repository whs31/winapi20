#include <winapi20/impl/consoleapi_impl.h>

#if defined(_MSC_VER)
# if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#   define _AMD64_
# elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) || defined(_M_IX86)
#   define _X86_
# elif defined(__arm__) || defined(_M_ARM) || defined(_M_ARMT)
#   define _ARM_
# endif
# include <consoleapi.h>
#endif // _MSC_VER

#include <array>
#include <iostream>
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

  auto try_free() noexcept -> bool {
    if(not ::FreeConsole())
      return false;
    return true;
  }

  auto get_handles() noexcept(false) -> array<void*, 3> {
    auto handles = array<void*, 3>();
    handles[0] = reinterpret_cast<void*>(::GetStdHandle(STD_OUTPUT_HANDLE));
    handles[1] = reinterpret_cast<void*>(::GetStdHandle(STD_ERROR_HANDLE));
    handles[2] = reinterpret_cast<void*>(::GetStdHandle(STD_INPUT_HANDLE));
    return handles;
  }

  auto set_handles(array<void*, 3> const& handles) noexcept -> void {
    ::SetStdHandle(STD_OUTPUT_HANDLE, handles[0]);
    ::SetStdHandle(STD_ERROR_HANDLE, handles[1]);
    ::SetStdHandle(STD_INPUT_HANDLE, handles[2]);
  }

  auto set_handles(array<std::FILE*, 3> const& handles) noexcept -> void {
    ::SetStdHandle(STD_OUTPUT_HANDLE, handles[0]);
    ::SetStdHandle(STD_ERROR_HANDLE, handles[1]);
    ::SetStdHandle(STD_INPUT_HANDLE, handles[2]);
  }
}

namespace winapi
{
  ConsoleHost::ConsoleHost(Mode mode) noexcept(false)
    : m_attached(false)
    , m_handles({nullptr, nullptr, nullptr})
    , m_original_handles(::get_handles())
  {
    try {
      ::try_alloc();
    } catch(windows_exception const&) {
      try {
        ::try_free();
        ::try_alloc();
      } catch(windows_exception const& e) {
        throw e;
      }
    }

    if(bool(mode & Mode::Stdout))
      ::freopen_s(&this->m_handles[0], "CONOUT$", "w", stdout);
    if(bool(mode & Mode::Stderr))
      ::freopen_s(&this->m_handles[1], "CONOUT$", "w", stderr);
    if(bool(mode & Mode::Stdin))
      ::freopen_s(&this->m_handles[2], "CONIN$", "r", stdin);

    ::set_handles(this->m_handles);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
    this->m_attached = true;
  }

  ConsoleHost::ConsoleHost(Mode mode, PID pid) noexcept(false)
    : m_attached(false)
    , m_handles({nullptr, nullptr, nullptr})
    , m_original_handles(::get_handles())
  {
    try {
      ::try_attach(*pid);
    } catch(windows_exception const&) {
      try {
        ::try_free();
        ::try_attach(*pid);
      } catch(windows_exception const& e) {
        throw e;
      }
    }
    if(bool(mode & Mode::Stdout))
      ::freopen_s(&this->m_handles[0], "CONOUT$", "w", stdout);
    if(bool(mode & Mode::Stderr))
      ::freopen_s(&this->m_handles[1], "CONOUT$", "w", stderr);
    if(bool(mode & Mode::Stdin))
      ::freopen_s(&this->m_handles[2], "CONIN$", "r", stdin);
    ::set_handles(this->m_handles);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
    this->m_attached = true;
  }

  auto ConsoleHost::free() noexcept -> void {
    if(not ::try_free())
      ::MessageBoxW(nullptr, L"Failed to free the console!", nullptr, MB_ICONEXCLAMATION);
  }

  ConsoleHost::~ConsoleHost() noexcept {
    for(auto& h : this->m_handles)
      if(h)
        ::fclose(h);
    ConsoleHost::free();
    ::set_handles(this->m_original_handles);
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