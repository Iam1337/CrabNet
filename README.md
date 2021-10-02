# CrabNet - networking engine for game programmers

[![⚙ Build And Release](https://github.com/Iam1337/CrabNet/actions/workflows/ci.yml/badge.svg)](https://github.com/Iam1337/CrabNet/actions/workflows/ci.yml)
[![](https://img.shields.io/badge/license-BSD-green.svg)](https://github.com/Iam1337/CrabNet/blob/master/LICENSE)

### Table of Contents
- [Introduction](#introduction)
- [Build](#build)
- - [Windows](#windows)
- - [Android (on Windows)](#android-on-windows)
- - [Linux](#linux)


## Introduction
Fork based on  [CrabNet](https://github.com/TES3MP/CrabNet). Made to work with [StolenNetwork](https://github.com/Iam1337/StolenNetwork) framework, the main difference is the presence of a simplified wrapper.

**This fork is not compatible with the original RakNet.**

**Notes:**
* The Help directory contains index.html, which provides full documentation and help
* The Source directory contain all files required for the core of CrabNet and is used
if you want to use the source in your program or create your own dll
* The Samples directory contains code samples and one game using an older version of RakNet.
The code samples each demonstrate one feature of CrabNet. The game samples cover several features.

## Build

The instructions are based on the personal experience of the developer and may not be accurate. It would be really cool if someone could improve the assembly instructions. 

### Windows
This example shows how to build release x64 libraries for Windows.

**Required:**
 * [CMake](https://cmake.org/) >= 3.15 (Must in `PATH` environment)
 * [Visual Studio 16 2019](https://visualstudio.microsoft.com/vs/) with Desktop Development C++

**PowerShell:**
```powershell
# Create build directory
mkdir build-win
cd build-win

# Setup project
# You can build 32-bit library using instead of x64 - Win32
cmake -G "Visual Studio 16 2019" -A x64 ..

# Build library
cmake --build . --config Release --clean-first
```

### Android (on Windows)
This example shows how to build release arm64-v8a libraries for Android. Best for building libs for Oculus Quest and Oculus Quest 2.  

**Required:**
 * [Android NDK](https://developer.android.com/ndk/downloads) (tested on r23)
 * [Ninja](https://ninja-build.org/) (developer use instalation via [Chocolatey](https://community.chocolatey.org/packages/ninja))

**PowerShell:**
```powershell
# Create build directory
mkdir build-android
cd build-android

# Download and unpack Android NDK
# You can skip these steps if you already have Android NDK
Invoke-WebRequest "https://dl.google.com/android/repository/android-ndk-r23-windows.zip" -OutFile "android-ndk.zip"
Expand-Archive -Path "android-ndk.zip" -DestinationPath .

# Setup project
# If you skipped the steps to download the Android NDK, then you need to replace the path in CMAKE_TOOLCHAIN_FILE with your own. 
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE="android-ndk-r23/build/cmake/android.toolchain.cmake" -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=23 ..

# Build library
cmake --build . --config Release --clean-first
```

### Linux

The developer tested this method on Arch Linux under WSL2.

**Required:**
 * [CMake](https://cmake.org/) >= 3.15

**Terminal:**
```
# Create build directory
mkdir build-linux
cd build-linux

# Setup project
cmake ..

# Build project
cmake --build . -config Release --clean-first
```

## License
This project is under the BSD License.

Copyright (c) 2014, Oculus VR, Inc. (original RakNet)<br>
Copyright (c) 2016-2018, TES3MP Team<br>
Copyright (c) 2021, Sigalkin Vladimir
