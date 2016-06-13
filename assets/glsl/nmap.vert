#version 330 core
// profile: gp5vp

// Input
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvcoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

// Output
out VS_OUT {
        vec3 position;
        vec2 uvcoord;
        mat3 TBN;
} vs_out;

// Transform matrices
uniform mat4 normal_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main()
{
        vec3 N = normalize(vec3(normal_matrix * vec4(normal, 0.0)));
        vec3 T = normalize(vec3(normal_matrix * vec4(tangent, 0.0)));

        // This block works
        T = normalize(T - dot(T, N) * N);
        vec3 B = cross(T, N);

        // This block doesn't
        //vec3 B = normalize(vec3(normal_matrix * vec4(bitangent, 0.0)));
        //if (dot(cross(N, T), B) < 0.0) {
        //        T = T * -1.0;
        //}

        vs_out.position = vec3(model_matrix * vec4(position, 1.0));
        vs_out.uvcoord = uvcoord;
        vs_out.TBN = transpose(mat3(T, B, N));
        gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);
}
