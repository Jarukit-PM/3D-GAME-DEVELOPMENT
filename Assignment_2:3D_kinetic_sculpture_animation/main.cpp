#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

#include "common.hpp"

// Window dimensions
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Kinetic sculpture parameters
float animationTime = 0.0f;
float rotationSpeed = 1.0f;
float pendulumAmplitude = 30.0f;
float pendulumFrequency = 2.0f;

// Function to load shader from file
std::string loadShaderFromFile(const std::string& filePath)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
        return "";
    }
}

// Process input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// Mouse callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;
    static bool firstMouse = true;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    static float yaw = -90.0f;
    static float pitch = 0.0f;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    const float fov = 45.0f;
    static float currentFov = fov;
    currentFov -= (float)yoffset;
    if (currentFov < 1.0f)
        currentFov = 1.0f;
    if (currentFov > 45.0f)
        currentFov = 45.0f;
}

// Create a cube with colors
std::vector<float> createColoredCube()
{
    return {
        // positions          // colors
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // red
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // green
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // blue
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // blue
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // yellow
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // red

        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // magenta
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // cyan
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // white
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // white
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // gray
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // magenta

        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // gray
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // yellow
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // red
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // red
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // magenta
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // gray

         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // white
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // blue
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // green
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // green
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // cyan
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // white

        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // red
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // green
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // cyan
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // cyan
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // magenta
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // red

        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // yellow
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // blue
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // white
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // white
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, // gray
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f  // yellow
    };
}

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 2: 3D Kinetic Sculpture Animation", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        int error;
        const char* description;
        error = glfwGetError(&description);
        if (error != GLFW_NO_ERROR) {
            std::cout << "GLFW Error: " << description << std::endl;
        }
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Load and compile shader program
    std::string vertexShaderSource = loadShaderFromFile("resources/vs/kinetic_sculpture.vs");
    std::string fragmentShaderSource = loadShaderFromFile("resources/fs/kinetic_sculpture.fs");
    
    if (vertexShaderSource.empty() || fragmentShaderSource.empty())
    {
        std::cout << "Failed to load shader files" << std::endl;
        return -1;
    }
    
    const char* vertexShaderCode = vertexShaderSource.c_str();
    const char* fragmentShaderCode = fragmentShaderSource.c_str();
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and buffers
    std::vector<float> cubeVertices = createColoredCube();

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), cubeVertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update animation time
        animationTime = currentFrame;

        // Input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        glUseProgram(shaderProgram);

        // Pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Render multiple cubes for kinetic sculpture
        glBindVertexArray(VAO);
        
        // Main rotating cube in center
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, animationTime * rotationSpeed, glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Pendulum cubes
        for (int i = 0; i < 4; ++i)
        {
            float angle = i * 90.0f; // 90 degrees apart
            float pendulumAngle = sin(animationTime * pendulumFrequency) * glm::radians(pendulumAmplitude);
            
            glm::mat4 pendulumModel = glm::mat4(1.0f);
            pendulumModel = glm::translate(pendulumModel, glm::vec3(
                cos(glm::radians(angle)) * 2.0f,
                sin(pendulumAngle) * 0.5f,
                sin(glm::radians(angle)) * 2.0f
            ));
            pendulumModel = glm::scale(pendulumModel, glm::vec3(0.5f, 0.5f, 0.5f));
            pendulumModel = glm::rotate(pendulumModel, animationTime * rotationSpeed * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
            
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(pendulumModel));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Orbiting cubes
        for (int i = 0; i < 6; ++i)
        {
            float angle = i * 60.0f + animationTime * rotationSpeed * 0.3f;
            float radius = 3.0f + sin(animationTime * 2.0f) * 0.5f;
            
            glm::mat4 orbitModel = glm::mat4(1.0f);
            orbitModel = glm::translate(orbitModel, glm::vec3(
                cos(glm::radians(angle)) * radius,
                sin(animationTime * 3.0f) * 0.3f,
                sin(glm::radians(angle)) * radius
            ));
            orbitModel = glm::scale(orbitModel, glm::vec3(0.3f, 0.3f, 0.3f));
            orbitModel = glm::rotate(orbitModel, animationTime * rotationSpeed * 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(orbitModel));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
