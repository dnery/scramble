#ifndef SCRAMBLE_SCENE_H
#define SCRAMBLE_SCENE_H

#include "scene_type.hh"
#include "program.hh"
#include "camera.hh"
#include "model.hh"
#include <array>

/*
 * Mouse POD-type
 */
struct mouse {

        GLdouble m_prev_xpos;
        GLdouble m_prev_ypos;
        GLboolean m_first_enter_viewport = GL_TRUE;
};

/*
 * Keyboard POD-type
 */
struct keyboard {

        GLboolean m_blinn_active;
        GLboolean m_flashlight_active;
        std::array<GLboolean, 1024> m_keymap;
};

/*
 * Actual scene element
 */
class scene : public scene_type {

        /* Input */
        mouse m_mouse;                  // Mouse input element
        camera m_camera;                // Viewport camera object
        keyboard m_keyboard;            // Keyboard input element

        /* Clock */
        GLdouble m_delta;               // Delta difference tracker
        GLdouble m_prev_frame;          // Previous frame timestamp
        GLdouble m_vp_aspect_ratio;     // Viewport aspect ratio

        /* Elements */
        model *m_object;                // Sample solid object
        program *m_object_program;      // Solid object shader program

public:
        scene();

        /* Display */
        void init(GLFWwindow *window) override;
        void update(GLdouble millis) override;
        void display() override;
        void drop() override;

        /* Events */
        void handle_cursor_move(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset) override;
        void handle_keypress(GLFWwindow *window, GLint key, GLint action, GLint mode) override;
        void handle_scroll(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset) override;
};

#endif /* ifndef SCRAMBLE_SCENE_H */
