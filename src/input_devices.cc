#include "input_devices.hh"

/*
 * Mouse: input_device_type
 */
mouse::mouse() :
                m_xdelta(),
                m_ydelta(),
                m_sdelta(),
                m_prev_xpos(),
                m_prev_ypos(),
                m_first_enter_viewport(GL_TRUE)
{
}

void mouse::set_callbacks(GLFWwindow *window)
{
        void *tmp = glfwGetWindowUserPointer(window);

        glfwSetWindowUserPointer(window, this);
        glfwSetCursorPosCallback(window, callback_cursor);
        glfwSetScrollCallback(window, callback_scroll);
        glfwSetWindowUserPointer(window, tmp);
}

void mouse::callback_cursor(GLFWwindow *window, double xdrag, double ydrag)
{
        mouse *m = static_cast<mouse *>(glfwGetWindowUserPointer(window));

        if (m->m_first_enter_viewport) {
                m->m_prev_xpos = static_cast<GLdouble>(xdrag);
                m->m_prev_ypos = static_cast<GLdouble>(ydrag);
                m->m_first_enter_viewport = GL_FALSE;
        }

        // New camera state
        m->m_xdelta = xdrag - m->m_prev_xpos;
        m->m_ydelta = m->m_prev_ypos - ydrag;

        m->m_prev_xpos = static_cast<GLdouble>(xdrag);
        m->m_prev_ypos = static_cast<GLdouble>(ydrag);

        // Update required
        m->m_state_altered = GL_TRUE;
}

void mouse::callback_scroll(GLFWwindow *window, double xoffs, double yoffs)
{
        mouse *m = reinterpret_cast<mouse *>(window);

        m->m_sdelta = static_cast<GLdouble>(yoffs);

        // Update required
        m->m_state_altered = GL_TRUE;
}

/*
 * Keyboard: input_device_type
 */
keyboard::keyboard() : m_flashlight_active(), m_keymap()
{
}

void keyboard::set_callbacks(GLFWwindow *window)
{
        void *tmp = glfwGetWindowUserPointer(window);

        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, callback_keypress);
        glfwSetWindowUserPointer(window, tmp);
}

void keyboard::callback_keypress(GLFWwindow *window, int key, int scan,
                int action, int mode)
{
        keyboard *k = static_cast<keyboard *>(glfwGetWindowUserPointer(window));

        // Check for application exit event (escape)
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

        // Flashlight toggle has to be sluggish
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
                if (k->m_flashlight_active)
                        k->m_flashlight_active = GL_FALSE;
                else
                        k->m_flashlight_active = GL_TRUE;
        }

        // Smooth input triggers
        if (key < 0 || key > 1023)
                return;
        else if (action == GLFW_PRESS)
                k->m_keymap[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
                k->m_keymap[key] = GL_FALSE;

        // Update required
        k->m_state_altered = GL_TRUE;
}
