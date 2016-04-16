//
// Created by danilo on 4/12/16.
//

#ifndef SCRAMBLE_OBJECT_HH
#define SCRAMBLE_OBJECT_HH

#include <GL/glew.h>
#include "shader_linker.hh"

namespace scramble {

        struct simple_object { // THIS IS AN INTERFACE!!!

                simple_object(); // This is an exception

                virtual ~simple_object() {};

                //simple_object(const simple_object& other) = delete;

                //simple_object& operator=(simple_object other) = delete;

                virtual void bind(scramble::program *program) const = 0;

                virtual void draw() const = 0;

                virtual void unbind() const = 0;

        protected:
                GLuint vbo;
                GLuint ebo;
                GLuint vao;
        };

        struct triangle : simple_object {

                virtual void bind(scramble::program *program) const override;

                virtual void draw() const override;

                virtual void unbind() const override;

                triangle();

                ~triangle();

        private:
                const GLfloat vertices[24];
                const GLuint  indices[3];

                int           texw;
                int           texh;
                GLuint        texture;
                unsigned char *texmap;
        };

        struct square : simple_object {

                virtual void bind(scramble::program *program) const override;

                virtual void draw() const override;

                virtual void unbind() const override;

                square();

                ~square();

        private:
                const GLfloat vertices[32];
                const GLuint  indices[6];

                int           texw;
                int           texh;
                GLuint        texture1;
                GLuint        texture2;
                unsigned char *texmap;
        };
}

#endif //SCRAMBLE_OBJECT_HH
