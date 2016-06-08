#ifndef SCRAMBLE_SHADER_H
#define SCRAMBLE_SHADER_H

#include <string>
#include <GL/glew.h>

/*
 * Reference counting can be a little more complex to implement corectly
 * in C++. Read more into the counted body idiom here:
 *
 *      http://bit.ly/1TOLp5U
 *
 * The implementation used here follows a well known and tested pattern,
 * all while respecting proper (can this be said?) encapsulation.
 */
class shader_rep {

        GLuint globject;        // GL shader resource
        int refcount;           // reference count

public: /* ================================================================== */

        friend class shader;   // shader object access facade

        /*
         * CTOR & DTOR
         */
        shader_rep(std::string code, GLenum type);
        ~shader_rep();

        /*
         * Methods
         */
        std::string compiler_errmsg(GLuint globject);
};

class shader {

        shader_rep *rep;        // pointer to ref-counted shader

public: /* ================================================================== */

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
         * Getters & Setters
         */
        const GLuint& get() const;
};

/*
 * Instantiate shader from source file.
 *
 * The r-value reference is a tiny optimization but imposes the
 * restriction that it must always be called passing an x-value. In this
 * situation, this means, exclusively, using it in conjunction with
 * resource_path(), like this:
 *
 *      shader_from_file(resource_path("<resource_name>"), <SHADER_TYPE>);
 */
shader shader_from_file(const std::string&& path, GLenum type);

#endif /* ifndef SCRAMBLE_SHADER_H */
