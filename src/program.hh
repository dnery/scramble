#ifndef SCRAMBLE_PROGRAM_H
#define SCRAMBLE_PROGRAM_H

#include "shader.hh"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class program {

        GLuint globject; // GL program resource

public:
        explicit program(const std::vector<shader>& shaders);
        ~program();

        const GLuint& get() const
        {
                return globject;
        }

        bool in_use() const;
        void toggle() const;
        GLint attrib(const GLchar *name) const;
        GLint uniform(const GLchar *name) const;

        /* Error check */
        std::string error(GLuint globject);

        /* Write signatures as macros */
#define _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE) \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
        \
        void setAttrib1v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib2v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib3v(const GLchar* attribName, const OGL_TYPE* v); \
        void setAttrib4v(const GLchar* attribName, const OGL_TYPE* v); \
        \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
        void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
        \
        void setUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
        void setUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \

        /* Generate previously proposed declarations */
        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)
        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)
        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)
        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)

        /* Uniforms & Attribs should be set through these */
        void setUniformMatrix2(const GLchar *uniformName,
                        const GLfloat *v, GLsizei count = 1,
                        GLboolean transpose = GL_FALSE);
        void setUniformMatrix3(const GLchar *uniformName,
                        const GLfloat *v, GLsizei count = 1,
                        GLboolean transpose = GL_FALSE);
        void setUniformMatrix4(const GLchar *uniformName,
                        const GLfloat *v, GLsizei count = 1,
                        GLboolean transpose = GL_FALSE);
        void setUniform(const GLchar *uniformName, const glm::mat2& m,
                        GLboolean transpose = GL_FALSE);
        void setUniform(const GLchar *uniformName, const glm::mat3& m,
                        GLboolean transpose = GL_FALSE);
        void setUniform(const GLchar *uniformName, const glm::mat4& m,
                        GLboolean transpose = GL_FALSE);
        void setUniform(const GLchar *uniformName, const glm::vec3& v);
        void setUniform(const GLchar *uniformName, const glm::vec4& v);
};

/*
 * Instantiate shader from source file.
 *
 * -- NOTE TO SELF --
 *
 * the r-value reference is a tiny optimization but imposes the
 * restriction that it must always be called passing an x-value.
 * in this situation, this means, exclusively, using it in
 * conjunction with resource_path(), like this:
 *
 *      shader_from_file(resource_path("<resource_name>"), <SHADER_TYPE>);
 */
shader shader_from_file(const std::string&& path, GLenum type);

#endif /* ifndef SCRAMBLE_PROGRAM_H */
