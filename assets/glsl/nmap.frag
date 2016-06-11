#version 330 core

// Input
in VS_OUT {
        vec2 uv;
        vec3 position;
        vec3 tg_position;
        vec3 tg_cam_position;
        vec3 tg_light_position;
} fs_in;

// Output
out vec4 frag_color;

// Material
uniform struct material_t {
        sampler2D texture_diffuse1;
        sampler2D texture_specular1;
        sampler2D texture_normal1;
};
uniform material_t material;

void main()
{
        // Textures
        vec3 normal_map = normalize(vec3(texture(material.texture_normal1, fs_in.uv)) * 2.0 - 1.0);
        vec3 diffuse_map = vec3(texture(material.texture_diffuse1, fs_in.uv));
        vec3 specular_map = vec3(texture(material.texture_specular1, fs_in.uv));

        // Directions
        vec3 cam_dir = normalize(fs_in.tg_cam_position - fs_in.tg_position);
        vec3 light_dir = normalize(fs_in.tg_light_position - fs_in.tg_position);
        vec3 reflect_dir = reflect(-light_dir, normal_map);
        vec3 halfway_dir = normalize(light_dir + cam_dir);

        // Ambient
        vec3 ambient = vec3(0.1) * diffuse_map;

        // Diffuse
        vec3 diffuse = max(dot(normal_map, light_dir), 0.0) * diffuse_map;

        // Specular
        vec3 specular = vec3(0.8) * pow(max(dot(halfway_dir, normal_map), 0.0), 16.0) * specular_map;

        frag_color = vec4(ambient + diffuse + specular, 1.0);
}
