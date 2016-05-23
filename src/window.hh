/*
 * Created by danilo on 4/17/16.
 */

#ifndef SCRAMBLE_WINDOW_H
#define SCRAMBLE_WINDOW_H

#ifdef CLANG_COMPLETE_ONLY
        #define GL_GLEXT_PROTOTYPES
        #include <GL/gl.h>
        #include <GL/glext.h>
#else
        #include <GL/glew.h>
#endif

#include <string>
#include <GLFW/glfw3.h>

/*
 * TODO Not completely sure this class is necessary...
 */

namespace scramble {

        extern GLuint WIN_WIDTH;     // Default global width
        extern GLuint WIN_HEIGHT;    // Default global height
        extern GLfloat ASPECT_RATIO; // Default global aspect-ratio

        struct window {

                window(GLuint width = WIN_WIDTH,
                       GLuint height = WIN_HEIGHT,
                       std::string title = "Scramble");

                ~window();

                GLuint _width() const;

                GLuint _height() const;

                GLFWwindow *_handle() const;

                void debug_info() const;

        private:
                std::string title;  // Window title (Scramble)
                GLuint width;       // Window width
                GLuint height;      // Window height
                GLFWwindow *handle; // Window resource
        };
}

#endif // SCRAMBLE_WINDOW_H
