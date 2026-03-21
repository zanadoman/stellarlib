# [stellarlib](https://github.com/zanadoman/stellarlib)

## Roadmap

- [x] Entity Component System
  - [x] Handle nested queries
  - [x] Handle nested modifications
- [ ] Linear Algebra Module

# Build Instructions

### Prerequisites

- [CMake](https://archlinux.org/packages/extra/x86_64/cmake/)
- [Ninja](https://archlinux.org/packages/extra/x86_64/ninja/)
- [Clang](https://archlinux.org/packages/extra/x86_64/clang/)
- [LLD](https://archlinux.org/packages/extra/x86_64/lld/)
- [SDL3](https://archlinux.org/packages/extra/x86_64/sdl3/)
- [DirectX Shader Compiler](https://aur.archlinux.org/packages/directx-shader-compiler-git)

<details>
<summary><strong>Recommended</strong></summary>

- [Valgrind](https://archlinux.org/packages/extra/x86_64/valgrind/)
- [Vulkan Validation Layers](https://archlinux.org/packages/extra/x86_64/vulkan-validation-layers/)
- [RenderDoc](https://archlinux.org/packages/extra/x86_64/renderdoc/)

</details>

<details>
<summary><strong>Cross-Compilation</strong></summary>

- x86_64-w64-mingw32
  - [MinGW-w64](https://archlinux.org/packages/extra/x86_64/mingw-w64-gcc/)
  - [SDL3](https://aur.archlinux.org/packages/mingw-w64-sdl3)
  - [Wine](https://archlinux.org/packages/extra/x86_64/wine/)
- x86_64-linux-android/aarch64-linux-android
  - [OpenJDK 8](https://archlinux.org/packages/extra/x86_64/jdk8-openjdk/)
  - [OpenJDK 21](https://archlinux.org/packages/extra/x86_64/jdk21-openjdk/)
  - [Android SDK](https://aur.archlinux.org/packages/android-sdk)
  - [Android NDK](https://aur.archlinux.org/packages/android-ndk)
  - [SDL3 (x86_64-linux-android)](https://aur.archlinux.org/packages/android-x86-64-sdl3)
  - [SDL3 (aarch64-linux-android)](https://aur.archlinux.org/packages/android-aarch64-sdl3)

</details>

<details>
<summary><strong>Testing</strong></summary>

- [GoogleTest (x86_64-linux-gnu)](https://archlinux.org/packages/extra/x86_64/gtest/)
- [GoogleTest (x86_64-w64-mingw32)](https://aur.archlinux.org/packages/mingw-w64-gtest)
- [GoogleTest (x86_64-linux-android)](https://aur.archlinux.org/packages/android-x86-64-gtest)
- [GoogleTest (aarch64-linux-android)](https://aur.archlinux.org/packages/android-aarch64-gtest)

</details>

### Configuration

<details>
<summary><strong>x86_64-linux-android/aarch64-linux-android</strong></summary>

```sh
env JAVA_HOME=/usr/lib/jvm/java-8-openjdk /opt/android-sdk/tools/bin/sdkmanager platform-tools 'platforms;android-36' 'build-tools;35.0.0'
```

</details>

### Build

```sh
cmake -B build -G Ninja --toolchain cmake/x86_64-linux-gnu.cmake     # Configure cmake
cmake --build build                                                  # Build project
cmake --install build                                                # Install project
cmake --build build -t example                                       # Build a target
build/example                                                        # Run the target
```

<details>
<summary><strong>Cross-Compilation</strong></summary>

- x86_64-w64-mingw32
```sh
cmake -B build -G Ninja --toolchain cmake/x86_64-w64-mingw32.cmake     # Configure cmake
cmake --build build                                                    # Build project
cmake --install build                                                  # Install project
cmake --build build -t example                                         # Build a target
build/example.exe                                                      # Run the target
```
- x86_64-linux-android
```sh
cmake -B build -G Ninja --toolchain cmake/x86_64-linux-android.cmake          # Configure cmake
cmake --build build                                                           # Build project
cmake --install build                                                         # Install project
env JAVA_HOME=/usr/lib/jvm/java-21-openjdk cmake --build build -t example     # Build a target
/opt/android-sdk/platform-tools/adb install -r build/example.apk              # Install the target
```
- aarch64-linux-android
```sh
cmake -B build -G Ninja --toolchain cmake/aarch64-linux-android.cmake         # Configure cmake
cmake --build build                                                           # Build project
cmake --install build                                                         # Install project
env JAVA_HOME=/usr/lib/jvm/java-21-openjdk cmake --build build -t example     # Build a target
/opt/android-sdk/platform-tools/adb install -r build/example.apk              # Install the target
```

</details>
