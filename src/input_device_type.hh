#ifndef SCRAMBLE_INPUT_DEVICE_TYPE_H
#define SCRAMBLE_INPUT_DEVICE_TYPE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct input_device_type {

        GLboolean m_state_altered; // Input device state tracker

        /*
         * CTOR & DTOR
         */
        input_device_type() : m_state_altered()
        {
        }

        virtual ~input_device_type()
        {
        }

        /*
         * Setter for inherited data member
         */
        const GLboolean& state_altered() const
        {
                return m_state_altered;
        }

        /*
         * Getter for inherited data member
         */
        void state_altered(const GLboolean& state)
        {
                m_state_altered = state;
        }

        /*
         * Public interface declaration
         */
        virtual void set_callbacks(GLFWwindow *window) = 0;
};

#endif /* ifndef SCRAMBLE_INPUT_DEVICE_TYPE_H */
