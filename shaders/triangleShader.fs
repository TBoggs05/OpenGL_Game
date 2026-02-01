    #version 330 core
    in vec3 ourColor;
    out vec4 fragColor;

    //uniform vec4 ourColor;

    void main()
    {
       fragColor = vec4(ourColor, 1.0);
    }