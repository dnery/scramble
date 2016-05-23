/*
 * Created by danilo on 3/31/16.
 */

#ifndef SCRAMBLE_SHADER_H
#define SCRAMBLE_SHADER_H

#ifdef CLANG_COMPLETE_ONLY
        #define GL_GLEXT_PROTOTYPES
        #include <GL/gl.h>
        #include <GL/glext.h>
#else
        #include <GL/glew.h>
#endif

#include <string>

namespace scramble {

        /*
         * Reference counting can be a little more complex to implement corectly
         * in C++. Read more into the counted body idiom here:
         *
         *      http://bit.ly/1TOLp5U
         *
         * The implementation used here follows a well known and tested pattern,
         * all while respecting proper (can this be said?) encapsulation.
         */
        struct shader_rep {

                shader_rep(std::string code, GLenum type);

                ~shader_rep();

                friend struct shader; // shader object access facade

        private:
                GLuint globject;      // GL shader resource
                int refcount;         // reference count
        };

        struct shader {

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

                GLuint get() const;

                friend void swap(shader& a, shader& b);

        private:
                shader_rep *rep; // pointer to ref-counted shader
        };

        /*
         * Compose compiler error message
         */
        std::string compiler_errmsg(GLuint globject);

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
}

#endif // SCRAMBLE_SHADER_H
