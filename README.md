lazy-winapi.c
=============

[![Build status](https://ci.appveyor.com/api/projects/status/3csvlsc4rixq167l/branch/master?svg=true)](https://ci.appveyor.com/project/DoumanAsh/lazy-winapi-c/branch/master)
[Documentation](https://doumanash.github.io/lazy-winapi.c/)
[Source code](https://github.com/DoumanAsh/lazy-winapi.c/archive/master.zip)

Simple and lazy wrapper over WinAPI

Requirements
------------

* C99 compiler;
* Windows platform;
* CMake (to build from source).

Usage
-----

#### Build from source

Headers are together with source code in `src/lazy_winapi`

```bash
mkdir build
cd build/
cmake ../
make
```

**Options:**

* `UNIT_TESTING` - build unit tests;
* `BUILD_SHARED` - build shared library instead of static.

**Commands:**

* `make lazy_winapi` - build the whole library.
* `make cppcheck` - Run code lint check. Works only if cppcheck is installed.
* `make test` - Run unit tests

### Import source code directly

In `src/lazy_winapi` you can find source code for each module.

Modules are designed to be independent so you can take source code of any module and import it into your project.

### Import to your CMake project

```cmake
set(MY_PREFIX <some_dir>)
include(ExternalProject)
ExternalProject_Add(lazy-winapi
    URL https://github.com/DoumanAsh/lazy-winapi.c/archive/master.zip

    INSTALL_COMMAND ""
    PREFIX ${MY_PREFIX}
)
add_dependencies(<my_target> LazyWinApi)
include_directories(SYSTEM ${MY_PREFIX}/src/lazy-winapi/src)
target_link_libraries(<my_target> ${MY_PREFIX}/src/lazy-winapi-build/liblazy_winapi.a)
```

Modules
-------

Each module has a corresponding `.c` & `.h` files.
Module are independent of each other so you're free to take a particular module sources.
And import it into your project.

### [Clipboard](https://doumanash.github.io/lazy-winapi.c/group__Clipboard.html)

Provides utilities to access Windows clipboard.

### [Error](https://doumanash.github.io/lazy-winapi.c/group__Error.html)

Utilities to work with WinAPI error.
