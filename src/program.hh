#ifndef SCRAMBLE_PROGRAM_H
#define SCRAMBLE_PROGRAM_H

#include "shader.hh"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class program {

        GLuint globject; // GL program resource

public: /* ================================================================== */

        /*
         * CTOR, DTOR, Copy Semantics
         */
        explicit program(const std::vector<shader>& shaders);
        ~program();

        /*
         * Getters & Setters
         */
        const GLuint& get() const;

        /*
         * Methods
         */
        bool in_use() const;
        void toggle() const;

        GLint attrib(const GLchar *name) const;
        GLint uniform(const GLchar *name) const;

        std::string linker_errmsg(GLuint globject);

/* ========================================================================== */
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

        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)

        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)

        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)

        _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)

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

#endif /* ifndef SCRAMBLE_PROGRAM_H */
