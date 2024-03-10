# TastyCode

[tastycode.dev](https://tastycode.dev) is a blog about programming. This repository contains
accompanying C++ code discussed in TastyCode.


## Build

### Windows

In Visual Studio open and use as a
[CMake project](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio).

### Linux

Tested on Ubuntu 22.04.3 LTS (WSL) with GCC 11 (see `Makefile` for more options):

- **Config**

  ```
  make config
  ```

- **Build & Run Tests**

  ```
  make check
  ```
