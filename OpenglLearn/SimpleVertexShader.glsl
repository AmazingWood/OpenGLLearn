#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float vertexOffset;
uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(aPos.x + vertexOffset, aPos.y, aPos.z, 1.0);
    ourColor = vec3(gl_Position.xyz);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}