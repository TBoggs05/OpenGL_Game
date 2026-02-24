#include "config.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "material.h"
#include "gameObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include <image_loading/stb_image.h>    //stb_image loader is open source image->pixel buffer converter (not enough time to write custom one for this project)


    //Time Variables
    float deltaTime = 0.0f; //Time between current frame and last frame
    float lastFrame = 0.0f; //Time of last Frame

    //Settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    //Camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    //function definitions
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow *window);
    GLFWwindow* initOpenGL();
    void updateDeltaTime();

int main()
{
    GLFWwindow* window = initOpenGL();
    if(window == nullptr){
        return -1;
    }

    //shaders
    Shader groundShader("shaders/groundShader.vs", "shaders/groundShader.fs");

    //meshes
    Mesh cubeMesh("ground");
    //materials
    Material cubeMaterial; cubeMaterial.shader = &groundShader;
    //objects/players
    GameObject ground(&cubeMaterial, &cubeMesh);
    ground.transform.position = {0, -1, 0};
        ground.transform.scale = {20, 0.1f, 20};
    //RENDER LOOP
    while(!glfwWindowShouldClose(window))
    {
        updateDeltaTime();
        processInput(window);

        // Clear at start of frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   //clear screen every frame
        // Setup shader and transformations
        ground.material->shader->use();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ground.material->shader->setMat4("projection", projection);
        ground.material->shader->setMat4("view", view);

        // Draw ground
        ground.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}

void updateDeltaTime(){
    //track deltaTime and lastFrame to weight frame rate
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
}

GLFWwindow* initOpenGL(){

//Initialize glfw and set GL version
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    //setting for MACOS compatability
    #endif
    
    //glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    //Initialize GLAD so we can call openGL functions AFTER intializing glfw and window object as current context
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    //pass GLAD a glfw function that gets OS specific GL function addresses.
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    } 
    //ENABLE DEPTH TESTING (z-buffer vals stored in fragment's z value)
    glEnable(GL_DEPTH_TEST);

    //hide cursor when its on screen, capture cursor instead.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetScrollCallback(window, scroll_callback); 
    glfwSetCursorPosCallback(window, mouse_callback);   //deal with cursor
    return window;
}  
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) //callback to resize window
{
    glViewport(0, 0, width, height);
} 

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
