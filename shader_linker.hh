//
// Created by danilo on 4/1/16.
//

#ifndef SCRAMBLE_SHADER_PROGRAM_H
#define SCRAMBLE_SHADER_PROGRAM_H

#include <vector>
#include <GL/glew.h>

#include "shader_loader.hh"

namespace scramble {

        struct program {

                program();

                program(const std::vector<scramble::shader>& shaders);

                ~program();

                program(const program& other) = delete;

                program& operator=(program other) = delete;

                GLuint get() const;

                GLint attrib(const GLchar *name) const;

                GLint uniform(const GLchar *name) const;

        private:
                GLuint globject; // GL program object itself
        };
}


#endif //SCRAMBLE_SHADER_PROGRAM_H
