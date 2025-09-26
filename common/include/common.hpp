#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

namespace Common {
    // Window management
    class Window {
    public:
        GLFWwindow* window;
        int width, height;
        
        Window(int width = 800, int height = 600, const char* title = "MyOpenGLBook");
        ~Window();
        
        bool shouldClose();
        void swapBuffers();
        void pollEvents();
        void processInput();
        
        // Callback functions
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    };
    
    // Shader utilities
    class Shader {
    public:
        unsigned int ID;
        
        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();
        
        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        
    private:
        void checkCompileErrors(unsigned int shader, const std::string &type);
    };
    
    // Utility functions
    std::string readFile(const std::string& path);
    void printOpenGLInfo();
}
