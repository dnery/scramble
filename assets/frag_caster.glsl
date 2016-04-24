#version 330 core

out vec4 fs_pixcolor;

uniform vec3 caster_color;

void main() {

        fs_pixcolor = vec4(caster_color, 1.0f);
}
