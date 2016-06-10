#ifndef SCRAMBLE_SHADER_H
#define SCRAMBLE_SHADER_H

#include <GL/glew.h>
#include <string>

/*
 * Reference counting can be a little more complex to implement corectly
 * in C++. Read more into the counted body idiom here:
 *
 *      http://bit.ly/1TOLp5U
 *
 * The implementation used here follows a well known and tested pattern,
 * all while respecting proper (can this be said?) encapsulation.
 */
class shared_shader {

        GLuint globject;        // GL shader resource
        GLuint refcount;        // reference count

public:
        friend class shader;    // shader object access facade

        /*
         * CTOR & DTOR
         */
        shared_shader(std::string code, GLenum type);
        ~shared_shader();

        /*
         * Error check
         */
        std::string error(GLuint globject);
};

class shader {

        shared_shader *rep;        // pointer to ref-counted shader

public:
        shader(std::string code, GLenum type);
        ~shader();

        /*
         * Shader is ref-counted; can be copy constructed
         */
        shader(const shader& other);

        /*
         * Shader is ref-counted; can be copy assigned
         */
        shader& operator=(shader other);

        /*
         * Non-throwing swap function, for clean copy
         */
        friend void swap(shader& a, shader& b);

        /*
         * Single Getter
         */
        const GLuint& get() const
        {
                return rep->globject;
        }
};

#endif /* ifndef SCRAMBLE_SHADER_H */
