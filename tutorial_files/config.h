#pragma once
#include <iostream>
#include <glad/glad.h> //must include glad before GLFW for compatability headache reasons
#include <GLFW/glfw3.h>
#include "shader.h" //custom shader class for easy handling of GLSL shaders

#define STB_IMAGE_IMPLEMENTATION
#include <image_loading/stb_image.h>    //stb_image loader is open source image->pixel buffer converter (not enough time to write custom one for this project)

//Header-Only Libraries to handle vector + matrix structures to save time.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
