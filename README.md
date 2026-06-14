# [stellarlib](https://github.com/zanadoman/stellarlib)

Opinionated, cross-platform game engine library in [C++23](https://en.cppreference.com/w/cpp/23.html), built on [SDL3](https://www.libsdl.org/), focusing on simplicity and performance.

## Features/Roadmap

- [x] Project Setup
  - [x] CMake, Clang, and Doxygen integration
  - [x] Linux, Windows, and Android support
- [x] Standard Library Extensions
  - [x] Vector/arena allocators
  - [x] Bitset utilities
  - [x] Extended type properties
  - [x] Sequential/type ID generators
  - [x] Compile-time padding
  - [x] File system utilities
  - [x] Utility containers
- [x] Entity Component System
  - [x] Simple, consistent, and foolproof interface
  - [x] Deferred modifications during iterations
  - [ ] Parent–child entity relationships
  - [ ] World serialization/deserialization
- [x] Linear Algebra Utilities
  - [x] Generic vector and matrix implementation
  - [x] Common shader intrinsics functions
  - [x] Common 2D/3D transformations
- [x] Resource Management Utilities
  - [x] Image resource
- [x] Graphics and Rendering Abstractions
  - [x] Texture resource
  - [ ] Common interfaces
- [x] Application Runtime
  - [x] Scene management
  - [x] Metadata subsystem
  - [x] Clock subsystem
  - [ ] Window/Renderer subsystem
  - [ ] Input subsystem
  - [ ] Universal renderer
  - [ ] Audio subsystem

## Build Instructions

### Prerequisites

- [CMake](https://archlinux.org/packages/extra/x86_64/cmake/)
- [Ninja](https://archlinux.org/packages/extra/x86_64/ninja/)
- [Clang](https://archlinux.org/packages/extra/x86_64/clang/)
- [LLD](https://archlinux.org/packages/extra/x86_64/lld/)
- [SDL3](https://archlinux.org/packages/extra/x86_64/sdl3/)
- [DirectX Shader Compiler](https://aur.archlinux.org/packages/directx-shader-compiler-git)

<details>
<summary>aarch64-linux-gnu</summary>

- [GCC](https://archlinux.org/packages/extra/x86_64/aarch64-linux-gnu-gcc/)
- [pkg-config](https://aur.archlinux.org/packages/aarch64-linux-gnu-pkg-config)
- [SDL3](https://github.com/libsdl-org/SDL/releases/latest)

</details>

<details>
<summary>x86_64-w64-mingw32</summary>

- [MinGW-w64](https://archlinux.org/packages/extra/x86_64/mingw-w64-gcc/)
- [SDL3](https://aur.archlinux.org/packages/mingw-w64-sdl3)
- [Wine](https://archlinux.org/packages/extra/x86_64/wine/)

</details>

<details>
<summary>x86_64-linux-android</summary>

- [Android NDK](https://aur.archlinux.org/packages/android-ndk)
- [OpenJDK 8](https://archlinux.org/packages/extra/x86_64/jdk8-openjdk/)
- [Android SDK](https://aur.archlinux.org/packages/android-sdk)
```sh
env JAVA_HOME=/usr/lib/jvm/java-8-openjdk /opt/android-sdk/tools/bin/sdkmanager 'platforms;android-36' 'build-tools;35.0.0' platform-tools
```
- [SDL3](https://aur.archlinux.org/packages/android-x86-64-sdl3)
- [OpenJDK 21](https://archlinux.org/packages/extra/x86_64/jdk21-openjdk/)

</details>

<details>
<summary>aarch64-linux-android</summary>

- [Android NDK](https://aur.archlinux.org/packages/android-ndk)
- [OpenJDK 8](https://archlinux.org/packages/extra/x86_64/jdk8-openjdk/)
- [Android SDK](https://aur.archlinux.org/packages/android-sdk)
```sh
env JAVA_HOME=/usr/lib/jvm/java-8-openjdk /opt/android-sdk/tools/bin/sdkmanager 'platforms;android-36' 'build-tools;35.0.0' platform-tools
```
- [SDL3](https://aur.archlinux.org/packages/android-aarch64-sdl3)
- [OpenJDK 21](https://archlinux.org/packages/extra/x86_64/jdk21-openjdk/)

</details>

<details>
<summary>Development Tools</summary>

- [clangd](https://archlinux.org/packages/extra/x86_64/clang/)
- [Valgrind](https://archlinux.org/packages/extra/x86_64/valgrind/)
- [Vulkan Validation Layers](https://archlinux.org/packages/extra/x86_64/vulkan-validation-layers/)
- [RenderDoc](https://archlinux.org/packages/extra/x86_64/renderdoc/)

</details>

<details>
<summary>Testing Tools</summary>

- [GoogleTest (x86_64-linux-gnu)](https://archlinux.org/packages/extra/x86_64/gtest/)
- [GoogleTest (aarch64-linux-gnu)](https://github.com/google/googletest/releases/latest)
- [GoogleTest (x86_64-w64-mingw32)](https://aur.archlinux.org/packages/mingw-w64-gtest)
- [GoogleTest (x86_64-linux-android)](https://aur.archlinux.org/packages/android-x86-64-gtest)
- [GoogleTest (aarch64-linux-android)](https://aur.archlinux.org/packages/android-aarch64-gtest)

</details>

<details>
<summary>Documentation Generator</summary>

- [Doxygen](https://archlinux.org/packages/extra/x86_64/doxygen/)

</details>

### Setup

```sh
cmake --preset x86_64-linux-gnu-debug                                   # Select a default a preset for development
cmake --build build/x86_64-linux-gnu/debug -t compile_commands.json     # Expose compile_commands.json for clangd
```

### Build & Run

The CMake configuration defines the following build targets:

- `stellarlib` (default target)
- `examples` (each `.cpp` file in the `examples` directory can be built as its own target (e.g., `example.cpp` &rarr; `example`))
- `apks` (when targeting Android, each example has an associated APK target (e.g., `example` &rarr; `example_apk`))
- `tests` (each `.cpp` file in the `tests` directory can be built as an individual target (e.g., `example.cpp` &rarr; `example_test`))
- `docs` (requires Doxygen)

```sh
cmake --preset x86_64-linux-gnu-release                     # Configure CMake
cmake --build build/x86_64-linux-gnu/release                # Build stellarlib
cmake --install build/x86_64-linux-gnu/release              # Install stellarlib
cmake --build build/x86_64-linux-gnu/release -t example     # Build a target
build/x86_64-linux-gnu/release/example                      # Run the target
```

<details>
<summary>aarch64-linux-gnu</summary>

```sh
cmake --preset aarch64-linux-gnu-release                     # Configure CMake
cmake --build build/aarch64-linux-gnu/release                # Build stellarlib
cmake --install build/aarch64-linux-gnu/release              # Install stellarlib
cmake --build build/aarch64-linux-gnu/release -t example     # Build a target
build/aarch64-linux-gnu/release/example                      # Run the target
```

</details>

<details>
<summary>x86_64-w64-mingw32</summary>

```sh
cmake --preset x86_64-w64-mingw32-release                     # Configure CMake
cmake --build build/x86_64-w64-mingw32/release                # Build stellarlib
cmake --install build/x86_64-w64-mingw32/release              # Install stellarlib
cmake --build build/x86_64-w64-mingw32/release -t example     # Build a target
build/x86_64-w64-mingw32/release/example.exe                  # Run the target
```

</details>

<details>
<summary>x86_64-linux-android</summary>

```sh
cmake --preset x86_64-linux-android-release                                                                                                  # Configure CMake
cmake --build build/x86_64-linux-android/release                                                                                             # Build stellarlib
cmake --install build/x86_64-linux-android/release                                                                                           # Install stellarlib
env JAVA_HOME=/usr/lib/jvm/java-21-openjdk ANDROID_HOME=/opt/android-sdk cmake --build build/x86_64-linux-android/release -t example_apk     # Build a target
/opt/android-sdk/platform-tools/adb install -r build/x86_64-linux-android/release/example.apk                                                # Install the target
```

</details>

<details>
<summary>aarch64-linux-android</summary>

```sh
cmake --preset aarch64-linux-android-release                                                                                                  # Configure CMake
cmake --build build/aarch64-linux-android/release                                                                                             # Build stellarlib
cmake --install build/aarch64-linux-android/release                                                                                           # Install stellarlib
env JAVA_HOME=/usr/lib/jvm/java-21-openjdk ANDROID_HOME=/opt/android-sdk cmake --build build/aarch64-linux-android/release -t example_apk     # Build a target
/opt/android-sdk/platform-tools/adb install -r build/aarch64-linux-android/release/example.apk                                                # Install the target
```

</details>

## [Documentation](https://zanadoman.github.io/stellarlib/)

```sh
$BROWSER build/x86_64-linux-gnu/release/html/index.html     # Browse local documentation (requires Doxygen)
```

## Contributing

Contributions are welcome!

- Fork the repository
- Make your changes in accordance with the existing style
- Open a pull request targeting the `develop` branch

---

Enjoy!

Domán Zana
