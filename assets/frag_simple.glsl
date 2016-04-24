#version 330 core

in vec2 vs_texcoord;
in vec3 vs_worldpos;
in vec3 vs_normal;

out vec4 fs_pixcolor;

uniform vec3 viewer_pos;
uniform vec3 caster_pos;
uniform vec3 caster_color;
uniform sampler2D fs_texture1;
uniform sampler2D fs_texture2;

void main()
{
        // Ambient
        vec3 ambient = caster_color * 0.1f;
        /*                            ^~~~
         *                            ambient intensity
         */

        // Diffuse
        vec3 caster_dir = normalize(caster_pos - vs_worldpos);
        vec3 diffuse = max(dot(caster_dir, vs_normal), 0.0f) * caster_color;
        
        // Specular
        vec3 viewer_dir = normalize(viewer_pos - vs_worldpos);
        vec3 reflect_dir = reflect(-caster_dir, vs_normal);
        vec3 specular = pow(max(dot(viewer_dir, reflect_dir), 0.0f), 64) * 0.5f * caster_color;
        /*                                                           ^~    ^~~~
         *                                                           |     specular intensity
         *                                                           specular component
         */

        // Finale
        vec3 result = ambient + diffuse + specular;

        fs_pixcolor = mix(texture(fs_texture1, vs_texcoord),
                          texture(fs_texture2, vs_texcoord), 0.5) *
                      vec4(result, 1.0f);
}