/*
 * Created by danilo on 4/12/16.
 */

#include <SOIL/SOIL.h>
#include "object.hh"
#include "ext/platform.hh"

scramble::object::object() :
	vbo(0),
	ebo(0),
	vao(0)
{
}

scramble::cube::cube() :
	object(),
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
	texmap = SOIL_load_image(resource_path("pentagram.jpg").c_str(),
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

	// 5.2. Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
	                      reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 5.3. Normal vectors
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
	                      reinterpret_cast<GLvoid *>(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// 6. Unbind vbo & vao (not the ebo!)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

scramble::cube::~cube()
{
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void scramble::cube::bind(scramble::program *program) const
{
	/*
	 * Without this check, instantiating a non-textured cube would not be
	 * possible using the current framework.
	 */
	if (glGetUniformLocation(program->get(), "fs_texture1") != -1) {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(program->uniform("fs_texture1"), 0);
	}

	/*
	 * Without this check, instantiating a non-textured cube would not be
	 * possible using the current framework.
	 */
	if (glGetUniformLocation(program->get(), "fs_texture2") != -1) {

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(program->uniform("fs_texture2"), 1);
	}

	glBindVertexArray(vao);
}

void scramble::cube::draw() const
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void scramble::cube::unbind() const
{
	glBindVertexArray(0);
}
