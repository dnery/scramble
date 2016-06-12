#version 330 core
// profile: gp5fp

#define N_LIGHTS 2

// Input
in VS_OUT {
        vec3 normal;
        vec2 uvcoord;
        vec3 position;
} fs_in;

// Output
out vec4 frag_color;

// Material
uniform struct material_t {
        sampler2D texture_normal1;
        sampler2D texture_diffuse1;
        sampler2D texture_specular1;
};
uniform material_t material;

// Uniform positions
uniform vec3 viewer_pos;
uniform vec3 light_pos[N_LIGHTS];
uniform vec3 light_col[N_LIGHTS];

void main()
{
        // Textures
        vec3 diff_map = texture(material.texture_diffuse1, fs_in.uvcoord).rgb;
        vec3 spec_map = texture(material.texture_specular1, fs_in.uvcoord).rgb;

        // Components
        vec3 ambient = vec3(0.0);
        vec3 diffuse = vec3(0.0);
        vec3 specular = vec3(0.0);

        // Specular - PBR Specular energy conservation
        const float k_pi = 3.14159265;
        const float k_shininess = 16.0;
        const float k_energy_conservation = (8.0 + k_shininess) / (8.0 * k_pi);

        for (int i = 0; i < N_LIGHTS; i++) {

                vec3 light_dir = normalize(light_pos[i] - fs_in.position);
                vec3 viewer_dir = normalize(viewer_pos - fs_in.position);

                // Ambient
                ambient += light_col[i] * 0.05 * diff_map;

                // Diffuse
                float lamb_factor = max(dot(fs_in.normal, light_dir), 0.0);
                diffuse += light_col[i] * lamb_factor * diff_map;

                // Specular
                vec3 halfway_dir = normalize(light_dir + viewer_dir);
                float spec_factor = pow(max(dot(halfway_dir, fs_in.normal), 0.0), k_shininess);
                specular += light_col[i] * k_energy_conservation * spec_factor * spec_map;
        }

        // Final Color
        frag_color = vec4((ambient + diffuse + specular), 1.0);
}
