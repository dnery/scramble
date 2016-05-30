/*
 * Created by danilo on 4/17/16.
 */

#include <stdexcept>
#include "camera.hh"
#include "window.hh"
#include "locator.tt"
#include "listener.hh"

/*
 * Abbreviations
 */
using camService = scramble::locator<scramble::camera>;

GLfloat scramble::lastxpos = WIN_WIDTH / 2.0f;  // last x look direction
GLfloat scramble::lastypos = WIN_HEIGHT / 2.0f; // last y look direction

bool scramble::keymap[1024];                    // Active key bitmap
bool scramble::mouse_enter = true;              // First occurrence on viewport
bool scramble::flashlight = false;              // Flashlight status

GLfloat scramble::last_frame = 0.0f;            // last frame timestamp
GLfloat scramble::delta_time = 0.0f;            // time since last frame

/*
 * Refresh delta step
 */
void scramble::refresh_delta()
{
        GLfloat curr_frame = static_cast<GLfloat>(glfwGetTime());

        delta_time = curr_frame - last_frame;
        last_frame = curr_frame;
}

/*
 * Refresh key inputs
 */
void scramble::refresh_keyput()
{
        if (keymap[GLFW_KEY_A])
                camService::current()->keypress(scramble::movement::LEFT,
                                delta_time);
        if (keymap[GLFW_KEY_D])
                camService::current()->keypress(scramble::movement::RIGHT,
                                delta_time);
        if (keymap[GLFW_KEY_W])
                camService::current()->keypress(scramble::movement::FORWARD,
                                delta_time);
        if (keymap[GLFW_KEY_S])
                camService::current()->keypress(scramble::movement::BACKWARD,
                                delta_time);
}

/*
 * Callback: on error function
 */
void scramble::callback_err(int code, const char *msg)
{
        throw std::runtime_error(msg);
}

/*
 * Callback: mouse-input function
 */
void scramble::callback_mouse(GLFWwindow *gwindow, double xpos, double ypos)
{
        if (mouse_enter) {
                lastxpos = static_cast<GLfloat>(xpos);
                lastypos = static_cast<GLfloat>(ypos);
                mouse_enter = false;
        }

        camService::current()->mouse_look(static_cast<GLfloat>(xpos - lastxpos),
                                          static_cast<GLfloat>(lastypos - ypos));

        lastxpos = static_cast<GLfloat>(xpos);
        lastypos = static_cast<GLfloat>(ypos);
}

/*
 * Callback: mouse-scroll function
 */
void scramble::callback_scroll(GLFWwindow *gwindow, double xoffset,
                               double yoffset)
{
        camService::current()->mouse_scroll(static_cast<GLfloat>(yoffset));
}

/*
 * Callback: keypress function
 */
void scramble::callback_keyboard(GLFWwindow *gwindow, int key, int scan,
                                 int action, int mode)
{
        // Check for application exit event (escape)
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(gwindow, GL_TRUE);

        // Flashlight toggle has to be sluggish
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
                if (flashlight)
                        flashlight = false;
                else
                        flashlight = true;
        }

        // Smooth input triggers
        if (key < 0 || key > 1023)
                return;
        else if (action == GLFW_PRESS)
                keymap[key] = true;
        else if (action == GLFW_RELEASE)
                keymap[key] = false;
}
