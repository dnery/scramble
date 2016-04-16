//
// Created by danilo on 4/12/16.
//

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include "objects.hh"
#include "platform.hh"
#include "shader_linker.hh"

scramble::simple_object::simple_object() :
        vbo(0),
        ebo(0),
        vao(0)
{
}

scramble::triangle::triangle() : simple_object(),
        vertices{
                // Verts            // Colors         // Tex-coords
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // lo-left
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // lo-right
                 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f  // hi-middle
        },
        indices{
                0, 1, 2
        },
        texw(0),
        texh(0),
        texture(0),
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

        // 3. Make & bind element buffer object
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        // 4. Make & bind texture information
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // 4.1 Set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 4.2 Set texture filter options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        // 4.3 Load in texture data
        texmap = SOIL_load_image(resource_path("illuminati.png").c_str(),
                                  &texw, &texh, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, texmap);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 5.1. Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(0));
        glEnableVertexAttribArray(0);

        // 5.2. Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // 5.3 Texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        // 6. Unbind tex, vbo & vao (not the ebo!)
        SOIL_free_image_data(texmap);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}

scramble::triangle::~triangle()
{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteVertexArrays(1, &vao);
        glDeleteTextures(1, &texture);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
}

void scramble::triangle::bind(scramble::program *program) const
{
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
}

void scramble::triangle::draw() const
{
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void scramble::triangle::unbind() const
{
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
}

scramble::square::square() : simple_object(),
        vertices{
                // Verts            // Colors         // Tex-coords
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bot-left
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bot-right
                 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
                -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // top-left
        },
        indices{
                0, 1, 3,
                1, 2, 3
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

        // 3. Make & bind element buffer object
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        // 4.1 Make & bind texture information
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // 4.1.1 Set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 4.1.2 Set texture filter options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        // 4.1.3 Load in texture data
        texmap = SOIL_load_image(resource_path("boxxy.jpg").c_str(),
                                 &texw, &texh, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, texmap);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 4.1.4 Free texture data
        SOIL_free_image_data(texmap);
        glBindTexture(GL_TEXTURE_2D, 0);

        // 4.2 Make & bind texture information
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // 4.2.1 Set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 4.2.2 Set texture filter options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        // 4.2.3 Load in texture data
        texmap = SOIL_load_image(resource_path("awesomeface.png").c_str(),
                                 &texw, &texh, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, texmap);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 4.2.4 Free texture data
        SOIL_free_image_data(texmap);
        glBindTexture(GL_TEXTURE_2D, 0);

        // 5.1. Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(0));
        glEnableVertexAttribArray(0);

        // 5.2. Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // 5.3 Texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              reinterpret_cast<GLvoid *>(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        // 6. Unbind vbo & vao (not the ebo!)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}

scramble::square::~square()
{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteVertexArrays(1, &vao);
        glDeleteTextures(1, &texture1);
        glDeleteTextures(1, &texture2);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
}

void scramble::square::bind(scramble::program *program) const
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(program->uniform("fs_texture1"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(program->uniform("fs_texture2"), 1);

        glBindVertexArray(vao);
}

void scramble::square::draw() const
{
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void scramble::square::unbind() const
{
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
}
