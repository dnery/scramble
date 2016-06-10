#ifndef SCRAMBLE_MODEL_H
#define SCRAMBLE_MODEL_H

#include "mesh.hh"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class model {

        std::string m_workpath;                 // A model's workpath must contain all of it's assets
        std::vector<mesh> m_meshes;             // A model is composed by several sub meshes
        std::vector<mesh::texture> m_texcache;  // A texture cache optimizes loading time

        void process_node(aiNode *ainode, const aiScene *aiscene);
        mesh process_mesh(aiMesh *aimesh, const aiScene *aiscene);

        std::vector<mesh::texture> load_mat_textures(aiMaterial *aimat,
                        aiTextureType aitype, std::string tname);

public:
        explicit model(const std::string&& path);


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

GLint texture_from_file(const char *path, std::string workpath);

#endif /* ifndef SCRAMBLE_MODEL_H */
