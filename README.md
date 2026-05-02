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
  - [ ] File system utilities
- [x] Entity Component System
  - [x] Simple, consistent, and foolproof interface
  - [x] Deferred modifications during iterations
  - [ ] Parent–child entity relationships
  - [ ] World serialization/deserialization
- [x] Linear Algebra Utilities
  - [x] Generic vector and matrix implementation
  - [x] Common shader intrinsics functions
  - [x] Common 2D/3D transformations
- [x] Application Runtime
  - [x] Scene management
  - [x] Metadata subsystem
  - [x] Clock subsystem
  - [ ] Input subsystem
  - [ ] Window subsystem
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

- [Valgrind](https://archlinux.org/packages/extra/x86_64/valgrind/)
- [Vulkan Validation Layers](https://archlinux.org/packages/extra/x86_64/vulkan-validation-layers/)
- [RenderDoc](https://archlinux.org/packages/extra/x86_64/renderdoc/)
- [ImageMagick](https://archlinux.org/packages/extra/x86_64/imagemagick/)

</details>

<details>
<summary>Testing Tools</summary>

- [GoogleTest (x86_64-linux-gnu)](https://archlinux.org/packages/extra/x86_64/gtest/)
- [GoogleTest (x86_64-w64-mingw32)](https://aur.archlinux.org/packages/mingw-w64-gtest)
- [GoogleTest (x86_64-linux-android)](https://aur.archlinux.org/packages/android-x86-64-gtest)
- [GoogleTest (aarch64-linux-android)](https://aur.archlinux.org/packages/android-aarch64-gtest)

</details>

<details>
<summary>Documentation Generator</summary>

- [Doxygen](https://archlinux.org/packages/extra/x86_64/doxygen/)

</details>

### Build & Run

The CMake configuration defines the following build targets:

- `stellarlib` (default target)
- Each `.cpp` file in the `tests` directory can be built as an individual target (e.g., `example.cpp` &rarr; `test_example`)
- `tests` (all tests combined)
- Each `.cpp` file in the `examples` directory can be built as its own target (e.g., `example.cpp` &rarr; `example`)

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja --toolchain cmake/x86_64-linux-gnu.cmake     # Configure CMake
cmake --build build                                                                             # Build stellarlib
cmake --install build                                                                           # Install stellarlib
cmake --build build -t example                                                                  # Build a target
build/example                                                                                   # Run the target
```

<details>
<summary>x86_64-w64-mingw32</summary>

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja --toolchain cmake/x86_64-w64-mingw32.cmake     # Configure CMake
cmake --build build                                                                               # Build stellarlib
cmake --install build                                                                             # Install stellarlib
cmake --build build -t example                                                                    # Build a target
build/example.exe                                                                                 # Run the target
```

</details>

<details>
<summary>x86_64-linux-android</summary>

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja --toolchain cmake/x86_64-linux-android.cmake             # Configure CMake
cmake --build build                                                                                         # Build stellarlib
cmake --install build                                                                                       # Install stellarlib
env JAVA_HOME=/usr/lib/jvm/java-21-openjdk ANDROID_HOME=/opt/android-sdk cmake --build build -t example     # Build a target
/opt/android-sdk/platform-tools/adb install -r build/example.apk                                            # Install the target
```

</details>

<details>
<summary>aarch64-linux-android</summary>

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja --toolchain cmake/aarch64-linux-android.cmake            # Configure CMake
cmake --build build                                                                                         # Build stellarlib
cmake --install build                                                                                       # Install stellarlib
env JAVA_HOME=/usr/lib/jvm/java-21-openjdk ANDROID_HOME=/opt/android-sdk cmake --build build -t example     # Build a target
/opt/android-sdk/platform-tools/adb install -r build/example.apk                                            # Install the target
```

</details>

## [Documentation](https://zanadoman.github.io/stellarlib/)

```sh
$BROWSER build/html/index.html     # Browse local documentation (doxygen required)
```

## Contributing

Contributions are welcome!

- Fork the repository
- Make your changes in accordance with the existing style
- Open a pull request targeting the `develop` branch

---

Enjoy!

Domán Zana
