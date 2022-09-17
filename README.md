# signature

## Prerequisites:
- installed python
- installed conan
- installed c++ build environment

## Linux build commands:
```
@ mkdir build
@ cd build
@ cmake .. -DCMAKE_BUILD_TYPE=Debug; and make; and ctest
@ ./bin/signature --help
or
@ ./bin/signature -i file -o result -b 512
```
## My linux environment:
```
@ uname -a
Linux DESKTOP-7POAGJB 5.10.102.1-microsoft-standard-WSL2 #1 SMP Wed Mar 2 00:30:59 UTC 2022 x86_64 GNU/Linux
@ gcc --version
gcc (GCC) 12.2.0
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

@ cmake --version
cmake version 3.24.1

CMake suite maintained and supported by Kitware (kitware.com/cmake).
@ python --version
Python 3.10.6
@ conan --version
Conan version 1.51.3
```
## Windows build steps:
```
- "Clone a repository" 
- F7 or "Build" -> "Build All"
- Select startup item: signature or runUnitTests
- F5 or "Debug" -> "Start debugging"
or
- Ctrl-F5 or "Debug" -> "Start without debugging"
```

## My windows environment:
Microsoft Visual Studio Community 2022 (64-bit) - Current
Version 17.3.3

Visual C++ 2022   00482-90000-00000-AA045
Microsoft Visual C++ 2022

C++ CMake tools for Windows
