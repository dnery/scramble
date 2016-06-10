#ifndef SCRAMBLE_MESH_H
#define SCRAMBLE_MESH_H

#include "program.hh"
#include <assimp/types.h>
#include <string>
#include <vector>

class mesh {

        GLuint m_vao;                           // The vertex array aggregate
        GLuint m_vbo;                           // The vertex position buffer
        GLuint m_ebo;                           // The element indices buffer

        struct vertex {                         // A vertex is composed of:
                glm::vec3 m_position;           //      a position vector
                glm::vec3 m_normal;             //      a normal unit vector
                glm::vec2 m_texcoord;           //      a texture coordinate
        };

        struct texture {                        // A texture is composed of:
                GLuint m_id;                    //      it's OpenGL ID
                aiString m_path;                //      it's file path
                std::string m_type;             //      it's category
        };

        friend class model;                     // A model aggregates meshes

public:
        std::vector<vertex> m_vertices;         // A mesh aggregates vertices
        std::vector<GLuint> m_indices;          // A mesh aggregates face indices
        std::vector<texture> m_textures;        // A mesh aggregates various textures

        mesh(std::vector<vertex>& vertices,
                        std::vector<GLuint>& indices,
                        std::vector<texture>& textures);

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
        void draw(program& program) const;
};

#endif /* ifndef SCRAMBLE_MESH_H */
