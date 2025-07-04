# Simulation Engine

Welcome to the **Simulation Engine** project! This is a high-performance and modular simulation framework written in **C++**, designed for creating real-time physical simulations, custom 3D environments, and dynamic systems.

---

## Features

* ✅ Written in modern **C++23**
* ✅ Modular architecture for easy extension
* ✅ Real-time 3D rendering with **OpenGL**
* ✅ Dynamic lighting with **GLSL** shaders
* ✅ Built-in asset manager and entity-component system (ECS)
* ✅ Interoperability with external assets (e.g. **Assimp** loader)

### Prerequisites:

* CMake >= 3.16
* C++23-compatible compiler (MSVC, g++, clang++)
* GPU OpenGL >= 4.4 Core version
* Operating System Windows 10 1903 or newer

## Build Instructions

```bash
# Clone the repository
$ git clone https://github.com/RafaPramudya/SimulationEngine.git
$ cd SimulationEngine

# Update external submodules
$ git submodule update --init --recursive

# Create a build directory
$ mkdir build && cd build

# Generate build files using CMake
$ cmake ..

# Build the project
$ cmake --build .
```
### Build Notes
If your GPU or CPU doesnt support 64-bit application then compile the cmake with Win32 or x86 architecture
- MSVC :
``` bash
# Change MSVC_VERSION to available version
# (e.g. "Visual Studio 17 2022") or cmake --help for more version
$ cmake -A Win32 -G MSVC_VERSION ..
```
- GCC(Gnu C Compiler) and Clang :
``` bash
# Change MSVC_VERSION to available version
# (e.g. "Visual Studio 17 2022") or cmake --help for more version
$ cmake -DCMAKE_CXX_FLAGS=-m32 ..
```

---

## Usage

You can run the simulation by executing the compiled binary:

```bash
$ ./Engine
```

---

## Roadmap / TODO

* [ ] Add GUI using Dear ImGui
* [ ] Expand parallel computing (OpenCL-based)
* [ ] Integrate particle systems
* [ ] Scripting support (Lua or Python)
* [ ] Support for multiplayer or networked simulation
<!-- 
## License

This project is licensed under the **MIT License**. See [LICENSE](LICENSE) for more information. -->

---

## Contributions

Contributions, bug reports, and feature requests are welcome!

* Fork the project
* Create a feature branch
* Submit a pull request

---

## Author

**Rafa Pramudya Susanto**

Feel free to reach out with ideas, feedback, or collaboration inquiries. :>