# Lesson 1.1: Hello Window
add_executable(hello_window main.cpp)

# Link with common library
target_link_libraries(hello_window common)

# Set shader and resource root directories
set(SHADER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(RESOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/resources")

# Add compile definitions for shader and resource paths
target_compile_definitions(hello_window PRIVATE
    SHADER_ROOT="${SHADER_ROOT}"
    RESOURCE_ROOT="${RESOURCE_ROOT}"
)
