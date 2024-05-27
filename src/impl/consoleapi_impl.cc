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

#include <io.h>
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
  struct FileDescriptor
  {
    decltype(stdout) type = nullptr;
    fpos_t pos = 0;
    int handle = -1;

    auto init(decltype(stdout) t) noexcept -> void {
      this->type = t;
      ::fflush(this->type);
      ::fgetpos(this->type, &this->pos);
      this->handle = ::_dup(::_fileno(this->type));
    }

    auto close() noexcept -> void {
      ::_dup2(this->handle, _fileno(this->type));
      ::_close(this->handle);
      ::clearerr(stdout);
      ::fsetpos(stdout, &this->pos);
    }
  };
  auto already_allocated = false;

  FileDescriptor out;
  FileDescriptor err;
  FileDescriptor in;

  auto try_alloc() noexcept(false) -> void {
    if(not AllocConsole())
      throw winapi::windows_exception(winapi::last_error_string());
  }

  auto try_attach(const uint32_t pid) noexcept(false) -> void {
    if(not AttachConsole(pid))
      throw winapi::windows_exception(winapi::last_error_string());
  }

  auto try_free() noexcept -> bool {
    if(not ::FreeConsole())
      return false;
    return true;
  }
}

namespace winapi
{
  ConsoleHost::ConsoleHost(const Mode mode) noexcept(false)
    : m_attached(false)
    , m_handles({nullptr, nullptr, nullptr})
  {
    try {
      ::try_alloc();
    } catch(windows_exception const&) {
      try {
        ::try_free();
        ::try_alloc();
      } catch(...) {
        throw ;
      }
    }

    if(::already_allocated)
      throw windows_exception("console is already allocated");

    out.init(stdout);
    err.init(stderr);
    in.init(stdin);

    if(not not(mode & Mode::Stdout))
      if(::freopen_s(&this->m_handles[0], "CONOUT$", "w", stdout))
        fatal_application_exit("failed to create console handles (stdout)");
    if(not not(mode & Mode::Stderr))
      if(::freopen_s(&this->m_handles[1], "CONOUT$", "w", stderr))
        fatal_application_exit("failed to create console handles (stderr)");
    if(not not(mode & Mode::Stdin))
      if(::freopen_s(&this->m_handles[2], "CONIN$", "w+", stdin))
        fatal_application_exit("failed to create console handles (stdin)");
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
    this->m_attached = true;
  }

  ConsoleHost::ConsoleHost(Mode mode, PID pid) noexcept(false)
    : m_attached(false)
    , m_handles({nullptr, nullptr, nullptr})
  {
    try {
      ::try_attach(*pid);
    } catch(windows_exception const&) {
      try {
        ::try_free();
        ::try_attach(*pid);
      } catch(...) {
        throw;
      }
    }

    if(::already_allocated)
      throw windows_exception("console is already allocated");

    out.init(stdout);
    err.init(stderr);
    in.init(stdin);

    if(not not(mode & Mode::Stdout))
      if(::freopen_s(&this->m_handles[0], "CONOUT$", "w", stdout))
        fatal_application_exit("failed to create console handles (stdout)");
    if(not not(mode & Mode::Stderr))
      if(::freopen_s(&this->m_handles[1], "CONOUT$", "w", stderr))
        fatal_application_exit("failed to create console handles (stderr)");
    if(not not(mode & Mode::Stdin))
      if(::freopen_s(&this->m_handles[2], "CONIN$", "w+", stdin))
        fatal_application_exit("failed to create console handles (stdin)");
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
    out.close();
    err.close();
    in.close();
    ConsoleHost::free();
    ::already_allocated = false;
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