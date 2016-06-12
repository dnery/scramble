#version 330 core
// profile: gp5fp

// Input
in VS_OUT {
        vec2 uvcoord;
        vec3 light_dir;
        vec3 viewer_dir;
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

void main()
{
        // Textures
        vec3 norm_map = texture(material.texture_normal1, fs_in.uvcoord).rgb;
        vec3 diff_map = texture(material.texture_diffuse1, fs_in.uvcoord).rgb;
        vec3 spec_map = texture(material.texture_specular1, fs_in.uvcoord).rgb;

        // Actual normal
        vec3 l_normal = (norm_map * 2.0 - 1.0);

        // Specular - PBR Specular energy conservation
        const float k_pi = 3.14159265;
        const float k_shininess = 16.0;
        const float k_energy_conservation = (8.0 + k_shininess) / (8.0 * k_pi);

        // Ambient
        vec3 ambient = 0.1 * diff_map;

        // Diffuse
        float lamb_factor = max(dot(l_normal, fs_in.light_dir), 0.0);
        vec3 diffuse = lamb_factor * diff_map;

        // Specular
        vec3 halfway_dir = normalize(fs_in.light_dir + fs_in.viewer_dir);
        float spec_factor = pow(max(dot(halfway_dir, l_normal), 0.0), k_shininess);
        vec3 specular = vec3(0.3) * k_energy_conservation * spec_factor * spec_map;

        // Final Color
        frag_color = vec4((ambient + diffuse + specular), 1.0);
}
