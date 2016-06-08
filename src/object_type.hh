#ifndef SCRAMBLE_OBJECT_TYPE_H
#define SCRAMBLE_OBJECT_TYPE_H

#include "program.hh"

struct object_type {

        GLuint vbo;                     // Vertices buffer
        GLuint ebo;                     // Elements buffer
        GLuint vao;                     // Vertex attribute array

        object_type() :
                vbo(0),
                ebo(0),
                vao(0)
        {
        }

        virtual ~object_type()
        {
        }

        /*
         * Each instance knows how to bind itself to shader.
         */
        virtual void bind(program *program) const = 0;

        /*
         * Each instance knows how to unbind itself.
         */
        virtual void unbind() const = 0;

        /*
         * Each instance knows how to draw itself.
         */
        virtual void draw() const = 0;
};

#endif /* ifndef SCRAMBLE_OBJECT_TYPE_H */
