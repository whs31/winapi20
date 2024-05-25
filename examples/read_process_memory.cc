#include <winapi20/winapi.h>

using namespace winapi;
using namespace winapi::th32;

auto main() -> int
{
  // make snapshot of system processes
  auto snapshot = Snapshot(Snapshot::IncludeFlags::Process, PID::CurrentProcess);

  // find process by name
  auto game = snapshot.find_first_by_name<ProcessEntry>("dota2.exe");
  if(not game)
    return 1;
  fmt::print("found dota2.exe: {}\n", *game);

  // make snapshot of modules of the process
  auto module_snapshot = Snapshot(Snapshot::IncludeFlags::Module | Snapshot::IncludeFlags::Process, game->pid);

  // get vector of all modules
  auto modules = module_snapshot.modules();
  // exclude all non-game modules (not containing "dota 2 beta" in path)
  modules.erase(std::remove_if(
      modules.begin(),
      modules.end(),
      [](auto& m) { return m.path.string().find("dota 2 beta") == std::string::npos; }), modules.end()
  );
  fmt::print("listing game modules:\n");
  fmt::print("\t{}", fmt::join(modules, "\n\t"));

  // find module by name in process
  auto client_dll = module_snapshot.find_first_by_name<ModuleEntry>("client.dll");
  if(not client_dll)
    return 1;
  fmt::print("\nfound client.dll: {}\n", *client_dll);

  // get handle to module
  fmt::print("client.dll handle: {}\n", client_dll->handle);
  fmt::print("client.dll base address: {}\n", client_dll->base_address);
  return 0;
}