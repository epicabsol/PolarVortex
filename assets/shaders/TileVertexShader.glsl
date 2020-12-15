#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;

out vec2 PixelUV;

uniform mat4 Transform;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * Transform * vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);
    PixelUV = VertexUV;
}