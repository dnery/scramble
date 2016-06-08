#ifndef SCRAMBLE_SCENE_TYPE_H
#define SCRAMBLE_SCENE_TYPE_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct scene_type {

        scene_type()
        {
        }

        virtual ~scene_type()
        {
        }

        /*
         * Receive window handle for preparation
         */
        virtual void init(GLFWwindow *) = 0;

        /*
         * Receive current fime for update
         */
        virtual void update(GLdouble) = 0;

        /*
         * Render the entire scene
         */
        virtual void display() = 0;

        /*
         * Drop all resources
         */
        virtual void drop() = 0;
};

#endif /* ifndef SCRAMBLE_SCENE_TYPE_H */
