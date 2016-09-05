#ifndef SCRAMBLE_SHADER_H
#define SCRAMBLE_SHADER_H

#include <GL/glew.h>
#include <string>

/*
 * -- NOTE TO SELF --
 *
 * read into refcounting again:
 *
 *      http://bit.ly/1TOLp5U
 */
class shared_shader {

        GLuint globject;        // GL shader resource
        GLuint refcount;        // reference count

public:
        friend class shader;    // object facade

        shared_shader(std::string code, GLenum type);
        ~shared_shader();

        /* Error check */
        std::string error(GLuint globject);
};

class shader {

        shared_shader *rep;        // pointer to ref-counted shader

public:
        shader(std::string code, GLenum type);
        ~shader();

        /* Shader is ref-counted; can be copy constructed */
        shader(const shader& other);

        /* Shader is ref-counted; can be copy assigned */
        shader& operator=(shader other);

        /* Non-throwing swap function */
        friend void swap(shader& a, shader& b);

        const GLuint& get() const
        {
                return rep->globject;
        }
};

#endif /* ifndef SCRAMBLE_SHADER_H */
