#include "mesh.hh"

#include <sstream>
#include "debug.hh"

mesh::mesh(std::vector<vertex>& vertices, std::vector<GLuint>& indices,
                std::vector<texture>& textures) :
        m_textures(textures),
        m_vertices(vertices),
        m_indices(indices)
{
        // Generate
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        // Bind VertArray
        glBindVertexArray(m_vao);

        // Bind & config VertBuffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex),
                        &m_vertices[0], GL_STATIC_DRAW);

        // Bind && config ElementBuffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                        &m_indices[0], GL_STATIC_DRAW);

        // VertPosition
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (GLvoid *)0);
        // VertNormal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (GLvoid *)offsetof(vertex, m_normal));
        // VertTex
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (GLvoid *)offsetof(vertex, m_texcoord));

        // Reset
        glBindVertexArray(0);
}

/*
 * Material implementations in the shader MUST follow this naming convention:
 *
 *      ...
 *      struct _material {
 *              double shininess;               // Required, discrete power of 2
 *              sampler2D texture_diffuse<n>;   // Required, n is a discrete number
 *              sampler2D texture_specular<n>;  // Required, n is a discrete number
 *              ...
 *      };
 *
 *      unform _material material;              // Has to be declared as "material"
 *      ...
 */
void mesh::draw(program& program) const
{
        GLuint n_diff = 1;                              // "Which diffuse map?" counter
        GLuint n_spec = 1;                              // "Which specular map?" counter

        for (GLuint i = 0; i < m_textures.size(); i++) {

                glActiveTexture(GL_TEXTURE0 + i);

                const texture& tex = m_textures[i];     // Texture reference
                std::stringstream ss;                   // Holds the number

                if (tex.m_type == "texture_diffuse")
                        ss << n_diff++;
                else if (tex.m_type == "texture_specular")
                        ss << n_spec++;

                // Get uniform name
                std::string which = "material." + tex.m_type + ss.str();

                // Set uniform
                program.setUniform(which.c_str(), i);

                // Bind
                glBindTexture(GL_TEXTURE_2D, tex.m_id);
        }

        // Draw the mesh
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Unbind all
        for (GLuint i = 0; i < m_textures.size(); i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, 0);
        }
}
