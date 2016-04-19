#version 330 core

in vec2 vs_texcoord;
in vec3 vs_worldpos;
in vec3 vs_normal;

out vec4 fs_pixcolor;

uniform vec3 caster;
uniform sampler2D fs_texture1;
uniform sampler2D fs_texture2;

void main()
{
        fs_pixcolor = mix(texture(fs_texture1, vs_texcoord),
                          texture(fs_texture2, vs_texcoord), 0.5);
}