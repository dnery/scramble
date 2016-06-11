#include "model.hh"

#include <SOIL/SOIL.h>
#include <stdexcept>
#include "debug.hh"

model::model(const std::string&& path)
{
        Assimp::Importer imp;

        // Postprocess: triangulate mesh, flip UV maps
        const aiScene *aiscene = imp.ReadFile(path,
                        aiProcess_CalcTangentSpace |
                        aiProcess_SplitLargeMeshes |
                        aiProcess_OptimizeMeshes |
                        aiProcess_Triangulate |
                        aiProcess_GenNormals |
                        aiProcess_FlipUVs);

        // Check for errors
        if (!aiscene || !aiscene->mRootNode || aiscene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
                throw std::runtime_error(imp.GetErrorString());

        // Get workdir
        m_workpath = path.substr(0, path.find_last_of('/'));

        // Procceed
        process_node(aiscene->mRootNode, aiscene);
}

void model::process_node(aiNode *ainode, const aiScene *aiscene)
{
        // Process node's children recursively
        for (GLuint i = 0; i < ainode->mNumChildren; i++)
                process_node(ainode->mChildren[i], aiscene);

        // Process each mesh linearly
        for (GLuint i = 0; i < ainode->mNumMeshes; i++) {
                aiMesh *mesh = aiscene->mMeshes[ainode->mMeshes[i]];
                m_meshes.push_back(process_mesh(mesh, aiscene));
        }
}

mesh model::process_mesh(aiMesh *aimesh, const aiScene *aiscene)
{
        std::vector<mesh::vertex> vertices;     // Holds all mesh's vertices
        std::vector<GLuint> indices;            // Holds all mesh's element indices
        std::vector<mesh::texture> textures;    // Holds all mesh's textures

        // Retrieve vertex data
        for (GLuint i = 0; i < aimesh->mNumVertices; i++) {

                mesh::vertex vertex;            // Shortening declaration scope

                // Retrieve vertex position
                glm::vec3 position;
                position.x = aimesh->mVertices[i].x;
                position.y = aimesh->mVertices[i].y;
                position.z = aimesh->mVertices[i].z;
                vertex.m_position = position;

                // Retrieve vertex normal vector
                glm::vec3 normal;
                normal.x = aimesh->mNormals[i].x;
                normal.y = aimesh->mNormals[i].y;
                normal.z = aimesh->mNormals[i].z;
                vertex.m_normal = normal;

                // Retrieve vertex texcoords, if any
                if (aimesh->mTextureCoords[0]) {
                      glm::vec2 texcoord;
                      texcoord.x = aimesh->mTextureCoords[0][i].x;
                      texcoord.y = aimesh->mTextureCoords[0][i].y;
                      vertex.m_texcoord = texcoord;
                }

                // TBN: retrieve tangent
                glm::vec3 tangent;
                tangent.x = aimesh->mTangents[i].x;
                tangent.y = aimesh->mTangents[i].y;
                tangent.z = aimesh->mTangents[i].z;
                vertex.m_tangent = tangent;

                // TBN: retrieve bitangent
                glm::vec3 bitangent;
                bitangent.x = aimesh->mBitangents[i].x;
                bitangent.y = aimesh->mBitangents[i].y;
                bitangent.z = aimesh->mBitangents[i].z;
                vertex.m_bitangent = bitangent;

                vertices.push_back(vertex);
        }

        // Process element indices
        for (GLuint i = 0; i < aimesh->mNumFaces; i++) {
                aiFace face = aimesh->mFaces[i];

                for (GLuint j = 0; j < face.mNumIndices; j++)
                        indices.push_back(face.mIndices[j]);
        }

        // Process material
        if (aimesh->mMaterialIndex) {
                aiMaterial *mat = aiscene->mMaterials[aimesh->mMaterialIndex];

                std::vector<mesh::texture> diffs = load_mat_textures(mat,
                                aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffs.begin(), diffs.end());

                std::vector<mesh::texture> specs = load_mat_textures(mat,
                                aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specs.begin(), specs.end());

                std::vector<mesh::texture> norms = load_mat_textures(mat,
                                aiTextureType_HEIGHT, "texture_normal");
                textures.insert(textures.end(), norms.begin(), norms.end());
        }

        return mesh(vertices, indices, textures);
}
std::vector<mesh::texture> model::load_mat_textures(aiMaterial *aimat,
                        aiTextureType aitype,
                        std::string textype)
{
        std::vector<mesh::texture> textures;    // Holds textures for material (returned)

        for (GLuint i = 0; i < aimat->GetTextureCount(aitype); i++) {

                aiString texpath;               // Retrieved texture absolute path
                bool loaded = false;            // Has it already been loaded?

                // Get texture path
                aimat->GetTexture(aitype, i, &texpath);

                // Check the texture cache
                for (const auto& texture : m_texcache) {

                        if (texture.m_path == texpath) {
                                textures.push_back(texture);
                                loaded = true;
                                break;
                        }
                }

                // Otherwise, actually load the texture
                if (!loaded) {
                        mesh::texture texture;
                        texture.m_id = texture_from_file(texpath.C_Str(), m_workpath);
                        texture.m_type = textype;
                        texture.m_path = texpath;
                        textures.push_back(texture);
                        m_texcache.push_back(texture);
                }
        }

        return textures;
}

void model::draw(program& program) const
{
        for (const auto& mesh : m_meshes)
                mesh.draw(program);
}

GLint texture_from_file(const char *path, std::string workpath)
{
        int w;                  // Texture image width
        int h;                  // Texture image height
        GLuint texID;           // Texture OpenGL ID
        unsigned char *image;   // The actual image
        std::string filepath;   // Path to image

        // Find file
        filepath = workpath + '/' + std::string(path);

        // Load texture
        image = SOIL_load_image(filepath.c_str(), &w, &h, 0, SOIL_LOAD_RGB);

        // Assert the filepath
        info("texpath is: %s\n", filepath.c_str());

        // Gen ID & Bind
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Goodbye
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(image);

        return texID;
}
