#version 330 core
// profile: gp5fp

// INPUTS
in vec2 vs_texcoord;
in vec3 vs_worldpos;
in vec3 vs_normal;

// OUTPUTS
out vec4 fs_pixcolor;

// UNIFORMS
struct Material {

	float shininess;
	sampler2D specular;
	sampler2D diffuse1;
	sampler2D diffuse2;
};

// instantiate
uniform Material material;

struct Caster {

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

// instantiate
uniform Caster caster;

uniform vec3 viewer_pos;

void main()
{
        // Ambient
        vec3 ambient = caster.ambient * vec3(mix(texture(material.diffuse1, vs_texcoord), texture(material.diffuse2, vs_texcoord), 0.35f));

        // Diffuse
        vec3 caster_dir = normalize(caster.position - vs_worldpos);
	vec3 diffuse_map = vec3(mix(texture(material.diffuse1, vs_texcoord), texture(material.diffuse2, vs_texcoord), 0.35f));
        vec3 diffuse = caster.diffuse * max(dot(caster_dir, vs_normal), 0.0f) * diffuse_map;
        
        // Specular
        vec3 viewer_dir = normalize(viewer_pos - vs_worldpos);
        vec3 reflect_dir = reflect(-caster_dir, vs_normal);
        vec3 specular = caster.specular * pow(max(dot(viewer_dir, reflect_dir), 0.0f), material.shininess) * vec3(texture(material.specular, vs_texcoord));

	// Final color
        fs_pixcolor = vec4(ambient + diffuse + specular, 1.0f);
}
