#include <winapi20/impl/libloaderapi_impl.h>

#include <array>
#include <winapi20/detail/cvt.h>
#include <winapi20/detail/exception.h>
#include <winapi20/detail/windows_headers.h>
#include <winapi20/impl/errhandlingapi_impl.h>
#include <psapi.h>

using std::array;
using std::string;
using std::optional;
using std::nullopt;
using std::string_view;

namespace winapi::dll
{
  Library::~Library() {
    if(this->handle() and this->m_cleanup)
      ::FreeLibrary(this->handle().as<HMODULE>());
  }

  Library::Library(std::string name, Library::HandleFlags flags) noexcept(false)
  {
    // get this library handle

  }

  Library::Library(std::string name, Handle&& handle, PID pid, bool cleanup)
    : m_name(std::move(name))
    , m_handle(std::move(handle))
    , m_pid(pid)
    , m_cleanup(cleanup)
  {}

  Library::Library(OwnershipMode mode, std::string name, PID pid, Library::HandleFlags flags) noexcept(false)
    : m_name(std::move(name))
    , m_pid(pid)
    , m_cleanup(false)
  {
    if(mode != OwnershipMode::View)
      throw std::logic_error("not implemented for now");
    if(bool(flags & HandleFlags::WeakReference) and bool(flags & HandleFlags::Pin))
      throw std::invalid_argument("invalid flags combination: cannot pin and weak reference at the same time");

    if(pid.is_current()) {
      this->m_handle = Handle(::GetModuleHandleA(this->m_name.c_str()), Cleanup::Manual);
      if(not this->m_handle)
        throw windows_exception("failed to view library via name in current process");
      return;
    } else {
      auto buf = HMODULE();
      if(not ::GetModuleHandleExA(static_cast<DWORD>(flags), this->m_name.c_str(), &buf))
        throw windows_exception("failed to view library via name in another process");
      if(bool(flags & HandleFlags::WeakReference))
        this->m_cleanup = true;
      this->m_handle = Handle(buf, Cleanup::Manual);
      return;
    }
  }

  Library::Library(
      OwnershipMode mode,
      MemoryAddress const& address,
      PID pid,
      Library::HandleFlags flags
  ) noexcept(false)
    : m_name(string())
    , m_pid(pid)
    , m_cleanup(false)
  {
    if(mode != OwnershipMode::View)
      throw std::logic_error("not implemented for now");
    if(bool(flags & HandleFlags::WeakReference) and bool(flags & HandleFlags::Pin))
      throw windows_exception("invalid flags combination: cannot pin and weak reference at the same time");

    if(pid.is_current())
      throw windows_exception("cannot load library via address from current process");

    auto buf = HMODULE();
    if(not ::GetModuleHandleExA(
        static_cast<DWORD>(flags) | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        static_cast<LPCSTR>(*address),
        &buf
    ))
      throw windows_exception("failed to load library via address");
    if(bool(flags & HandleFlags::WeakReference))
      m_cleanup = true;
    try {
      this->m_name = file_path().filename().string();
    } catch(...) {
      m_name = fmt::format("UnknownLibrary(0x{:x})", reinterpret_cast<uintptr_t>(static_cast<void*>(address)));
    }
    this->m_handle = Handle(buf, Cleanup::Manual);
  }

  auto Library::file_path() const noexcept(false) -> std::filesystem::path {
    auto buf = array<wchar_t, MAX_PATH>();
    if(this->m_pid.is_current()) {
      if(not ::GetModuleFileNameW(this->handle().as<HMODULE>(), buf.data(), buf.size()))
        throw windows_exception(fmt::format("failed to get file path of loaded library {} ({})", this->name(), this->handle()));
    } else {
      auto proc = Process(this->m_pid, AccessRights::QueryInformation | AccessRights::VirtualMemoryRead);
      if(not ::GetModuleFileNameExW(
          *proc.handle(),
          this->handle().as<HMODULE>(),
          buf.data(),
          buf.size())
      )
        throw windows_exception(fmt::format("failed to get file path of external library {} ({}), reason: {}",
          this->name(),
          this->handle(),
          last_error_string()
        ));
    }
    return std::filesystem::path{buf.data()};
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
