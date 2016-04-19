//
// Created by danilo on 4/17/16.
//

#ifndef SCRAMBLE_LISTENER_H
#define SCRAMBLE_LISTENER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace scramble
{
        /*
         * Camera related look positions
         */
        extern GLfloat lastxpos;                            // last x look
        extern GLfloat lastypos;                            // last y look

        /*
         * Input workarounds
         */
        extern bool mouse_enter;                            // First mouse input
        extern bool keymap[1024];                           // GLFW key bitmap

        /*
         * Delta step
         */
        extern GLfloat delta_time;                          // last timestamp diff
        extern GLfloat last_frame;                          // last frame timestamp

        void refresh_delta();

        void refresh_keyput();

        void callback_err(int code, const char *msg);

        void callback_mouse(GLFWwindow *gwindow, double xpos, double ypos);

        void callback_scroll(GLFWwindow *gwindow, double xoffset, double yoffset);

        void callback_keyboard(GLFWwindow *gwindow, int key, int scan, int action, int mode);
}

#endif                                                      //SCRAMBLE_LISTENER_H
