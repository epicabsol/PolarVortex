#version 330 core

in vec2 PixelUV;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    vec4 textureColor = texture(Texture, PixelUV);
    if (textureColor.a < 0.01f) {
        discard;
    }
    FragColor = textureColor;
}