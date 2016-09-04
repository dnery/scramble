#include "mesh.hh"

#include <sstream>

mesh::mesh(std::vector<vertex>& vertices,
                std::vector<GLuint>& indices,
                std::vector<texture>& textures) :
        m_vertices(vertices), m_indices(indices), m_textures(textures)
{
        // Generate
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        // Bind VertArray
        glBindVertexArray(m_vao);

        // Bind & config VertBuffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(mesh::vertex),
                        &m_vertices[0], GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::vertex),
                        (GLvoid *)offsetof(mesh::vertex, m_position));
        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::vertex),
                        (GLvoid *)offsetof(mesh::vertex, m_normal));
        // UVMap
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh::vertex),
                        (GLvoid *)offsetof(mesh::vertex, m_texcoord));

        // Tangent vector
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::vertex),
                        (GLvoid *)offsetof(mesh::vertex, m_tangent));

        // Bitangent vector
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::vertex),
                        (GLvoid *)offsetof(mesh::vertex, m_bitangent));

        // Bind && config ElementBuffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                        &m_indices[0], GL_STATIC_DRAW);

        // Reset
        glBindVertexArray(0);
}

void mesh::draw(program& program) const
{
        GLuint n_diff = 1;                              // "Which diffuse map?" counter
        GLuint n_spec = 1;                              // "Which specular map?" counter
        GLuint n_norm = 1;                              // "Which normal map?" counter

        for (GLuint i = 0; i < m_textures.size(); i++) {

                const texture& tex = m_textures[i];     // Texture reference
                std::stringstream ss;                   // Holds the number

                if (tex.m_type == "texture_diffuse")
                        ss << n_diff++;
                else if (tex.m_type == "texture_specular")
                        ss << n_spec++;
                else if (tex.m_type == "texture_normal")
                        ss << n_norm++;
                else    // Texture type is unknown
                        continue;

                // Get uniform name
                std::string which = "material." + tex.m_type + ss.str();

                // Set the uniform, if it exists
                if (glGetUniformLocation(program.get(), which.c_str()) != -1) {

                        glActiveTexture(GL_TEXTURE0 + i);
                        glBindTexture(GL_TEXTURE_2D, tex.m_id);
                        glUniform1i(program.uniform(which.c_str()), i);
                }
        }

        // Draw the mesh
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Unbind all
        for (GLuint i = 0; i < m_textures.size(); i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, 0);
        }
}
