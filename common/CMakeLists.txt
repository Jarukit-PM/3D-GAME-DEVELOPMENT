# Common library CMakeLists.txt
add_library(common STATIC
    src/common.cpp
)

target_include_directories(common PUBLIC
    include
    ${glad_SOURCE_DIR}/include
    ${glad_SOURCE_DIR}/src
    ${glm_SOURCE_DIR}
)

# Link with GLFW and GLAD
target_link_libraries(common PUBLIC glfw glad)

# macOS specific linking
if(APPLE)
    target_link_libraries(common PUBLIC
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
