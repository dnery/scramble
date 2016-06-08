#include "object.hh"

#include "ext/platform.hh"
#include <SOIL/SOIL.h>

cube::cube() : object_type(),
        vertices{
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
/*               ^~~~~~~~~~~~~~~~~~^  ^~~~~~~~~~^  ^~~~~~~~~~~~~~~~~~^
 *               positions            texcoords    normals
 */
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f
        },
        texw(0),
        texh(0),
        texture1(0),
        texture2(0),
        texmap(nullptr)
{
        // 1. Make & bind vertex array object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // 2. Make & bind vertex buffer object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        // 4.1 Make & bind texture information
        glGenTextures(1, &texture1);
        texmap = SOIL_load_image(resource_path("framedcrate_diffuse1.png").c_str(),
                                 &texw, &texh, 0, SOIL_LOAD_RGB);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, texmap);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(texmap);

        // 4.1.1 Set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 4.1.2 Set texture filter options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        // 4.1.4 Unbind texture identifier
        glBindTexture(GL_TEXTURE_2D, 0);

        // 4.2 Make & bind texture information
        glGenTextures(1, &texture2);
        texmap = SOIL_load_image(resource_path("framedcrate_diffuse2.jpg").c_str(),
                                 &texw, &texh, 0, SOIL_LOAD_RGB);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, texmap);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(texmap);

        // 4.2.1 Set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 4.2.2 Set texture filter options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        // 4.2.4 Unbind texture identifier
        glBindTexture(GL_TEXTURE_2D, 0);

        // 4.2 Make & bind texture information
        glGenTextures(1, &specular);
        texmap = SOIL_load_image(resource_path("framedcrate_specular.png").c_str(),
                                 &texw, &texh, 0, SOIL_LOAD_RGB);
        glBindTexture(GL_TEXTURE_2D, specular);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, texmap);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(texmap);

        // 4.2.1 Set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 4.2.2 Set texture filter options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        // 4.2.4 Unbind texture identifier
        glBindTexture(GL_TEXTURE_2D, 0);

        // 5.1. Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(0));
        glEnableVertexAttribArray(0);

        // 5.2. Texture coords
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // 5.3. Normal vectors
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(5 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        // 6. Unbind vbo & vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}

cube::~cube()
{
        glDeleteTextures(1, &texture1);
        glDeleteTextures(1, &texture2);
        glDeleteTextures(1, &specular);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
}

void cube::bind(program *program) const
{
        /*
         * Without this check, instantiating a non-textured cube would not be
         * possible using the current framework. It's repeated for every
         * possible texture uniform available in the shader.
         */
        if (glGetUniformLocation(program->get(), "material.diffuse1") != -1) {

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture1);
                glUniform1i(program->uniform("material.diffuse1"), 0);
        }

        if (glGetUniformLocation(program->get(), "material.diffuse2") != -1) {

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture2);
                glUniform1i(program->uniform("material.diffuse2"), 1);
        }

        if (glGetUniformLocation(program->get(), "material.specular") != -1) {

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, specular);
                glUniform1i(program->uniform("material.specular"), 2);
        }

        glBindVertexArray(vao);
}

void cube::draw() const
{
        glDrawArrays(GL_TRIANGLES, 0, 36);
}

void cube::unbind() const
{
        glBindVertexArray(0);
}
