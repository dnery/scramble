#ifndef SCRAMBLE_WINDOW_MANAGER_H
#define SCRAMBLE_WINDOW_MANAGER_H

#include "scene_type.hh"
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class window_manager {

        /* Window */
        int m_width;                    // Managed window width
        int m_height;                   // Managed window height
        std::string m_title;            // Managed window title
        GLFWwindow *m_window;           // Wrapped window object

        /* Scene */
        scene_type *m_scene;            // Render target Scene

        /*
         * GLFW callback upon window being resized
         */
        static void callback_window_resize(GLFWwindow *window, int w, int h)
        {
                window_manager *wm = static_cast<window_manager *>
                        (glfwGetWindowUserPointer(window));

                wm->viewport_did_resize(w, h);
        }

        /*
         * GLFW callback upon window needing to be re-drawn
         */
        static void callback_window_refresh(GLFWwindow *window)
        {
                window_manager *wm = static_cast<window_manager *>
                        (glfwGetWindowUserPointer(window));

                wm->m_scene->display();
        }

public: /* ================================================================== */

        /*
         * CTOR & DTOR
         */
        window_manager(int width, int height, std::string title, scene_type *scene);

        ~window_manager();

        /*
         * Getters & Setters
         */
        const GLuint width() const
        {
                return static_cast<GLuint>(m_width);
        }

        const GLuint height() const
        {
                return static_cast<GLuint>(m_height);
        }

        /*
         * GLFW conformance resize function
         */
        void viewport_did_resize(int weight, int height);

        /*
         * Render loop: makes scene calls and polls
         */
        void render_scene();
};

#endif /* ifndef SCRAMBLE_WINDOW_MANAGER_H */
