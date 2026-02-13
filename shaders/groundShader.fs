#version 330 core
out vec4 FragColor;
  
uniform sampler2D texture;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
                //linearly interpolates between both textures based on the third arg (0.2 => 80% first input color 20% second.)
    FragColor = texture(texture, TexCoord);
}