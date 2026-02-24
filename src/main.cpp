#include "config.h"
#include "camera.h"
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <image_loading/stb_image.h>    //stb_image loader is open source image->pixel buffer converter (not enough time to write custom one for this project)

    //Time Variables
    float deltaTime = 0.0f; //Time between current frame and last frame
    float lastFrame = 0.0f; //Time of last Frame

    //Settings
    const unsigned int SCR_WIDTH = 1000;
    const unsigned int SCR_HEIGHT = 700;

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

    //positions of the player cubes
    glm::vec3 cubePositions[] = {
    glm::vec3( -1.0f,  0.0f,  -0.45f), //left cube
    glm::vec3( 1.0f,  0.0f,  -0.45f),  //right cube
    };

    glm::vec3 cubeRotation[2] = {
    glm::vec3(0.0f), // left cube
    glm::vec3(0.0f)  // right cube
    };
    glm::vec3 faceNormals[6] = {
        { 0.0f,  0.0f,  1.0f}, // front
        { 0.0f,  0.0f, -1.0f}, // back
        { 1.0f,  0.0f,  0.0f}, // right
        {-1.0f,  0.0f,  0.0f}, // left
        { 0.0f,  1.0f,  0.0f}, // top
        { 0.0f, -1.0f,  0.0f}  // bottom
    };
    int getFrontFace(const glm::mat4 &modelMatrix, const glm::vec3 &cubePos, const glm::vec3 &camPos)
    {
        float maxDot = -1.0f;
        int frontFace = -1;

        glm::vec3 cameraDir = glm::normalize(camPos - cubePos);

        for(int f = 0; f < 6; f++){
            glm::vec3 worldNormal = glm::vec3(modelMatrix * glm::vec4(faceNormals[f], 0.0f));
            float dot = glm::dot(worldNormal, cameraDir);
            if(dot > maxDot){
                maxDot = dot;
                frontFace = f;
            }
        }
        return frontFace; // index 0-5 maps our colors
    }
    std::string colors[6] = {"green","red","yellow","blue","magenta","cyan"};
    std::string colorCodes[6] = {"\033[32m","\033[31m","\033[33m","\033[34m","\033[35m","\033[36m",};
    int selectRandomColor(){    //returns a number between 0 and 5
        int i = rand() % 6;
        std::cout << "New Color is: "<< colorCodes[i] << colors[i] << "\033[0m" << std::endl;
        return i;
    }
    int currColor; //will be randomly selected every 3 seconds
    float resetTime = 3.0f; //time to reset to
    float timeRemaining = resetTime; //timer
    float lastPrintedTime = timeRemaining;
    int playerOneLives = 2;
    int playerTwoLives = 2;
    bool checkWinners(){

        return false;
    }
    void handleOutcome(int oneChoice, int twoChoice, int truth){
        if(oneChoice != truth){
                playerOneLives--; 
                std::cout << "\033[31m" << "Player One Lives: " << playerOneLives << "\033[0m" << std::endl;
        }
        if(twoChoice != truth){
                playerTwoLives--;
                std::cout << "\033[31m" << "Player Two Lives: " << playerTwoLives << "\033[0m" << std::endl;
        }
        if(playerOneLives < 1 && playerTwoLives < 1){
                std::cout << "\033[31m" << "GAME OVER! " << "\033[33m" << "PLAYERS TIE" <<"\033[0m" << std::endl;
        }
        else if(playerOneLives < 1){
                std::cout << "\033[31m" << "GAME OVER! " << "\033[32m" << "PLAYER TWO WINS!" <<"\033[0m" << std::endl;
        }
        else if(playerTwoLives < 1){
                std::cout << "\033[31m" << "GAME OVER! " << "\033[32m" << "PLAYER ONE WINS!" <<"\033[0m" << std::endl;
        }

    }
int main()
{
    //error checking vars
    int  success;
    char infoLog[512];
     // Seed with current time
    srand(time(NULL)); 
    GLFWwindow* window = initOpenGL();
    if(window == nullptr){
        return -1;
    }

    //creating our shader objects
    Shader triangleShader("shaders/triangleShader.vs", "shaders/triangleShader.fs");
    Shader yellowTriangleShader("shaders/triangleShader.vs", "shaders/yellowTriangleShader.fs");

    // --- Vertex Data ---
// Each face has a solid color (r,g,b)
float vertices[] = {
    // positions         // colors
    // Back face (red)
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 

    // Front face (green)
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

    // Left face (blue)
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

    // Right face (yellow)
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

    // Bottom face (cyan)
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

    // Top face (magenta)
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f
};
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };


    // --- VAO/VBO setup ---
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
 
    int width, height, nrChannels;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    unsigned char  *data = stbi_load("textures/paleobo_certified.gif", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    triangleShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    //glUniform1i(glGetUniformLocation(triangleShader.ID, "texture1"), 0);
    // or set it via the texture class
    triangleShader.setInt("texture2", 1);
    
    

    // uncomment this call to draw in wireframe polygons.
     //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    currColor = selectRandomColor(); //select our first color for the game!
    //RENDER LOOP
    while(!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering commands (clear screen and set color). There's the color, depth, and stencil buffers. In this case, we reset the color buffer.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glfwSetCursorPosCallback(window, mouse_callback);  
        //uses bitmap macros + bitwise or to pass a binary string s.t. all values to be cleared this frame are set to 1.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //track deltaTime and lastFrame to weight frame rate
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame; 
        triangleShader.use();


        //MODEL, VIEW, AND PROJECTION MATRICES
       // glm::mat4 model = glm::mat4(1.0f);
       // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f)); 


       // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        triangleShader.setMat4("view", view);

        int viewLoc = glGetUniformLocation(triangleShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f); 

        

        int projectionLoc = glGetUniformLocation(triangleShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
          // bind textures on corresponding texture units
           // glActiveTexture(GL_TEXTURE0);
          //  glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            

            //create transformation
             glm::mat4 trans = glm::mat4(1.0f);
             trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
             trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            //pass the transformation uniform to the vertex shader
            unsigned int transformLoc = glGetUniformLocation(triangleShader.ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

            glBindVertexArray(VAO);
            //uncomment to draw single cube.
            //glDrawArrays(GL_TRIANGLES, 0, 36);
            
            //random rotation code
            int faces[2];
            for(unsigned int i = 0; i < 2; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]); // fixed position

                // Apply rotation based on cubeRotation array
                model = glm::rotate(model, cubeRotation[i].x, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
                model = glm::rotate(model, cubeRotation[i].y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
                model = glm::rotate(model, cubeRotation[i].z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis if needed

                int modelLoc = glGetUniformLocation(triangleShader.ID, "model");
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                glDrawArrays(GL_TRIANGLES, 0, 36);
                faces[i] = getFrontFace(model, cubePositions[i], camera.Position);
                //std::cout<<faces[i] << std::endl;
            }
            timeRemaining -= deltaTime;
            if(floor(timeRemaining) <= lastPrintedTime-1){
                lastPrintedTime = floor(timeRemaining);
                std::cout << "time remaining: " << lastPrintedTime+1 << std::endl;
            }
            if(timeRemaining <= 0){
                std::cout << "faces[0]: " << faces[0] << " faces[1]: " << faces[1] << " currColor: " << currColor << std::endl;
                if(faces[0] == currColor && faces[1] == currColor){ //if theres no winner, continue
                    lastPrintedTime = timeRemaining = resetTime;
                    currColor = selectRandomColor();
                }
                else{
                    handleOutcome(faces[0], faces[1], currColor);
                    if(playerOneLives > 0 && playerTwoLives > 0){
                        lastPrintedTime = timeRemaining = resetTime;
                        currColor = selectRandomColor();
                    }
                    else{
                        break;
                    }
                }
            }
        //check and call events and swap the buffers (glfw uses double buffers for windowing, where one is written to and one is displayed to avoid artifacts)
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
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
    //glfwSetScrollCallback(window, scroll_callback); 
    return window;
}  
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float rotationSpeed = 3 * deltaTime; // radians per second

    // Left cube (WASD)
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cubeRotation[0].x -= rotationSpeed; // rotate up
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cubeRotation[0].x += rotationSpeed; // rotate down
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cubeRotation[0].y -= rotationSpeed; // rotate left
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cubeRotation[0].y += rotationSpeed; // rotate right

    // Right cube (IJKL)
    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        cubeRotation[1].x -= rotationSpeed; // rotate up
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        cubeRotation[1].x += rotationSpeed; // rotate down
    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        cubeRotation[1].y -= rotationSpeed; // rotate left
    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        cubeRotation[1].y += rotationSpeed; // rotate right

    /*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){}
    //    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){}
    //    camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){}
     //   camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){}
     //   camera.ProcessKeyboard(RIGHT, deltaTime);
     */
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) //callback to resize window
{
    glViewport(0, 0, width, height);
} 
//MOUSE FUNCTIONS NOT NEEDED SINCE WE CHANGED GAME IDEA
/* 
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
}*/
