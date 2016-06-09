#version 330 core

in vec2 TexCoords;

out vec4 color;

struct Material {
        sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{
    color = vec4(texture(material.texture_diffuse1, TexCoords));
}
