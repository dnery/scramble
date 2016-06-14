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

// Material Structure
uniform struct material_t {
        sampler2D texture_normal1;
        sampler2D texture_diffuse1;
        sampler2D texture_specular1;
};

// Omnilight Structure
uniform struct omnilight_t {
        vec3 position;

        vec3 Ia;
        vec3 Id;
        vec3 Is;

        float Kc;
        float Kl;
        float Kq;
};

// Object
uniform material_t material;

// Viewer
uniform vec3 viewer_pos;

// Lights
uniform omnilight_t omnilight[N_LIGHTS];

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
                // Ambient
                ambient += omnilight[i].Ia * diff_map;

                // Diffuse
                vec3 light_dir = normalize(omnilight[i].position - fs_in.position);
                float lamb_factor = max(dot(fs_in.normal, light_dir), 0.0);
                diffuse += omnilight[i].Id * lamb_factor * diff_map;

                // Specular
                vec3 viewer_dir = normalize(viewer_pos - fs_in.position);
                vec3 halfway_dir = normalize(light_dir + viewer_dir);
                float spec_factor = pow(max(dot(halfway_dir, fs_in.normal), 0.0), k_shininess);
                specular += omnilight[i].Is * k_energy_conservation * spec_factor * spec_map;
        }

        // Final Color
        frag_color = vec4((ambient + diffuse + specular), 1.0);
}
