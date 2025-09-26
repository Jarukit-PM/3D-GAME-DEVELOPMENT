#!/bin/bash

# MyOpenGLBook Build Script for macOS M1 Pro

set -e  # Exit on any error

echo "🎮 MyOpenGLBook Build Script"
echo "=============================="

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
echo "  • Hello Window:  ./1.getting_started/1.1_hello_window/1.1_hello_window"
echo "  • Hello Triangle: ./1.getting_started/1.2_hello_triangle/1.2_hello_triangle"
echo "  • Colors:        ./2.lighting/2.1_colors/2.1_colors"
echo ""
echo "💡 To run an example:"
echo "  cd build"
echo "  ./1.getting_started/1.1_hello_window/1.1_hello_window"
echo "  # or"
echo "  ./1.getting_started/1.2_hello_triangle/1.2_hello_triangle"
echo "  # or"
echo "  ./2.lighting/2.1_colors/2.1_colors"
echo ""
echo "🎯 Press ESC in the window to exit"
