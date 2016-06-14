#ifndef SCRAMBLE_SCENE_TYPE_H
#define SCRAMBLE_SCENE_TYPE_H

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

        /*
         * Event handler: cursor move
         */
        virtual void handle_cursor_move(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset) = 0;

        /*
         * Event handler: keypress
         */
        virtual void handle_keypress(GLFWwindow *window, GLint key, GLint action, GLint mode) = 0;

        /*
         * Event handler: scroll
         */
        virtual void handle_scroll(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset) = 0;
};

#endif /* ifndef SCRAMBLE_SCENE_TYPE_H */
