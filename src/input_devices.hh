#ifndef SCRAMBLE_INPUT_DEVICES_H
#define SCRAMBLE_INPUT_DEVICES_H

#include "input_device_type.hh"
#include <array>

class mouse : public input_device_type {

        GLdouble m_xdelta;                      // X delta from latest look action
        GLdouble m_ydelta;                      // Y delta form latest look action
        GLdouble m_sdelta;                      // Drag delta from latest scroll action

        GLdouble m_prev_xpos;                   // Previous X mouse position
        GLdouble m_prev_ypos;                   // Previous Y mouse position

        GLboolean m_first_enter_viewport;       // First time cursor enters viewport

public: /* ================================================================== */

        mouse();

        const GLdouble& xdelta() const
        {
                return m_xdelta;
        }

        const GLdouble& ydelta() const
        {
                return m_ydelta;
        }

        const GLdouble& sdelta() const
        {
                return m_sdelta;
        }

        // Implement interface
        void set_callbacks(GLFWwindow *window);

        // Declare callbacks
        static void callback_cursor(GLFWwindow *window, double xdrag, double ydrag);
        static void callback_scroll(GLFWwindow *window, double xoffs, double yoffs);
};

class keyboard : public input_device_type {

        GLboolean m_flashlight_active;          // Flashlight object state

        std::array<GLboolean, 1024> m_keymap;   // key status (ative/unactive)

public: /* ================================================================== */

        keyboard();

        const GLboolean& flashlight_active() const
        {
                return m_flashlight_active;
        }

        const std::array<GLboolean, 1024>& keymap() const
        {
                return m_keymap;
        }

        // Implement interface
        void set_callbacks(GLFWwindow *window);

        // Declare callbacks
        static void callback_keypress(GLFWwindow *window, int key, int scan,
                        int action, int mode);
};

#endif /* ifndef SCRAMBLE_INPUT_DEVICES_H */
