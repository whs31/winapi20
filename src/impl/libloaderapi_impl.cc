#include <winapi20/impl/libloaderapi_impl.h>

#include <winapi20/detail/cvt.h>
#include <winapi20/detail/exception.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/impl/errhandlingapi_impl.h>

using std::optional;
using std::nullopt;
using std::string_view;

namespace winapi::dll
{
  Library::~Library() {
    if(this->handle() and this->m_cleanup)
      ::FreeLibrary(this->handle().as<HMODULE>());
  }

  auto Library::view(std::string name, PID pid, HandleFlags flags) -> optional<Library>
  {
    if(bool(flags & HandleFlags::WeakReference) and bool(flags & HandleFlags::Pin))
      throw windows_exception("invalid flags combination: cannot pin and weak reference at the same time");

    auto self = Library();
    self.m_name = std::move(name);
    self.m_cleanup = false;
    if(pid.is_current()) {
      self.m_handle = Handle(::GetModuleHandleA(self.name().c_str()), Cleanup::Manual);
      if(not self.handle())
        return nullopt;
      return self;
    } else {
      auto buf = HMODULE();
      if(not ::GetModuleHandleExA(static_cast<DWORD>(flags), self.name().c_str(), &buf))
        return nullopt;
      self.m_handle = Handle(buf, Cleanup::Manual);
      if(bool(flags & HandleFlags::WeakReference))
        self.m_cleanup = true;
      return self;
    }
  }

  auto Library::view(MemoryAddress const& address, PID pid, Library::HandleFlags flags) -> optional<Library>
  {
    if(bool(flags & HandleFlags::WeakReference) and bool(flags & HandleFlags::Pin))
      throw windows_exception("invalid flags combination: cannot pin and weak reference at the same time");

    if(pid.is_current())
      throw windows_exception("cannot load library via address from current process");

    auto self = Library();
    self.m_name = "loaded from address"; // todo
    self.m_cleanup = false;
    auto buf = HMODULE();
    if(not ::GetModuleHandleExA(
        static_cast<DWORD>(flags) | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        static_cast<LPCSTR>(*address),
        &buf
    ))
      return nullopt;
    self.m_handle = Handle(buf, Cleanup::Manual);
    if(bool(flags & HandleFlags::WeakReference))
      self.m_cleanup = true;
    return self;
  }

  auto Library::exported_function_address(std::string_view name) const -> optional<Function>
  {
    auto a = FARPROC();
    if(auto res = ::GetProcAddress(this->handle().as<HMODULE>(), name.data()); res == nullptr)
      return nullopt;
    else
      return Function(res);
  }
}
