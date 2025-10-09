#include <common.hpp>

int main() {
    // Create window
    Common::Window window(800, 600, "Hello Window - MyOpenGLBook");
    
    if (!window.window) {
        return -1;
    }
    
    // Render loop
    while (!window.shouldClose()) {
        // Input
        window.processInput();
        
        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Swap buffers and poll events
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}
