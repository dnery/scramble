//
// Created by danilo on 4/12/16.
//

#include <GL/glew.h>

#include "objects.hh"

void scramble::load_triangle(GLuint& vao, GLuint& ebo, GLuint& vbo)
{
        GLfloat verts[] = { // Vertices to draw
                -0.5f, -0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.0f,  0.5f, 0.0f
        };

        GLuint inds[] = {
                0, 1, 2
        };

        // 1. Make & bind vertex array object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // 2. Make & bind vertex buffer object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        // 3. Make & bind element buffer object
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

        // 4. Link vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);

        // 5. Unbind vbo & vao (not the ebo!)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}

void scramble::load_square(GLuint& vao, GLuint& ebo, GLuint& vbo)
{
        GLfloat verts[] = { // Vertices to draw
                -0.5f, -0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.5f,  0.5f, 0.0f,
                -0.5f,  0.5f, 0.0f
        };

        GLuint inds[] = { // Drawing indices
                0, 1, 3,
                1, 2, 3
        };

        // 1. Make & bind vertex array object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // 2. Make & bind vertex buffer object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        // 3. Make & bind element buffer object
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

        // 4. Link vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);

        // 5. Unbind vbo & vao (not the ebo!)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}
