#version 330 core
layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aColor;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float vertexOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position =  projection * view * model * vec4(aPos.x + vertexOffset, aPos.y, aPos.z, 1.0); 
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}