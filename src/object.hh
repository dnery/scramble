/*
 * Created by danilo on 4/12/16.
 */

#ifndef SCRAMBLE_OBJECT_HH
#define SCRAMBLE_OBJECT_HH

#ifdef CLANG_COMPLETE_ONLY
        #define GL_GLEXT_PROTOTYPES
        #include <GL/gl.h>
        #include <GL/glext.h>
#else
        #include <GL/glew.h>
#endif

#include "program.hh"

namespace scramble {

        struct object {

                object();

                virtual ~object()
                {
                }

                /*
                 * Each instance knows how to bind itself to shader.
                 */
                virtual void bind(scramble::program *program) const = 0;

                /*
                 * Each instance knows how to draw itself.
                 */
                virtual void draw() const = 0;

                /*
                 * Each instance knows how to unbind itself.
                 */
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
                const GLfloat vertices[288]; // All coordinates

                int texw;                    // Texture width
                int texh;                    // Texture height

                GLuint texture1;             // Texture object 1
                GLuint texture2;             // Texture object 2
                GLuint specular;             // Specular map texture

                unsigned char *texmap;       // Texture resource (reusable)
        };
}

#endif // SCRAMBLE_OBJECT_H
