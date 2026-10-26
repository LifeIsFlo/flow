# Flow

A small, Bash-based build system for C++ projects whose layout separates platform-independent code (`common/`) from Linux- and Windows-specific code (`<platform>/`). Flow scaffolds directories, is intended to migrate existing sources into the common header and source roots, compiles sources for a chosen platform and configuration, links them into an executable, and can generate a VS Code IntelliSense configuration.

> **Platform support:** only the Linux build has been exercised so far. Windows targets are cross-compiled with MinGW (`x86_64-w64-mingw32-g++`), and the generated editor settings assume a Linux host. Flow is a Bash script, not a POSIX `sh` script: it relies on arrays, `[[ ]]`, `shopt`, and `echo -e`.

## Requirements

- Bash (`/bin/bash`)
- `g++` for Linux builds, and/or `x86_64-w64-mingw32-g++` for Windows cross-builds
- `jq` (used to read and write `flow.properties`)
- `rsync` (used by `init` to migrate existing sources)
- `git` (only for `./flow -u`)
- Standard utilities: `find`, `mkdir`, `cp`, `rm`, `mktemp`

## Installation

Place the `flow` script in the root of your project, make it executable, and run it from that directory. `-i` requires a platform:

```sh
chmod +x flow
./flow -p linux -i
```

Flow always operates on the current working directory (`pwd`), so it must be invoked from the project root.

## Quick start

```sh
./flow -p linux -i           # create common/ and linux/ directories and flow.properties
./flow -p linux -g           # write .vscode/c_cpp_properties.json
./flow -p linux -b debug     # compile and link a Linux debug build
./flow -p linux -b release   # compile and link a Linux release build
./flow -p linux -r debug     # prepare (rebuild) and run the debug build
./flow -s                    # show the working directory and project metadata
./flow -c all                # remove build output and generated files
```

The platform flag must come **before** the command it applies to, because Flow executes each option as it parses it (`-b debug -p linux` fails with a "platform must be defined" error). `-p` is required for `-b`, `-r`, `-g`, and `-i`.

> **Known blockers:** the current script cannot complete a build or migrate sources. The `init` rsync filters exclude everything before the include rules are reached, so no files are moved. The build creates its object folders, but the main source is either skipped by the `common/` loop or compiled to a hidden `.o` that the link glob does not match, so linking fails. The generated `c_cpp_properties.json` is also invalid JSON (missing comma). See [Status and limitations](#status-and-limitations) for details.

## Project structure

`./flow -p <platform> -i` creates `common/` and `<platform>/` subdirectories under each of the four roots. Only the platform passed to `-p` is created:

```
project-root/
├── flow                    # the build system script
├── flow.properties         # project metadata (JSON)
│
├── include/                # header root
│   ├── common/             # headers shared by every platform
│   └── <platform>/         # headers used only by the selected platform
│
├── headers/                # second header root
│   ├── common/             # headers shared by every platform
│   └── <platform>/         # headers used only by the selected platform
│
├── src/                    # source root
│   ├── common/             # sources compiled on every platform
│   └── <platform>/         # sources compiled only for the selected platform
│
├── assets/                 # assets
│   ├── common/             # assets shared by all platforms
│   └── <platform>/         # assets used only by the selected platform
│
├── build/                  # created by the build
│   ├── bin/
│   │   └── <mode>/<platform>/<projectName>
│   └── obj/
│       └── <mode>/<platform>/
│           ├── common/     # objects from src/common/*.cpp
│           └── platform/   # objects from src/<platform>/*.cpp
│
├── logs/                   # created by prepareRun
└── .vscode/                # generated editor configuration
```

`<mode>` is the exact string passed to `-b`/`-r` (`debug` or `release`), so builds land in `build/bin/debug/linux`, `build/bin/release/windows`, and so on.

`<platform>` is whatever was passed to `-p`; there is no validation beyond accepting the literal strings `linux` and `windows`, so `-p linux -i` creates `linux/` but not `windows/`.

### What goes where

| Location | Purpose |
| --- | --- |
| `include/common/`, `headers/common/` | Headers valid on every platform; both are include roots for common sources |
| `include/<platform>/`, `headers/<platform>/` | Headers valid only on that platform (OS APIs, platform types) |
| `src/common/` | Sources compiled on every platform |
| `src/<platform>/` | Platform implementations; also where the build looks for `mainFile` |
| `assets/common/` | Assets shared by all platforms |
| `assets/<platform>/` | Assets that only make sense on one platform |
| `build/bin/<mode>/<platform>/` | Linked executable for that configuration and platform |
| `build/obj/<mode>/<platform>/common/`, `build/obj/<mode>/<platform>/platform/` | Object files for common and platform sources |

Include paths are not the bare `include/` and `headers/` roots. Common sources are compiled with `-I include/common -I headers/common`; platform sources add `-I include/<platform> -I headers/<platform>`. Platform sources therefore see `headers/common` as well, but not `include/common`.

The intended pattern is a shared interface plus per-platform implementations, for example:

```
headers/common/Platform.h     # declares the interface, included everywhere
headers/linux/Platform.h      # Linux-specific detail included from Platform.h
headers/windows/Platform.h    # Windows-specific detail included from Platform.h
src/linux/Platform.cpp        # Linux implementation
src/windows/Platform.cpp      # Windows implementation
```

## Project metadata

`flow.properties` is a small JSON file read with `jq`. `./flow -i` writes these defaults when the file does not exist:

```json
{
  "projectName": "Example",
  "flowVersion": "v1.0.0",
  "cVersion": "c++17",
  "mainFile": "Program.cpp"
}
```

- `projectName` becomes the name of the linked executable (`build/bin/<mode>/<platform>/<projectName>`).
- `flowVersion` is informational and shown by `./flow -s`.
- `cVersion` is passed to the compiler as `-std=<value>`.
- `mainFile` is the basename Flow treats as the executable's entry point. It is compiled from `src/<platform>/<mainFile>` and skipped by the `src/common/` compile loop.

`./flow -i` only writes `flow.properties` when it is missing; an existing file is left untouched and reported as skipped. Edit the file to rename the project or change the language standard.

## Command reference

```
./flow -p <platform>   select the target platform (linux or windows)
./flow -b <mode>       build        compile sources and link the executable
./flow -r <mode>       run          prepare the build and run it (run is a stub)
./flow -c <value>      clean        remove build output or generated files
./flow -g              genVsCodeRunsLinux  write .vscode/c_cpp_properties.json
./flow -i              init         scaffold directories and migrate sources
./flow -h              tasks        list available tasks
./flow -u              updateFlow   download the latest Flow files into this project
./flow -s              status       print the working directory and project metadata
```

- `<mode>` for `-b`/`-r` must be `debug` or `release`; anything else is rejected with an error.
- `<platform>` must be `linux` or `windows`; there is no default, so `-p` is required for `-b`, `-r`, `-g`, and `-i`. `./flow -b debug` without `-p` exits with an error.
- `-c` takes its own value (`debug`, `release`, `logs`, `runs`, or anything else for a full clean) and does not require `-p`.
- `-h`, `-s`, and `-u` do not require `-p`.
- Invalid flags print the task list and exit with a non-zero status.

Options are processed left to right, and each action runs immediately. That means `-p` must precede `-b`, `-r`, `-g`, or `-i`, and a single invocation can chain actions, e.g. `./flow -p linux -b debug`.

Flow uses single-letter `getopts` options. The names listed by `./flow -h` are task labels, not positional commands: `./flow build debug` does nothing, use `./flow -p linux -b debug`.

### `build` (`-p <platform> -b <mode>`)

```sh
./flow -p linux -b debug
./flow -p linux -b release
./flow -p windows -b debug   # cross-compile with MinGW
```

1. Requires `platform` to be `linux` or `windows` and `mode` to be `debug` or `release`.
2. Selects the compiler: `g++` on Linux, `x86_64-w64-mingw32-g++` on Windows.
3. Creates `build/bin`, `build/obj`, `build/bin/<mode>/<platform>/`, and `build/obj/<mode>/<platform>/common/` plus `build/obj/<mode>/<platform>/platform/`.
4. If `src/<platform>/<mainFile>` exists, compiles it into the platform object folder with:

   ```sh
   $CC -c src/<platform>/<mainFile> -I include/common -I headers/common -std=<cVersion> \
       -o build/obj/<mode>/<platform>/platform/<object>
   ```

5. Compiles every top-level `src/common/*.cpp` except the file whose basename equals `mainFile` into `build/obj/<mode>/<platform>/common/` with:

   ```sh
   $CC -c <file> -I include/common -I headers/common -std=<cVersion> \
       -o build/obj/<mode>/<platform>/common/<file>.o
   ```

6. Compiles every top-level `src/<platform>/*.cpp` except `mainFile` into `build/obj/<mode>/<platform>/platform/` with:

   ```sh
   $CC -c <file> -I include/<platform> -I headers/<platform> -I headers/common -std=<cVersion> \
       -o build/obj/<mode>/<platform>/platform/<file>.o
   ```

7. Links every `*.o` from `build/obj/<mode>/<platform>/common/` and `build/obj/<mode>/<platform>/platform/` into `build/bin/<mode>/<platform>/<projectName>`. If no objects are found, it prints "No object files found to link" and exits.

The build stops at the first source that fails to compile. `debug` and `release` currently differ only in output directories: no extra flags (`-g`, `-O2`, …) are passed to `$CC`.

Current issues prevent a successful build:

- **The main file is never linked.** The main block expands `$(basename "${f%.cpp}.o")`, but `$f` is unset there, so the object is written as a hidden file named `.o`. The link step's `*.o` glob does not match dotfiles, so that object is omitted from the link.
- **The main file can be skipped entirely.** The `common/` loop skips any file named `mainFile`, while the main block only looks in `src/<platform>/`. With the layout `init` produces (sources migrated to `src/common/`), `Program.cpp` matches neither path and is never compiled, so the link fails with a missing `main`.
- **No explicit `main` fallback.** Nothing warns that the entry point was skipped; the failure surfaces only as a linker error (or as "No object files found to link" when `src/common/` is empty).

### `run` (`-p <platform> -r <mode>`)

```sh
./flow -p linux -r debug
```

Validates the platform and mode, then calls `prepareRun` — so it rebuilds the project and ensures `logs/` exists — before printing `no run function yet`. Launching the executable is **not implemented**.

### `prepareRun`

Builds the project and creates a `logs/` directory. It is listed by `./flow -h` but has no command-line flag of its own; it runs as part of `./flow -r`.

### `clean` (`-c <value>`)

```sh
./flow -c debug   # removes build/bin/debug and build/obj/debug
./flow -c release # removes build/bin/release and build/obj/release
./flow -c logs    # removes logs/
./flow -c runs    # removes .vscode/ and run/
./flow -c all     # any other value: removes build/bin/, build/obj/, .vscode/, logs/, run/
```

Clean is not platform-aware: cleaning a mode removes that mode for every platform. Source, header, and asset directories are never touched. The mode names are lowercase and match the build output directories.

### `init` (`-i`)

`./flow -p <platform> -i` requires a platform and performs the following:

1. Creates `common/` and `<platform>/` under `include/`, `headers/`, `src/`, and `assets/`.
2. Writes `flow.properties` with the four sample values **only if it does not already exist**.
3. Prints every `.h`, `.hpp`, `.c`, and `.cpp` file found under the project root by `find` (including files already inside `headers/` and `src/`).
4. Prompts for confirmation, then runs two dry-run rsyncs that are intended to show the migration plan:
   - `.h`/`.hpp` → `headers/common/`
   - `.c`/`.cpp` → `src/common/`
5. Asks you to approve the structure, then repeats the two rsyncs for real (with `--ignore-existing`, and `--remove-source-files` if you opted in).

The intended filters preserve subdirectories, exclude `.git/`, `assets/`, `include/`, `headers/`, and `src/`, and copy only the relevant extensions. However, the commands place `--exclude='*'` before the `--include` rules and never include directories, so every path is excluded before the includes can match. In practice **both the dry runs and the real copies transfer nothing**; `init` only creates directories and `flow.properties`.

`init` does **not** write or modify sample source files, and it does **not** generate VS Code files (that is `-g`). When `init` finishes it re-reads `flow.properties`.

### `status` (`-s`)

Prints the absolute working directory. If `flow.properties` exists it also prints `projectName`, `flowVersion`, and `cppVersion` (the value of `cVersion`); otherwise it reports that the project needs initialisation with `./flow -i` (the message omits the required `-p`).

### `updateFlow` (`-u`)

Clones `https://github.com/LifeIsFlo/CPP-Project-System.git` (depth 1) into a temporary directory, copies its contents into the project root, and removes the temporary directory. The copy uses `cp "$tmpdir"/* "$ROOT/"` without `-r`, so only top-level files are copied; upstream subdirectories are skipped with a warning. Treat this as experimental and review what it touches before running it on a project with local changes.

## VS Code integration

`./flow -p <platform> -g` creates `.vscode/c_cpp_properties.json` if it does not already exist. It does not use the platform value, but `-p` is still required because the option handler validates it first.

The generated file contains a single configuration named **Windows on Linux**:

- include paths `${workspaceFolder}/headers/common` and `${workspaceFolder}/include/common`
- `/usr/bin/g++`, `c17`, `c++17`, `linux-gcc-x64`

Things to check after generation:

- The two `includePath` entries are missing a comma between them, so the file is not valid JSON and editors will reject it.
- Platform include paths (`headers/<platform>`, `include/<platform>`) are not listed, and the configuration claims "Windows on Linux" while using the Linux compiler and `linux-gcc-x64`.
- `genVsCodeRunsLinux` only produces `c_cpp_properties.json`: it does not create `assets/`, platform folders, `tasks.json`, or `launch.json`. `init` does not call it.

## How Flow works

- The script captures the project root once at startup from `pwd`; it must be started from the project root.
- Configuration is a single JSON file read with `jq`; no makefiles, CMake, or other tooling is involved.
- Common and platform sources are compiled separately into `common/` and `platform/` object folders under `build/obj/`, then linked in one `$CC` call.
- Only the top level of `src/common/` and `src/<platform>/` is scanned; nested folders are ignored.
- Status messages are colourised ANSI tags: `[COMPILING]`, `[SUCCES]` (as spelled in the script), `[FAILED]`, and `[INFO]`.

## Status and limitations

| Area | State |
| --- | --- |
| Linux build | Object folders are created and sources compile, but the main file is skipped or written as a hidden `.o`, so linking cannot produce a program |
| Windows build | Cross-compiler selected, untested; no `.exe` suffix on the output |
| macOS | Not supported |
| Platform selection | Explicit via `-p`; no auto-detection, and it must precede `-b`/`-r`/`-g`/`-i` |
| Object folders | `common/` and `platform/` are created before compiling |
| Empty platform sources | Skipped by the `[ -f "$f" ]` guard in the compile loops |
| Linking | Globs `build/obj/<mode>/<platform>/{common,platform}/*.o` |
| Main file | Only looked for in `src/<platform>/`; skipped by the common loop |
| Main object filename | Empty `$f` in the main block produces an object named `.o` |
| Platform include paths | Platform sources see `headers/common` but not `include/common` |
| `run` | Builds, then prints a stub message |
| Shared library build | Removed from the current CLI |
| Compiler flags | No debug symbols or optimisation flags, no warning flags |
| Source discovery | Top-level `src/common/*.cpp` and `src/<platform>/*.cpp` only |
| Object layout | Flat inside `common/` and `platform/`, so identical basenames collide |
| Incremental builds | None; every build recompiles everything |
| Clean by mode | Removes lowercase `build/bin/debug`/`build/obj/debug` (matches build output) |
| Source migration | `init` rsync filters select no files, so nothing is migrated |
| Sample files | `init` generates none; existing project files are meant to be migrated |
| Generated VS Code config | Missing comma makes `c_cpp_properties.json` invalid; no `tasks.json` |
| `updateFlow` | Experimental; non-recursive copy from the upstream repository |

## Available tasks

`./flow -h` prints the task names the script recognises:

```
build
tasks
run
prepareRun
genVsCodeRunsLinux
displayTasks
updateFlow
status
clean
init
```

The list does not mention the `-p` platform option or the actual flag syntax; see [Command reference](#command-reference).
