#version 330 core

in vec2 vs_texcoord;

out vec4 fs_pixcolor;

uniform float fluct;
uniform sampler2D fs_texture1;
uniform sampler2D fs_texture2;

void main()
{
        fs_pixcolor = mix(texture(fs_texture1, vs_texcoord),
                          texture(fs_texture2, vs_texcoord), 0.5);
}