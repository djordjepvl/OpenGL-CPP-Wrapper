# Simple OpenGL Engine

A small C++/OpenGL wrapper that was meant to grow into a small game. Built for a computer graphics course. Not sure if I will continue working on it or not.

## Features

- Thin OpenGL wrapper classes (shader, VBO, VAO, EBO, texture, mesh, camera, skybox)
- `.glb` 3D model importer (via tinygltf)
- Basic PBR shader
- Close-range shadow mapping
- Free-flying camera

## Controls

| Input | Action |
|---|---|
| Mouse | Look around |
| `W` `A` `S` `D` | Move the camera |
| `←` / `→` | Spin some of the imported 3D models |

## Building

Everything the project needs is in the repo (`dependencies/`): GLAD, GLM, stb, nlohmann/json, tinygltf, and the GLFW **source**, which is compiled together with the project. You only need a C++17 compiler, CMake and OpenGL drivers.

### Windows

1. Install CMake (3.16+) and a MinGW-w64 GCC. The easiest way is:
   ```
   winget install Kitware.CMake
   winget install BrechtSanders.WinLibs.POSIX.UCRT
   ```
   Open a **new** terminal afterwards and check that `g++ --version` and `cmake --version` work.
2. From the project root:
   ```
   cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ```
3. Run it from the build folder, so the `assets` folder is found:
   ```
   cd build
   LearningOpenGL.exe
   ```

The MinGW build links the runtime statically, so the exe doesn't need extra DLLs on other PCs. To share it, zip `LearningOpenGL.exe` together with the `assets` folder.

### Linux

1. Install the dependencies.

   **Arch / CachyOS:**
   ```
   sudo pacman -S --needed base-devel cmake gdb git mesa libx11 libxrandr libxinerama libxcursor libxi xorgproto wayland wayland-protocols libxkbcommon
   ```
   **Debian / Ubuntu:**
   ```
   sudo apt install build-essential cmake gdb git libgl1-mesa-dev xorg-dev libwayland-dev libxkbcommon-dev wayland-protocols
   ```
2. Build:
   ```
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ```
3. Run it from the build folder:
   ```
   cd build
   ./LearningOpenGL
   ```

If GLFW's configure step complains about Wayland or X11 packages, you can turn off the backend you don't use by adding `-DGLFW_BUILD_WAYLAND=OFF` or `-DGLFW_BUILD_X11=OFF` to the first `cmake` command.

### Notes

- **Don't share a `build/` folder between Windows and Linux.** The CMake cache holds absolute paths and the generator, so it breaks on the other OS. Delete it, or use a different folder (`-B build-linux`).
- Assets are copied next to the executable after every build, and loaded with relative paths. Always run the exe from the folder it's in.
- You need GPU drivers with OpenGL support (Mesa for AMD/Intel, the proprietary driver for NVIDIA).
- **VS Code:** install *CMake Tools* plus a debugger extension (*C/C++* for gdb, or *CodeLLDB*), pick the GCC kit, and use the CMake build/run buttons.
- Linux is case-sensitive, so `#include "VBO.h"` must match the real filename exactly.

## Libraries

[GLFW](https://www.glfw.org/), [GLAD](https://glad.dav1d.de/), [GLM](https://github.com/g-truc/glm), [stb](https://github.com/nothings/stb), [nlohmann/json](https://github.com/nlohmann/json), [tinygltf](https://github.com/syoyo/tinygltf)
