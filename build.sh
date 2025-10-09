#!/bin/bash

# 3D Game Development Build Script for macOS M1 Pro

set -e  # Exit on any error

echo "🎮 3D Game Development Build Script"
echo "===================================="

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "❌ Error: Please run this script from the project root directory"
    exit 1
fi

# Create build directory
echo "📁 Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "⚙️  Configuring with CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ..

# Build the project
echo "🔨 Building project..."
ninja

echo ""
echo "✅ Build completed successfully!"
echo ""
echo "🚀 Available executables:"
echo "  • Kinetic Sculpture Animation: ./Assignment_2:3D_kinetic_sculpture_animation/Assignment_2_3D_kinetic_sculpture_animation"
echo ""
echo "💡 To run the kinetic sculpture:"
echo "  cd build/Assignment_2:3D_kinetic_sculpture_animation"
echo "  ./Assignment_2_3D_kinetic_sculpture_animation"
echo ""
echo "🎮 Controls:"
echo "  • WASD: Move camera"
echo "  • Mouse: Look around"
echo "  • Scroll: Zoom in/out"
echo "  • ESC: Exit application"
echo ""
echo "🎯 Press ESC in the window to exit"