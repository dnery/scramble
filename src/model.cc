#include "model.hh"

#include <SOIL/SOIL.h>
#include <stdexcept>
#include "debug.hh"

model::model(const std::string&& path)
{
        Assimp::Importer imp;

        // Postprocess: triangulate mesh, flip UV maps
        const aiScene *aiscene = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        // Check for errors
        if (!aiscene || !aiscene->mRootNode ||
                        aiscene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
                throw std::runtime_error(imp.GetErrorString());

        // Get workdir
        m_workpath = path.substr(0, path.find_last_of('/'));

        // Procceed
        process_node(aiscene->mRootNode, aiscene);
}

void model::process_node(aiNode *ainode, const aiScene *aiscene)
{
        for (GLuint i = 0; i < ainode->mNumMeshes; i++){
                aiMesh *mesh = aiscene->mMeshes[ainode->mMeshes[i]];
                m_meshes.push_back(process_mesh(mesh, aiscene));
        }

        for (GLuint i = 0; i < ainode->mNumChildren; i++)
                process_node(ainode->mChildren[i], aiscene);
}

mesh model::process_mesh(aiMesh *aimesh, const aiScene *aiscene)
{
        std::vector<vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<texture> textures;

        // Retrieve vertex data
        for (GLuint i = 0; i < aimesh->mNumVertices; i++) {
                vertex vert;

                glm::vec3 vector;
                vector.x = aimesh->mVertices[i].x;
                vector.y = aimesh->mVertices[i].y;
                vector.z = aimesh->mVertices[i].z;
                vert.m_vertex = vector;

                // AQUI JEAN!
                glm::vec3 normal;
                normal.x = aimesh->mNormals[i].x;
                normal.y = aimesh->mNormals[i].y;
                normal.z = aimesh->mNormals[i].z;
                vert.m_normal = normal;

                if (aimesh->mTextureCoords[0]) {
                      glm::vec2 tex;
                      tex.x = aimesh->mTextureCoords[0][i].x;
                      tex.y = aimesh->mTextureCoords[0][i].y;
                      vert.m_texcoord = tex;
                }

                vertices.push_back(vert);
        }

        // Process indices
        for (GLuint i = 0; i < aimesh->mNumFaces; i++) {
                aiFace face = aimesh->mFaces[i];

                for (GLuint j = 0; j < face.mNumIndices; j++)
                        indices.push_back(face.mIndices[j]);
        }

        // Process material
        if (aimesh->mMaterialIndex) {
                aiMaterial *mat = aiscene->mMaterials[aimesh->mMaterialIndex];

                std::vector<texture> diffs = load_mat_textures(mat,
                                aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffs.begin(), diffs.end());

                std::vector<texture> specs = load_mat_textures(mat,
                                aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specs.begin(), specs.end());
        }

        return mesh(vertices, indices, textures);
}
std::vector<texture> model::load_mat_textures(aiMaterial *aimat,
                        aiTextureType aitype, std::string tname)
{
        std::vector<texture> textures;

        for (GLuint i = 0; i < aimat->GetTextureCount(aitype); i++) {
                aiString texpath;
                bool loaded = false;

                // Get texture path
                aimat->GetTexture(aitype, i, &texpath);

                // Check between loaded textures
                for (GLuint j = 0; j < m_texcache.size(); j++) {

                        if (m_texcache[j].m_path == texpath) {
                                textures.push_back(m_texcache[j]);
                                loaded = true;
                                break;
                        }
                }

                // Otherwise, actually load the texture
                if (!loaded) {
                        texture texture;
                        texture.m_id = texture_from_file(texpath.C_Str(), m_workpath);
                        texture.m_type = tname;
                        texture.m_path = texpath;
                        textures.push_back(texture);
                        m_texcache.push_back(texture);
                }
        }

        return textures;
}

GLint texture_from_file(const char *path, std::string workpath)
{
        int w;
        int h;
        GLuint texID;
        unsigned char *map;
        std::string filepath;

        // Find file
        filepath = workpath + '/' + std::string(path);

        // Load texture
        map = SOIL_load_image(filepath.c_str(), &w, &h, 0, SOIL_LOAD_RGB);
        info("texpath is: %s\n", filepath.c_str());

        // Gen ID & Bind
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, map);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Goodbye
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(map);

        return texID;
}

void model::draw(program& program) const
{
        for (const auto& mesh : m_meshes)
                mesh.draw(program);
}
