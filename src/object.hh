//
// Created by danilo on 4/12/16.
//

#ifndef SCRAMBLE_OBJECT_HH
#define SCRAMBLE_OBJECT_HH

#include <GL/glew.h>
#include "program.hh"

namespace scramble {

        struct object { // This is a proper interface!

                object(); // This is an exception, but useful in this case

                virtual ~object() {}; // This should to be this way generally

                virtual void bind(scramble::program *program) const = 0;

                virtual void draw() const = 0;

                virtual void unbind() const = 0;

        protected:
                GLuint vbo; // Vertices buffer
                GLuint ebo; // Elements buffer
                GLuint vao; // Vertex attribute array
        };

        struct cube : object {

                virtual void bind(scramble::program *program) const override;

                virtual void draw() const override;

                virtual void unbind() const override;

                cube();

                ~cube();

        private:
                const GLfloat vertices[180]; // All vertices

                int           texw;	     // Texture width
                int           texh; 	     // Texture height

                GLuint        texture1;      // Texture object 1
                GLuint        texture2;      // Texture object 2

                unsigned char *texmap;       // Texture resource (reusable)
        };
}

#endif                                                       //SCRAMBLE_OBJECT_HH