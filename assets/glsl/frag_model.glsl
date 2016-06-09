#version 330 core

in vec2 TexCoords;

out vec4 color;

struct Material {
        float shininess;
        sampler2D texture_diffuse1;
        sampler2D texture_specular1;
};

uniform Material material;

void main()
{
    color = vec4(texture(material.texture_diffuse1, TexCoords));
}
