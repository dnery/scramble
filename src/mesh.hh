#ifndef SCRAMBLE_MESH_H
#define SCRAMBLE_MESH_H

#include "program.hh"
#include <assimp/types.h>
#include <string>
#include <vector>

struct vertex {

        glm::vec3 m_vertex;
        glm::vec3 m_normal;
        glm::vec2 m_texcoord;
};

struct texture {

        GLuint m_id;
        aiString m_path;
        std::string m_type;
};

class mesh {

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ebo;

public:
        std::vector<texture> m_textures;
        std::vector<vertex> m_vertices;
        std::vector<GLuint> m_indices;

        mesh(std::vector<vertex>& vertices, std::vector<GLuint>& indices,
                        std::vector<texture>& textures);

        void draw(program& program) const;
};

#endif /* ifndef SCRAMBLE_MESH_H */
