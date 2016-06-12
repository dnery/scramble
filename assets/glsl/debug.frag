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

        // Frag Normal
        vec3 frag_normal = normalize(norm_map * 2.0 - 1.0);

        // Directions
        vec3 reflect_dir = reflect(-fs_in.light_dir, frag_normal);
        vec3 halfway_dir = normalize(fs_in.light_dir + fs_in.viewer_dir);

        // Ambient
        vec3 ambient = 0.1 * diff_map;

        // Diffuse
        float lambert_factor = max(dot(frag_normal, fs_in.light_dir), 0.0);
        vec3 diffuse = 1.0 * lambert_factor * diff_map;

        // Specular
        float specular_factor = pow(max(dot(fs_in.viewer_dir, reflect_dir), 0.0), 16.0);
        vec3 specular = 1.0 * specular_factor * spec_map;

        // Final Color
        //frag_color = vec4((ambient + diffuse + specular) * 0.01 + fs_in.light_dir, 1.0);
        frag_color = vec4(fs_in.light_dir, 1.0);
        //frag_color = vec4((ambient + diffuse + specular), 1.0);
}
