#include <winapi20/winapi.h>

using namespace winapi;
using namespace winapi::th32;

auto main() -> int try
{
  using enum OwnershipMode;

  // make snapshot of system processes
  auto snapshot = Snapshot(Snapshot::IncludeFlags::Process, PID::current());

  // find process by name
  auto process = snapshot.find_first_by_name<ProcessEntry>("explorer.exe");
  if(not process)
    return 1;
  fmt::print("found explorer.exe: {}\n", *process);

  auto lib = dll::Library(View, "kernel32.dll", process->pid);
  fmt::print("library path: {}\n", lib.file_path().string());
  auto fn = lib["MulDiv"];

  if(not fn)
    return 1;
  auto res = fn->call<int>(10, 2, 5);
  fmt::print("calling function MulDiv(10, 2, 5) = {}\n", *fn);
  fmt::print("10 * 2 / 5 = {}\n", res);
  return 0;
} catch(std::exception const& e) {
  fmt::print("error: {}\n", e.what());
  return 1;
}