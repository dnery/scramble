//
// Created by danilo on 3/31/16.
//

#ifndef SCRAMBLE_SHADER_LOADER_H
#define SCRAMBLE_SHADER_LOADER_H

#include <string>

#include <GL/glew.h>

namespace scramble {

        struct shader_rep {

                friend struct shader;   // shader object access facade

                shader_rep(std::string code, GLenum type);

                ~shader_rep();

        private:
                GLuint        globject; // GL shader object itself
                int           refcount; // reference count
        };

        struct shader {

                shader(std::string code, GLenum type);

                ~shader();

                shader(const shader& other);

                shader& operator=(shader other);

                GLuint get() const;

                friend void swap(shader& a, shader& b);

        private:
                shader_rep *rep; // pointer to ref-counted shader
        };

        // Compose compiler error message
        std::string compiler_errmsg(GLuint globject);

        // Instantiate shader from source file
        shader shader_from_file(const std::string path, GLenum type);
}

#endif //SCRAMBLE_SHADER_LOADER_H