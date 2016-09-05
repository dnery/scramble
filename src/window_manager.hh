#ifndef SCRAMBLE_WINDOW_MANAGER_H
#define SCRAMBLE_WINDOW_MANAGER_H

#include "abs/scene_type.hh"

class window_manager {

        int m_width;                    // Managed window width
        int m_height;                   // Managed window height
        const char *m_title;            // Managed window title
        GLFWwindow *m_window;           // Wrapped window object
        scene_type *m_scene;            // Render target Scene

        /* GLFW callback upon window being resized */
        inline static void callback_window_resize(GLFWwindow *window, int w, int h)
        {
                window_manager *wm = static_cast<window_manager *>
                        (glfwGetWindowUserPointer(window));

                wm->viewport_did_resize(w, h);
        }

        /* GLFW callback upon window needing to be re-drawn */
        inline static void callback_window_refresh(GLFWwindow *window)
        {
                window_manager *wm = static_cast<window_manager *>
                        (glfwGetWindowUserPointer(window));

                wm->m_scene->display();
        }

        /* GLFW input callback redirection functions: cursor move */
        inline static void callback_cursor_move(GLFWwindow *window, double x, double y)
        {
                window_manager *wm = static_cast<window_manager *>
                        (glfwGetWindowUserPointer(window));

                wm->m_scene->handle_cursor_move(window, x, y);
        }

        /* GLFW input callback redirection functions: keypress */
        inline static void callback_keypress(GLFWwindow *window, int k, int s, int a, int m)
        {
                window_manager *wm = static_cast<window_manager *>
                        (glfwGetWindowUserPointer(window));

                wm->m_scene->handle_keypress(window, k, a, m);
        }

        /* GLFW input callback redirection functions: scroll */
        inline static void callback_scroll(GLFWwindow *window, double x, double y)
        {
                window_manager *wm = static_cast<window_manager *>
                        (glfwGetWindowUserPointer(window));

                wm->m_scene->handle_scroll(window, x, y);
        }

public:
        window_manager(int width, int height, const char * title, scene_type *scene);

        ~window_manager();

        GLuint width() const
        {
                return static_cast<GLuint>(m_width);
        }

        GLuint height() const
        {
                return static_cast<GLuint>(m_height);
        }

        /* GLFW conformance resize function */
        void viewport_did_resize(int weight, int height);

        /* Render loop */
        void render_scene();
};

#endif /* ifndef SCRAMBLE_WINDOW_MANAGER_H */
