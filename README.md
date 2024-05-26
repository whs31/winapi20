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
I was tired of using outdated raw C Windows API in my modern projects, so I decided to write my own C++20 API wrapper, which is focusing on safety and extensive use of modern C++20 features such as templates, constexpr and operator overloading.
Also, flooding the global namespace with Windows API types, functions and especially **macros** (which breaks existing STL code btw) is not a good practice for me.

This library provides _partial_ wrapper for Windows API functions and types. Feel free to extend it and submit PRs.

Remark: `UNICODE` macro is automatically defined when compiling this library. Idk if this is a good idea, but docs said ANSI versions are deprecated, so it's not a big deal.

### Features
- Wraps some Win32API functions and types into nice classes.
- Replaces `GetLastError()`-based error handling with `winapi::windows_exception`, which provides string error message out-of-the-box and can be used in `try` blocks.
- No more `void*` or `wchar_t*`-ptr based type erasing. All fields and arguments are wrapped into STL (if possible) or my own newtypes (so paths becomes `std::filesystem::path` instead of `LPCWSTR`).
- All Win32API headers are hidden in implementation. No more polluting global namespace with strange stuff.
- Also, working with `wchar_t` strings is a pain, so all strings in this library is good old `std::string`, which is internally converted to correct `wchar_t` representation if needed.
- See [Examples](#examples) below for other cool features!

### Dependencies
- **C++20 compiler**. I tested it with MinGW, feel free to sumbit issues if you want for me to add support for other compilers.
- **CMake 3.16** at least.
- [**fmt**](https://github.com/fmtlib/fmt). You can install it via `conan`, `vcpkg` or `msys2` (see [Installation](#installation)).

### Contributing
If you want to contribute, please see [CONTRIBUTING.md](CONTRIBUTING.md).

### Installation/building
I recommend using [**conan**](https://github.com/conan-io/conan) to build this library.
Run:
```shell
conan install . -b "missing" -s "build_type=Release"
cmake --preset "conan-release" .
cmake --build build/Release
cmake --install build/Release --prefix "where-you-want-the-library-to-be-installed"
```

Alternatively, you can build it yourself with CMake, passing prefix path:
```shell
cmake -S . -B target -DCMAKE_PREFIX_PATH="path-to-fmt" -DCMAKE_BUILD_TYPE=Release
cmake --build target
cmake --install target --prefix "<where-you-want-the-library-to-be-installed>"
```

If you want to build tests and examples, use following commands:

For **conan**:
```shell
conan install . -b missing -s "build_type=Release" -o "winapi20/*:test=True" -o "winapi20/*:examples=True"
cmake --preset "conan-release" .
cmake --build build/Release
```

For **cmake**:
```shell
cmake -S . -B target -DCMAKE_PREFIX_PATH="<path-to-fmt>" -DCMAKE_BUILD_TYPE=Release -DTESTS=ON -DEXAMPLES=ON
cmake --build target --target all
````

### Examples
#### Call function from loaded library in any process
Finds process by name, retrieves dll with given name from it and calls function with given name in it, returning result.

```cpp
// wrap this in try block if you want to handle errors properly.

// make snapshot of system processes
auto snapshot = Snapshot(Snapshot::IncludeFlags::Process, PID::current());

// find process by name
auto process = snapshot.find_first_by_name<ProcessEntry>("explorer.exe");
if(not process)
  return 1;
fmt::print("found explorer.exe: {}\n", *process);

// open view-only handle to library
auto lib = dll::Library(View, "kernel32.dll", process->pid); 
fmt::print("library path: {}\n", lib.file_path().string());
auto fn = lib["MulDiv"]; // aka `lib.exported_function_address("MulDiv")`;

if(not fn)
  return 1;
auto const res = fn->call<int>(10, 2, 5);
fmt::print("calling function MulDiv(10, 2, 5) = {}\n", *fn);
fmt::print("10 * 2 / 5 = {}\n", res);
```

Possible output:
```shell
found explorer.exe: ProcessEntry { pid: PID(585656), parent_pid: PID(76060), thread_count: 182... 
library path: C:\Windows\System32\KERNEL32.DLL
calling function MulDiv(10, 2, 5) = Function(0x7ffd429e0930)
10 * 2 / 5 = 4
```

[Source Code](./examples/call_external_function.cc)

#### Get modules from process
Finds process by name and retrieve module with given name from it.

```cpp
auto snapshot = Snapshot(Snapshot::IncludeFlags::Process, PID::current());

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