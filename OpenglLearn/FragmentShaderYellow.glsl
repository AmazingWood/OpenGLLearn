#version 330 core
out vec4 FragColor;

in vec4 gl_Position;

void main()
{
    FragColor = gl_Position;
} 