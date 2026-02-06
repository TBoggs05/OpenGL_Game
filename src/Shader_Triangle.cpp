#include "config.h"
#include <thread>
#include <chrono>

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) //callback to resize window
{
    glViewport(0, 0, width, height);
} 

int main()
{
    //error checking vars
    int  success;
    char infoLog[512];

    //Initialize glfw and set GL version
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    //setting for MACOS compatability
    #endif
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Initialize GLAD so we can call openGL functions AFTER intializing glfw and window object as current context
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    //pass GLAD a glfw function that gets OS specific GL function addresses.
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    //creating our shader objects
    Shader triangleShader("shaders/triangleShader.vs", "shaders/triangleShader.fs");
    Shader yellowTriangleShader("shaders/triangleShader.vs", "shaders/yellowTriangleShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float verticesOne[] = {
     0.75f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //left point
     -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  //peak
     0.25f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  //right point

    };
    
    float verticesTwo[] = {
      0.25f,  0.0f, 0.0f,  //left point
      0.5f, -0.5f, 0.0f,  //peak
      0.75f,  0.0f, 0.0f,  //right point
    };

    float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
    };
    
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOne), verticesOne, GL_STATIC_DRAW);
    //params go: location, location_size, type, normalization, stride_size, offset (0 since we start with position attribute, for color its non zero, and is size of this attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);   //6*(float_size=4) is our stride since we store 3 vertices w/ 6 points each
    glEnableVertexAttribArray(0);
    //Color attributes from shader
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //second triangle setup
    //---------------------
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTwo), verticesTwo, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
     //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //RENDER LOOP
    while(!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering commands (clear screen and set color). There's the color, depth, and stencil buffers. In this case, we reset the color buffer.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        
        // draw our first triangle
        triangleShader.use();
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0,3);
        

        
        yellowTriangleShader.use();
       // render the other triangle
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0,3);
 
        //check and call events and swap the buffers (glfw uses double buffers for windowing, where one is written to and one is displayed to avoid artifacts)
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    //UNNECESSARY SINCE WE USE OBJECTS THAT 
   // glDeleteProgram(shaderProgram);
   // glDeleteProgram(yellowShaderProgram);

    glfwTerminate();
    return 0;
}

     
