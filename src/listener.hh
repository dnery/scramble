/*
 * Created by danilo on 4/17/16.
 */

#ifndef SCRAMBLE_LISTENER_H
#define SCRAMBLE_LISTENER_H

#ifdef CLANG_COMPLETE_ONLY
        #define GL_GLEXT_PROTOTYPES
        #include <GL/gl.h>
        #include <GL/glext.h>
#else
        #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

namespace scramble
{
        /*
         * Camera related look positions
         */
        extern GLfloat lastxpos;   // last x look direction
        extern GLfloat lastypos;   // last y look direction

        /*
         * Input workarounds
         */
        extern bool keymap[1024];  // Active key bitmap
        extern bool mouse_enter;   // First occurrence on viewport
        extern bool flashlight;    // Flashlight status

        /*
         * Delta step
         */
        extern GLfloat last_frame; // last frame timestamp
        extern GLfloat delta_time; // time since last frame

        void refresh_delta();

        void refresh_keyput();

        void callback_err(int code, const char *msg);

        void callback_mouse(GLFWwindow *gwindow, double xpos, double ypos);

        void callback_scroll(GLFWwindow *gwindow, double xoffset, double yoffset);

        void callback_keyboard(GLFWwindow *gwindow, int key, int scan, int action, int mode);
}

#endif // SCRAMBLE_LISTENER_H
