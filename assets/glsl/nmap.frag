#version 330 core
// profile: gp5fp

#define N_OMNIS 4
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

// Spotlight Structure
uniform struct spotlight_t {
        vec3 position;
        vec3 front;

        vec3 Ia;
        vec3 Id;
        vec3 Is;

        float Kc;
        float Kl;
        float Kq;

        float mincutoff;
        float maxcutoff;
};

// Object
uniform material_t material;

// Viewer
uniform vec3 viewer_pos;

// Lights
uniform omnilight_t omnilight[N_OMNIS];
uniform spotlight_t spotlight[N_SPOTS];

float radatten(float Kc, float Kl, float Kq, float f_l_dist)
{
        return ((1.0) / (Kc + Kl * f_l_dist + Kq * (f_l_dist * f_l_dist)));
}

float angatten(float mincutoff, float maxcutoff, float f_l_angle)
{
        return clamp((f_l_angle - maxcutoff) / (mincutoff - maxcutoff), 0.0, 1.0);
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

        // Omnilights
        for (int i = 0; i < N_OMNIS; i++) {
                // Ambient
                vec3 a_c = omnilight[i].Ia * diff_map;

                // Diffuse
                vec3 light_dir = fs_in.TBN * normalize(omnilight[i].position - fs_in.position);
                float lamb_factor = max(dot(norm_map, light_dir), 0.0);
                vec3 d_c = omnilight[i].Id * lamb_factor * diff_map;

                // Specular
                vec3 viewer_dir = fs_in.TBN * normalize(viewer_pos - fs_in.position);
                vec3 halfway_dir = normalize(light_dir + viewer_dir);
                float spec_factor = pow(max(dot(halfway_dir, norm_map), 0.0), k_shininess);
                vec3 s_c = omnilight[i].Is * k_energy_conservation * spec_factor * spec_map;

                // Attenuation
                float radial = radatten(omnilight[i].Kc, omnilight[i].Kl, omnilight[i].Kq, length(omnilight[i].position - fs_in.position));
                ambient += (a_c * radial);
                diffuse += (d_c * radial);
                specular += (s_c * radial);
        }

        // Spotlights
        for (int i = 0; i < N_SPOTS; i++) {
                // Ambient
                vec3 a_c = spotlight[i].Ia * diff_map;

                // Diffuse
                vec3 light_dir = fs_in.TBN * normalize(spotlight[i].position - fs_in.position);
                float lamb_factor = max(dot(norm_map, light_dir), 0.0);
                vec3 d_c = spotlight[i].Id * lamb_factor * diff_map;

                // Specular
                vec3 viewer_dir = fs_in.TBN * normalize(viewer_pos - fs_in.position);
                vec3 halfway_dir = normalize(light_dir + viewer_dir);
                float spec_factor = pow(max(dot(halfway_dir, norm_map), 0.0), k_shininess);
                vec3 s_c = spotlight[i].Is * k_energy_conservation * spec_factor * spec_map;

                // Attenuation
                float radial = radatten(spotlight[i].Kc, spotlight[i].Kl, spotlight[i].Kq, length(spotlight[i].position - fs_in.position));
                float angular = angatten(spotlight[i].mincutoff, spotlight[i].maxcutoff, dot(light_dir, fs_in.TBN * normalize(-spotlight[i].front)));
                ambient += (a_c * angular * radial);
                diffuse += (d_c * angular * radial);
                specular += (s_c * angular * radial);
        }

        // Final Color
        frag_color = vec4((ambient + diffuse + specular), 1.0);
}
