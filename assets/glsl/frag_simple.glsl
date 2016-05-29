#version 330 core
// profile: gp5fp

// Preprocessor
#define N_POINTLIGHTS 3

// INPUTS
in vec2 vs_texcoord;
in vec3 vs_worldpos;
in vec3 vs_normal;

// OUTPUTS
out vec4 fs_pixcolor;

// STRUCTURES
struct Material {

        float shininess;
        sampler2D specular;
        sampler2D diffuse1;
        sampler2D diffuse2;
};

struct PointLight {

        vec3 position;

        float constant;
        float linear;
        float quad;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};

// UNIFORMS
uniform vec3 viewer_pos;                       // Unique viewer position
uniform Material material;                     // Unique object material
uniform PointLight pointlights[N_POINTLIGHTS]; // PointLight instances

/*
 * Calculates point light contribution to the current fragment.
 *
 * Calculations based on caster structure data, current fragment's position,
 * current fragment's normal vector and global viewer position vector (uniform).
 */
vec3 pointlight_contrib(PointLight caster, vec3 frag_pos, vec3 frag_normal)
{
        // Direction vectors
        vec3 caster_dir = normalize(caster.position - frag_pos);
        vec3 viewer_dir = normalize(viewer_pos - frag_pos);
        vec3 reflect_dir = reflect(-caster_dir, frag_normal);

        // Lighting maps
        vec3 diffuse_map = vec3(mix(texture(material.diffuse1, vs_texcoord),
                                    texture(material.diffuse2, vs_texcoord),
                                    0.35f));
        vec3 specular_map = vec3(texture(material.specular, vs_texcoord));

        // Specularity factor
        float specular_comp = pow(max(dot(viewer_dir, reflect_dir), 0.0f),
                                  material.shininess);

        // Ambient lighting
        vec3 ambient = caster.ambient * diffuse_map;

        // Diffuse lighting
        vec3 diffuse = caster.diffuse * diffuse_map;

        // Specular lighting
        vec3 specular = caster.specular * specular_comp * specular_map;

        /*
         *                          1.0             -> Kc = const term
         * Attenuation = -------------------------  -> Kl = linear term
         *               Kc + (Kl * d) + (Kq * d²)  -> Kq = quadratic term
         *                                          -> d  = caster-fragment distance
         *
         * Absolute values table from Ogre3D: http://bit.ly/1sTnrOJ
         *
         * Intensity as function of distance: http://bit.ly/1VmfQ67
         */
        float caster_dist = length(caster.position - frag_pos);
        float attenuation = 1.0f / (caster.constant +
                                    caster.linear * caster_dist +
                                    caster.quad * (caster_dist * caster_dist));

        // Apply attenuation
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        // Return summation of components
        return (ambient + diffuse + specular);
}

void main()
{
        // result R = summation of contributions
        vec3 R = vec3(0.0f);

        for (int i = 0; i < N_POINTLIGHTS; i++)
                R += pointlight_contrib(pointlights[i], vs_worldpos, vs_normal);

        // Final color
        fs_pixcolor = vec4(R, 1.0f);
}
