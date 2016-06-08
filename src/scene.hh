#ifndef SCRAMBLE_SCENE_H
#define SCRAMBLE_SCENE_H

#include "camera.hh"
#include "object.hh"
#include "program.hh"
#include "scene_type.hh"
#include "input_devices.hh"

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
        object_type *m_object;          // Sample solid object
        object_type *m_caster;          // Sample light caster
        program *m_object_program;      // Solid object shader program
        program *m_caster_program;      // Light caster shader program

public: /* ================================================================== */

        scene();

        void init(GLFWwindow *window) override;
        void update(GLdouble millis) override;
        void display() override;
        void drop() override;
};

#endif /* ifndef SCRAMBLE_SCENE_H */
