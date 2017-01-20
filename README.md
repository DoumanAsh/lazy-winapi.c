lazy-winapi.c
--------

[Documentation](https://doumanash.github.io/lazy-winapi.c/)

Simple and lazy wrapper over WinAPI


Modules
=======

Each module has a corresponding `.c` & `.h` files.
Module are independent of each other so you're free to take a particular Modules
And import it into your project.

* Clipboard - provides utilities to access Windows clipboard.
* Error - utilities to work with WinAPI error.

Build
=====

For now CMake assumes that your compiler is either GCC or Clang
