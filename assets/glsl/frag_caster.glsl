#version 330 core

out vec4 fs_pixcolor;

uniform vec3 color;

void main() {

        fs_pixcolor = vec4(color, 1.0f);
}
