#version 330 core

in vec2 PixelUV;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    //FragColor = vec4(0.9f, 0.5f, 0.2f, 1.0f);
    //FragColor = vec4(PixelUV.x, PixelUV.y, 0.0f, 1.0f);
    vec4 textureColor = texture(Texture, PixelUV);
    if (textureColor.a < 0.01f) {
        discard;
    }
    FragColor = textureColor;
}