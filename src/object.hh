#ifndef SCRAMBLE_OBJECT_H
#define SCRAMBLE_OBJECT_H

#include "object_type.hh"

class cube : public object_type {

        const GLfloat vertices[288];    // All coordinates

        int texw;                       // Texture width
        int texh;                       // Texture height

        GLuint texture1;                // Texture object 1
        GLuint texture2;                // Texture object 2
        GLuint specular;                // Specular map texture

        unsigned char *texmap;          // Texture resource (reusable)

public:
        cube();

        ~cube();

        virtual void bind(program *program) const override;

        virtual void unbind() const override;

        virtual void draw() const override;
};

class plane : public object_type {

        const GLfloat vertices[48];    // All coordinates

        int texw;                       // Texture width
        int texh;                       // Texture height

        GLuint texture1;                // Texture object 1
        GLuint specular1;               // Texture object 1

        unsigned char *texmap;          // Texture resource (reusable)

public:
        plane();

        ~plane();

        virtual void bind(program *program) const override;

        virtual void unbind() const override;

        virtual void draw() const override;
};

#endif /* ifndef SCRAMBLE_OBJECT_H */
