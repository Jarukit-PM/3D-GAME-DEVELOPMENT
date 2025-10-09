# 3D Game Development - Assignment 2: Kinetic Sculpture Animation

A comprehensive 3D kinetic sculpture animation project built with OpenGL, featuring dynamic movement patterns, interactive controls, and parametric geometric decorations.

## 🎨 Project Overview

This project implements a sophisticated 3D kinetic sculpture animation that combines:
- **19 Animated Elements**: Central rotating cube, pendulum system, orbital mechanics, and floating elements
- **3 Parametric Patterns**: External geometric decorations with complex orbital motion
- **Interactive Controls**: Wind simulation, user interaction, and real-time parameter adjustment
- **Advanced Rendering**: Phong lighting, depth testing, and efficient OpenGL rendering

## 📁 Project Structure

```
3D-GAME-DEVELOPMENT/
├── Assignment_2:3D_kinetic_sculpture_animation/    # Main assignment project
│   ├── README.md                                   # Detailed project documentation
│   ├── main.cpp                                    # Main application code
│   ├── CMakeLists.txt                              # Build configuration
│   ├── images/                                     # Screenshots directory
│   ├── video/                                      # Video demonstration
│   └── resources/                                  # Assets and shaders
│       ├── vs/kinetic_sculpture.vs                 # Vertex shader
│       ├── fs/kinetic_sculpture.fs                 # Fragment shader
│       └── parametric_pattern_2.dxf/              # 3D model files
│           ├── scene.gltf                          # GLTF model
│           ├── f.bin                               # Binary data
│           └── license.txt                        # Model license
├── common/                                         # Shared utilities
│   ├── CMakeLists.txt
│   ├── include/common.hpp
│   └── src/common.cpp
├── CMakeLists.txt                                  # Root build configuration
└── build.sh                                        # Build script
```

## 🚀 Quick Start

### Prerequisites
- **macOS M1 Pro** (optimized for ARM64)
- **Xcode Command Line Tools**
- **CMake 3.16+**
- **Git**

### Build and Run
```bash
# Clone the repository
cd /Users/jarukit/Documents/GitHub/3D-GAME-DEVELOPMENT

# Build the project
./build.sh

# Run the kinetic sculpture animation
cd build/Assignment_2:3D_kinetic_sculpture_animation
./Assignment_2_3D_kinetic_sculpture_animation
```

## 🎮 Controls

- **WASD**: Move camera
- **Mouse**: Look around
- **Scroll**: Zoom in/out
- **ESC**: Exit application

## 🎯 Features

### Kinetic Sculpture Elements
- **Central Rotating Element**: Complex multi-axis rotation
- **Pendulum System**: Four synchronized pendulums with realistic physics
- **Orbital Mechanics**: Six elements with dynamic radius variation
- **Floating Elements**: Eight small cubes with complex 3D patterns
- **Parametric Patterns**: Three geometric decorations orbiting the sculpture

### Interactive Features
- **Wind Simulation**: Dynamic wind effects affecting all elements
- **User Control**: Real-time parameter adjustment
- **Environmental Response**: Elements react to wind strength and user input

### Technical Features
- **OpenGL 3.3 Core Profile**: Modern graphics programming
- **Phong Lighting**: Ambient, diffuse, and specular lighting
- **GLTF Model Loading**: Custom loader for 3D models
- **Frame-rate Independent**: Smooth animation regardless of FPS

## 📊 Technical Specifications

- **Language**: C++17
- **Graphics API**: OpenGL 3.3 Core Profile
- **Window Management**: GLFW 3.3.8
- **Mathematics**: GLM 0.9.9.8
- **Model Format**: GLTF 2.0
- **Rendering**: Line-based parametric patterns + Triangle-based cubes

## 🎨 Model Credits

- **Parametric Pattern**: "Parametric_pattern_2.dxf" by STEELWORX
  - Source: https://www.fab.com/listings/84a8bd54-e2a1-4d3e-aa83-95ec46e13daa
  - License: Creative Commons Attribution (CC BY 4.0)
  - Used as decorative geometric patterns with orbital animation

## 📚 Documentation

For detailed information about the project, including:
- Complete feature descriptions
- Technical implementation details
- Screenshots and video demonstration
- Model integration techniques
- Future enhancement plans

Please see the [Assignment 2 README](Assignment_2:3D_kinetic_sculpture_animation/README.md)

## 🛠️ Development

### Adding New Features
1. Modify `main.cpp` for new animation patterns
2. Update shaders in `resources/vs/` and `resources/fs/`
3. Add new models to `resources/` directory
4. Update `CMakeLists.txt` if needed

### Build System
- Uses CMake with FetchContent for automatic dependency management
- Optimized for macOS M1 Pro with ARM64 architecture
- Supports both Debug and Release builds

## 🎓 Educational Value

This project demonstrates:
- **3D Graphics Programming**: OpenGL rendering pipeline
- **Mathematical Concepts**: Vector operations, matrix transformations, trigonometry
- **Animation Systems**: Frame-rate independent timing, interpolation
- **Model Integration**: GLTF loading, mesh processing, buffer management
- **Interactive Design**: User input handling, real-time parameter adjustment

## 📈 Performance

- **Rendering**: 19 animated elements + 3 parametric patterns
- **Vertex Count**: ~7,500 vertices for parametric patterns
- **Frame Rate**: 60+ FPS on macOS M1 Pro
- **Memory Usage**: Optimized buffer management with VAO/VBO/EBO

## 🔮 Future Enhancements

- Enhanced GLTF parser with full 2.0 specification support
- Texture mapping for parametric patterns
- Particle effects synchronized with animations
- Sound integration for wind effects
- VR support for immersive experience

---

*Created for Assignment 2: 3D Kinetic Sculpture Animation*  
*Course: Computer Graphics and Game Development*  
*Submission Date: October 2024*

Happy coding! 🎮✨