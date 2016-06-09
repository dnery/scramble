#ifndef SCRAMBLE_MODEL_H
#define SCRAMBLE_MODEL_H

#include "mesh.hh"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class model {

        std::string m_workpath;
        std::vector<mesh> m_meshes;
        std::vector<texture> m_texcache;

        void process_node(aiNode *ainode, const aiScene *aiscene);
        mesh process_mesh(aiMesh *aimesh, const aiScene *aiscene);

        std::vector<texture> load_mat_textures(aiMaterial *aimat,
                        aiTextureType aitype, std::string tname);

public:
        explicit model(const std::string&& path);

        void draw(program& program) const;
};

GLint texture_from_file(const char *path, std::string workpath);

#endif /* ifndef SCRAMBLE_MODEL_H */
