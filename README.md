# MyOpenGLBook

A C++ OpenGL learning project structured like LearnOpenGL, designed for macOS M1 Pro with CMake.

## Project Structure

```
MyOpenGLBook/
├── CMakeLists.txt                 # Root CMake configuration
├── common/                        # Common library
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── common.hpp            # Common utilities and classes
│   └── src/
│       └── common.cpp            # Common implementation
├── 1.getting_started/            # Chapter 1: Getting Started
│   ├── CMakeLists.txt
│   └── 1.1_hello_window/         # Lesson 1.1: Hello Window
│       ├── CMakeLists.txt
│       ├── main.cpp
│       ├── shaders/              # Shader files
│       └── resources/            # Resource files
└── 2.lighting/                   # Chapter 2: Lighting
    ├── CMakeLists.txt
    └── 2.1_colors/               # Lesson 2.1: Colors
        ├── CMakeLists.txt
        ├── main.cpp
        ├── shaders/
        └── resources/
```

## Dependencies

This project uses CMake's FetchContent to automatically download:
- **GLFW 3.3.8** - Window management
- **GLAD v0.1.36** - OpenGL function loading
- **GLM 0.9.9.8** - Mathematics library
- **stb** - Image loading utilities

## Prerequisites

### macOS M1 Pro Requirements

1. **Xcode Command Line Tools**:
   ```bash
   xcode-select --install
   ```

2. **CMake** (version 3.16 or higher):
   ```bash
   # Using Homebrew
   brew install cmake
   
   # Or using MacPorts
   sudo port install cmake
   ```

3. **Git** (for FetchContent):
   ```bash
   # Usually pre-installed, but if needed:
   brew install git
   ```

## Building the Project

### Quick Build (Recommended)
```bash
# From the project root directory
./build.sh
```

### Manual Build

#### 1. Clone and Navigate
```bash
cd /Users/jarukit/Documents/GitHub/3D-GAME-DEVELOPMENT
```

#### 2. Create Build Directory
```bash
mkdir build
cd build
```

#### 3. Configure with CMake
```bash
# For Debug build (recommended for development)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# For Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### 4. Build the Project
```bash
# Build all targets
make -j$(sysctl -n hw.ncpu)

# Or build specific target
make hello_window
make colors
```

## Running the Examples

### Hello Window (Lesson 1.1)
```bash
# From the build directory
./1.getting_started/1.1_hello_window/1.1_hello_window
```

### Colors (Lesson 2.1)
```bash
# From the build directory
./2.lighting/2.1_colors/2.1_colors
```

## Controls

- **ESC**: Close the window and exit the program

## Features

### Common Library
The `common` library provides:
- **Window Management**: Easy GLFW window creation and management
- **Shader Utilities**: Shader loading, compilation, and uniform setting
- **OpenGL Info**: Automatic OpenGL information display
- **File Utilities**: File reading functions

### macOS M1 Pro Optimizations
- Configured for ARM64 architecture
- macOS-specific framework linking (Cocoa, IOKit, CoreVideo)
- OpenGL Core Profile 3.3
- Forward compatibility enabled

## Development Workflow

1. **Add New Lessons**:
   - Create new lesson directory under appropriate chapter
   - Add `CMakeLists.txt` for the lesson
   - Create `main.cpp` and link with `common` library
   - Add shader and resource directories

2. **Add New Chapters**:
   - Create chapter directory
   - Add `CMakeLists.txt` with `add_subdirectory()` calls
   - Update root `CMakeLists.txt` to include new chapter

3. **Using Shaders**:
   - Place shader files in lesson's `shaders/` directory
   - Use `SHADER_ROOT` macro in code for shader paths
   - Example: `Common::Shader shader(SHADER_ROOT "/vertex.glsl", SHADER_ROOT "/fragment.glsl");`

4. **Using Resources**:
   - Place resource files in lesson's `resources/` directory
   - Use `RESOURCE_ROOT` macro in code for resource paths

## Troubleshooting

### Common Issues

1. **CMake not found**:
   ```bash
   brew install cmake
   ```

2. **Build errors on M1**:
   - Ensure you're using ARM64 version of CMake
   - Check that Xcode Command Line Tools are installed

3. **OpenGL context creation failed**:
   - Make sure you're running on macOS 10.15 or later
   - Check that your graphics drivers are up to date

4. **Dependencies not downloading**:
   - Check internet connection
   - Ensure Git is installed and accessible

### Clean Build
If you encounter issues, try a clean build:
```bash
rm -rf build
mkdir build
cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

## Next Steps

This project structure is designed to grow with your OpenGL learning journey. Each lesson builds upon the previous ones, and the common library provides a solid foundation for more complex graphics programming.

Happy coding! 🎮