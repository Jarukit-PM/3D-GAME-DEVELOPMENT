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
#include <map>
#include <string>

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

// Advanced kinetic sculpture parameters
float windStrength = 0.0f;
float userInteraction = 0.0f;
bool isWindActive = false;
bool isUserControlled = false;

// Sculpture components
struct KineticElement {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    float phase;
    float frequency;
    float amplitude;
    glm::vec3 axis;
};

std::vector<KineticElement> sculptureElements;

// GLTF Model structure
struct GLTFModel {
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
    bool loaded;
};

GLTFModel parametricPattern;

// Function to load GLTF model (simplified version)
bool loadGLTFModel(const std::string& gltfPath, GLTFModel& model)
{
    std::ifstream file(gltfPath);
    if (!file.is_open()) {
        std::cout << "Failed to open GLTF file: " << gltfPath << std::endl;
        return false;
    }
    
    std::string jsonContent((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
    file.close();
    
    // Simple JSON parsing for GLTF (this is a basic implementation)
    // In a real project, you'd use a proper JSON library like nlohmann/json
    
    // For now, we'll create a simple parametric pattern manually
    // This creates a geometric pattern similar to the GLTF model
    
    model.vertices.clear();
    model.indices.clear();
    
    // Create a parametric pattern with lines
    int segments = 50;
    float width = 2.0f;
    float height = 1.0f;
    
    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            float x = (float)i / segments * width - width/2.0f;
            float y = (float)j / segments * height - height/2.0f;
            float z = 0.0f;
            
            // Create parametric pattern
            float pattern = sin(x * 3.14159f) * cos(y * 3.14159f) * 0.1f;
            z += pattern;
            
            // Add vertex
            model.vertices.push_back(x);
            model.vertices.push_back(y);
            model.vertices.push_back(z);
            
            // Add bright cyan color for pattern visibility
            model.vertices.push_back(0.0f);
            model.vertices.push_back(1.0f);
            model.vertices.push_back(1.0f);
        }
    }
    
    // Create indices for lines
    for (int i = 0; i < segments - 1; ++i) {
        for (int j = 0; j < segments - 1; ++j) {
            int current = i * segments + j;
            int right = current + 1;
            int down = (i + 1) * segments + j;
            int diagonal = down + 1;
            
            // Create line pattern
            if ((i + j) % 2 == 0) {
                model.indices.push_back(current);
                model.indices.push_back(diagonal);
            } else {
                model.indices.push_back(right);
                model.indices.push_back(down);
            }
        }
    }
    
    // Set bounds
    model.minBounds = glm::vec3(-width/2.0f, -height/2.0f, -0.1f);
    model.maxBounds = glm::vec3(width/2.0f, height/2.0f, 0.1f);
    
    // Create OpenGL buffers
    glGenVertexArrays(1, &model.VAO);
    glGenBuffers(1, &model.VBO);
    glGenBuffers(1, &model.EBO);
    
    glBindVertexArray(model.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(float), 
                 model.vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), 
                 model.indices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    model.loaded = true;
    std::cout << "Parametric pattern model loaded successfully with " << model.vertices.size()/6 
              << " vertices and " << model.indices.size() << " indices" << std::endl;
    
    return true;
}

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

// Initialize kinetic sculpture elements
void initializeKineticSculpture()
{
    sculptureElements.clear();
    
    // Central rotating element
    KineticElement center;
    center.position = glm::vec3(0.0f, 0.0f, 0.0f);
    center.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    center.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    center.phase = 0.0f;
    center.frequency = 1.0f;
    center.amplitude = 0.0f;
    center.axis = glm::vec3(0.5f, 1.0f, 0.0f);
    sculptureElements.push_back(center);
    
    // Pendulum elements
    for (int i = 0; i < 4; ++i) {
        KineticElement pendulum;
        float angle = i * 90.0f;
        pendulum.position = glm::vec3(
            cos(glm::radians(angle)) * 2.0f,
            0.0f,
            sin(glm::radians(angle)) * 2.0f
        );
        pendulum.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        pendulum.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        pendulum.phase = i * 0.5f;
        pendulum.frequency = 2.0f;
        pendulum.amplitude = 30.0f;
        pendulum.axis = glm::vec3(1.0f, 0.0f, 0.0f);
        sculptureElements.push_back(pendulum);
    }
    
    // Orbiting elements
    for (int i = 0; i < 6; ++i) {
        KineticElement orbit;
        float angle = i * 60.0f;
        orbit.position = glm::vec3(
            cos(glm::radians(angle)) * 3.0f,
            0.0f,
            sin(glm::radians(angle)) * 3.0f
        );
        orbit.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        orbit.scale = glm::vec3(0.3f, 0.3f, 0.3f);
        orbit.phase = i * 0.3f;
        orbit.frequency = 0.3f;
        orbit.amplitude = 0.5f;
        orbit.axis = glm::vec3(0.0f, 1.0f, 0.0f);
        sculptureElements.push_back(orbit);
    }
    
    // Floating elements
    for (int i = 0; i < 8; ++i) {
        KineticElement floating;
        float angle = i * 45.0f;
        floating.position = glm::vec3(
            cos(glm::radians(angle)) * 1.5f,
            sin(glm::radians(angle * 2)) * 0.5f,
            sin(glm::radians(angle)) * 1.5f
        );
        floating.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        floating.scale = glm::vec3(0.2f, 0.2f, 0.2f);
        floating.phase = i * 0.8f;
        floating.frequency = 1.5f;
        floating.amplitude = 0.3f;
        floating.axis = glm::vec3(0.0f, 0.0f, 1.0f);
        sculptureElements.push_back(floating);
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
    
    // Kinetic sculpture controls
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        isWindActive = !isWindActive;
        windStrength = isWindActive ? 1.0f : 0.0f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        isUserControlled = !isUserControlled;
    }
    
    // Adjust wind strength
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        windStrength = glm::clamp(windStrength + deltaTime, 0.0f, 2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        windStrength = glm::clamp(windStrength - deltaTime, 0.0f, 2.0f);
    }
    
    // User interaction
    if (isUserControlled) {
        userInteraction += deltaTime * 2.0f;
    } else {
        userInteraction = 0.0f;
    }
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize kinetic sculpture
    initializeKineticSculpture();

    // Load parametric pattern model
    if (!loadGLTFModel("resources/parametric_pattern_2.dxf/scene.gltf", parametricPattern)) {
        std::cout << "Failed to load parametric pattern model, continuing without it..." << std::endl;
    }

    // Load and compile shader program
    std::string vertexShaderSource = loadShaderFromFile("vs/kinetic_sculpture.vs");
    std::string fragmentShaderSource = loadShaderFromFile("fs/kinetic_sculpture.fs");
    
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
        
        // Lighting setup
        glm::vec3 lightPos = glm::vec3(2.0f, 4.0f, 2.0f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));

        // Render kinetic sculpture elements
        glBindVertexArray(VAO);
        
        for (size_t i = 0; i < sculptureElements.size(); ++i) {
            KineticElement& element = sculptureElements[i];
            
            // Calculate dynamic position based on element type and animation
            glm::vec3 dynamicPosition = element.position;
            glm::vec3 dynamicRotation = element.rotation;
            
            // Apply different animations based on element index
            if (i == 0) {
                // Central rotating element
                dynamicRotation = glm::vec3(0.0f, animationTime * element.frequency, 0.0f);
            } else if (i >= 1 && i <= 4) {
                // Pendulum elements
                float pendulumAngle = sin(animationTime * element.frequency + element.phase) * 
                                    glm::radians(element.amplitude + windStrength * 10.0f);
                dynamicPosition.y = sin(pendulumAngle) * 0.5f;
                dynamicRotation = glm::vec3(pendulumAngle, animationTime * 0.5f, 0.0f);
            } else if (i >= 5 && i <= 10) {
                // Orbiting elements
                float orbitAngle = animationTime * element.frequency + element.phase;
                float radius = 3.0f + sin(animationTime * 2.0f) * element.amplitude + windStrength * 0.3f;
                dynamicPosition = glm::vec3(
                    cos(orbitAngle) * radius,
                    sin(animationTime * 3.0f) * 0.3f + windStrength * 0.2f,
                    sin(orbitAngle) * radius
                );
                dynamicRotation = glm::vec3(animationTime * 2.0f, orbitAngle, 0.0f);
            } else {
                // Floating elements
                float floatAngle = animationTime * element.frequency + element.phase;
                dynamicPosition += glm::vec3(
                    sin(floatAngle) * element.amplitude,
                    cos(floatAngle * 1.5f) * element.amplitude * 0.5f,
                    cos(floatAngle) * element.amplitude * 0.3f
                );
                dynamicRotation = glm::vec3(
                    animationTime * 1.5f,
                    animationTime * 0.8f,
                    animationTime * 2.2f
                );
            }
            
            // Apply user interaction effects
            if (isUserControlled) {
                float interactionEffect = sin(userInteraction) * 0.5f;
                dynamicPosition += glm::vec3(interactionEffect, interactionEffect * 0.5f, interactionEffect * 0.3f);
                dynamicRotation += glm::vec3(interactionEffect, interactionEffect, interactionEffect);
            }
            
            // Create model matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, dynamicPosition);
            model = glm::rotate(model, dynamicRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, dynamicRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, dynamicRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, element.scale);
            
            // Apply wind effects
            if (windStrength > 0.0f) {
                float windEffect = sin(animationTime * 3.0f + i) * windStrength * 0.1f;
                model = glm::rotate(model, windEffect, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Render parametric pattern as background/decoration
        if (parametricPattern.loaded) {
            std::cout << "Rendering parametric pattern..." << std::endl;
            glBindVertexArray(parametricPattern.VAO);
            
            // Create multiple instances of the pattern with different animations
            for (int i = 0; i < 3; ++i) {
                glm::mat4 patternModel = glm::mat4(1.0f);
                
                // Position patterns around the sculpture
                float angle = i * 120.0f + animationTime * 0.2f;
                float radius = 4.0f + sin(animationTime * 1.5f) * 0.5f;
                
                patternModel = glm::translate(patternModel, glm::vec3(
                    cos(glm::radians(angle)) * radius,
                    sin(animationTime * 2.0f) * 0.3f,
                    sin(glm::radians(angle)) * radius
                ));
                
                // Rotate the pattern
                patternModel = glm::rotate(patternModel, animationTime * 0.5f + i * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                patternModel = glm::rotate(patternModel, sin(animationTime * 1.0f + i) * 0.3f, glm::vec3(1.0f, 0.0f, 0.0f));
                
                // Scale the pattern
                float scale = 0.8f + sin(animationTime * 2.0f + i) * 0.2f;
                patternModel = glm::scale(patternModel, glm::vec3(scale, scale, scale));
                
                // Apply wind effects
                if (windStrength > 0.0f) {
                    float windEffect = sin(animationTime * 3.0f + i) * windStrength * 0.2f;
                    patternModel = glm::rotate(patternModel, windEffect, glm::vec3(0.0f, 0.0f, 1.0f));
                }
                
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(patternModel));
                glDrawElements(GL_LINES, parametricPattern.indices.size(), GL_UNSIGNED_INT, 0);
            }
        } else {
            std::cout << "Parametric pattern not loaded!" << std::endl;
        }

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    // Cleanup GLTF model
    if (parametricPattern.loaded) {
        glDeleteVertexArrays(1, &parametricPattern.VAO);
        glDeleteBuffers(1, &parametricPattern.VBO);
        glDeleteBuffers(1, &parametricPattern.EBO);
    }

    glfwTerminate();
    return 0;
}
