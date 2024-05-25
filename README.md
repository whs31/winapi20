# WinAPI20
<h3 align="center">
    <img src="./docs/images/logo2.png" alt=""/><br/>
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/>
    Modern C++20 API for Windows
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/><br/>
    <img src="https://raw.githubusercontent.com/catppuccin/catppuccin/main/assets/misc/transparent.png" height="30" width="0px"/><br/>
    <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="cpp_badge"/><img src="https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="cmake_badge"/>
</h3>

### Description

### Installation

### Examples
#### Get modules from process
Finds process by name and retrieve module with given name from it.

```cpp
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
    [](auto& m) { return m.path.find("dota 2 beta") == std::string::npos; }), modules.end()
);
fmt::print("listing game modules:\n");
fmt::print("{}", fmt::join(modules, "\n\t"));

// find module by name in process
auto client_dll = module_snapshot.find_first_by_name<ModuleEntry>("client.dll");
if(not client_dll)
  return 1;
fmt::print("\nfound client.dll: {}\n", *client_dll);

// get handle to module
fmt::print("client.dll handle: {}\n", client_dll->handle);
fmt::print("client.dll base address: {}\n", client_dll->base_address);
return 0;
```

Possible output:
```shell
found dota2.exe: ProcessEntry { pid: 346156, parent_pid: 517408, thread_count: 82, thread_base_priority: 8, name: dota2.exe }
listing game modules:
    ModuleEntry { pid: 346156, base_address: 140699576827904, size: 2416640, handle: Handle(0x7ff72c4c0000), name: dota2.exe, path: C:\Program Files (x86)\Steam\steamapps\common\dota 2 beta\game\bin\win64\dota2.exe }
    ... 
    ModuleEntry { pid: 346156, base_address: ...
    
found client.dll: ModuleEntry { pid: 346156, base_address: 140722899255296, size: 85884928, handle: Handle(0x7ffc9a6c0000), name: client.dll, path: C:\Program Files (x86)\Steam\steamapps\common\dota 2 beta\game\dota\bin\win64\client.dll }
client.dll handle: Handle(0x7ffc9a6c0000)
client.dll base address: 140722899255296
```
[Source Code](./examples/read_process_memory.cc)
