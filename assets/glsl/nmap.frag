#version 330 core
// profile: gp5fp

#define N_OMNIS 2
#define N_SPOTS 1

// Input
in VS_OUT {
        vec3 position;
        vec2 uvcoord;
        mat3 TBN;
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
uniform omnilight_t omnilight[N_OMNIS];

float radatten(omnilight_t light, float f_v_dist)
{
        return ((1.0) / (light.Kc + light.Kl * f_v_dist + light.Kq * (f_v_dist * f_v_dist)));
}

void main()
{

        // Textures
        vec3 diff_map = texture(material.texture_diffuse1, fs_in.uvcoord).rgb;
        vec3 spec_map = texture(material.texture_specular1, fs_in.uvcoord).rgb;
        vec3 norm_map = normalize(texture(material.texture_normal1, fs_in.uvcoord).rgb * 2.0 - 1.0);

        // Components
        vec3 ambient = vec3(0.0);
        vec3 diffuse = vec3(0.0);
        vec3 specular = vec3(0.0);

        // Specular - PBR Specular energy conservation
        const float k_pi = 3.14159265;
        const float k_shininess = 16.0;
        const float k_energy_conservation = (8.0 + k_shininess) / (8.0 * k_pi);

        for (int i = 0; i < N_OMNIS; i++) {
                // Directions
                vec3 light_dir = fs_in.TBN * normalize(omnilight[i].position - fs_in.position);
                vec3 viewer_dir = fs_in.TBN * normalize(viewer_pos - fs_in.position);

                // Ambient
                vec3 a_c = omnilight[i].Ia * diff_map;

                // Diffuse
                float lamb_factor = max(dot(norm_map, light_dir), 0.0);
                vec3 d_c = omnilight[i].Id * lamb_factor * diff_map;

                // Specular
                vec3 halfway_dir = normalize(light_dir + viewer_dir);
                float spec_factor = pow(max(dot(halfway_dir, norm_map), 0.0), k_shininess);
                vec3 s_c = omnilight[i].Is * k_energy_conservation * spec_factor * spec_map;

                // Attenuation
                float radial = radatten(omnilight[i], length(omnilight[i].position - fs_in.position));
                ambient += (a_c * radial);
                diffuse += (d_c * radial);
                specular += (s_c * radial);
        }

        // Final Color
        frag_color = vec4((ambient + diffuse + specular), 1.0);
}
