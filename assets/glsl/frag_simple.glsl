#version 330 core
// profile: gp5fp

// Preprocessor
#define N_SPOTLIGHTS  1
#define N_POINTLIGHTS 2

// INPUTS
in vec2 vs_texcoord;
in vec3 vs_worldpos;
in vec3 vs_normal;

// OUTPUTS
out vec4 fs_pixcolor;

// STRUCTURES
struct Material {

        float shininess;
        sampler2D texture_diffuse1;
        sampler2D texture_specular1;
};

struct SpotLight {

        vec3 position;
        vec3 direction;

        float mincutoff;
        float maxcutoff;

        /*
         * These are not being used because the flashlight is not being
         * attenuated. It is in our best interest, though, that it _is_
         * for the final build. An ever-reaching flashlight is probably
         * too good to be true.
         */
        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};

struct PointLight {

        vec3 position;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};

// UNIFORMS
uniform vec3 viewer_pos;                       // Unique viewer position
uniform Material material;                     // Unique object material
uniform SpotLight spotlights[N_SPOTLIGHTS];    // SpotLight instances
uniform PointLight pointlights[N_POINTLIGHTS]; // PointLight instances

/*
 * Calculates spot light contribution to the current fragment.
 *
 * Calculations based on caster structure data, current fragment's position,
 * current fragment's normal vector and global viewer position vector (uniform).
 */
vec3 spotlight_contrib(SpotLight caster, vec3 frag_pos, vec3 frag_normal)
{
        // Direction vectors
        vec3 caster_dir = normalize(caster.position - frag_pos);
        vec3 viewer_dir = normalize(viewer_pos - frag_pos);
        vec3 reflect_dir = reflect(-caster_dir, frag_normal);

        // Lighting maps
        vec3 diffuse_map = vec3(texture(material.texture_diffuse1, vs_texcoord));
        vec3 specular_map = vec3(texture(material.texture_specular1, vs_texcoord));

        // Lighting factors
        float diffuse_comp = max(dot(frag_normal, caster_dir), 0.0f);
        float specular_comp = pow(max(dot(viewer_dir, reflect_dir), 0.0f),
                                  material.shininess);

        // Ambient lighting
        vec3 ambient = caster.ambient * diffuse_map;

        // Diffuse lighting
        vec3 diffuse = caster.diffuse * diffuse_comp * diffuse_map;

        // Specular lighting
        vec3 specular = caster.specular * specular_comp * specular_map;

        /*
         *                 theta - gamma      -> theta = cosine fragment angle to normal
         * Intensity = ---------------------  -> gamma = cosine outer cutoff angle
         *             epsilon = phi - gamma  -> phi = cosine inner cutoff angle
         *
         * The differences seem inverted because they are actually cosines. In
         * a range from 0 to 180 degrees, the cosine of a smaller angle is
         * greater than the cosine of a bigger angle. As a spotlight that
         * expands greater than 90⁰ makes no sense, the condition applies.
         *
         * Schematic visualization: http://bit.ly/1Z7CMox
         */
        float theta = dot(caster_dir, normalize(-caster.direction));             // caster_dir is the vector source->fragment, while caster.direction is the light normal
        float epsilon = caster.mincutoff - caster.maxcutoff;                     // Epsilon is a simple difference between cosines calculation
        float intensity = clamp((theta - caster.maxcutoff) / epsilon, 0.0, 1.0); // Values are clamped between 0.0 and 1.0

        // Apply intensity
        ambient *= intensity;
        diffuse *= intensity;
        specular *= intensity;

        // Return summation of components
        return (ambient + diffuse + specular);
}

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
        vec3 diffuse_map = vec3(texture(material.texture_diffuse1, vs_texcoord));
        vec3 specular_map = vec3(texture(material.texture_specular1, vs_texcoord));

        // Lighting factors
        float diffuse_comp = max(dot(frag_normal, caster_dir), 0.0f);
        float specular_comp = pow(max(dot(viewer_dir, reflect_dir), 0.0f),
                                  material.shininess);

        // Ambient lighting
        vec3 ambient = caster.ambient * diffuse_map;

        // Diffuse lighting
        vec3 diffuse = caster.diffuse * diffuse_comp * diffuse_map;

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
                                    caster.quadratic * (caster_dist * caster_dist));

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

        for (int i = 0; i < N_SPOTLIGHTS; i++)
                R += spotlight_contrib(spotlights[i], vs_worldpos, vs_normal);

        for (int i = 0; i < N_POINTLIGHTS; i++)
                R += pointlight_contrib(pointlights[i], vs_worldpos, vs_normal);

        // Final color
        fs_pixcolor = vec4(R, 1.0f);
}
