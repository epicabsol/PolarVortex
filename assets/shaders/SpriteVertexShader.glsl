#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;

out vec2 PixelUV;

void main()
{
    gl_Position = vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);
    PixelUV = VertexUV;
}