/*
 * Created by danilo on 4/17/16.
 */

#include "listener.hh"

#include <stdexcept>
#include "camera.hh"
#include "window.hh"

GLfloat scramble::lastxpos = scramble::WIN_WIDTH / 2.0f;  // last x look
GLfloat scramble::lastypos = scramble::WIN_HEIGHT / 2.0f; // last y look

bool scramble::keymap[1024];                              // GLFW key bitmap
bool scramble::mouse_enter = true;                        // First mouse input

GLfloat scramble::delta_time = 0.0f;                      // last timestamp diff
GLfloat scramble::last_frame = 0.0f;                      // last frame timestamp

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
		scramble::camera.keypress(scramble::movement::LEFT, delta_time);
	if (keymap[GLFW_KEY_D])
		scramble::camera
			.keypress(scramble::movement::RIGHT, delta_time);
	if (keymap[GLFW_KEY_W])
		scramble::camera
			.keypress(scramble::movement::FORWARD, delta_time);
	if (keymap[GLFW_KEY_S])
		scramble::camera
			.keypress(scramble::movement::BACKWARD, delta_time);
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

	scramble::camera.mouse_look(static_cast<GLfloat>(xpos - lastxpos),
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
	scramble::camera.mouse_scroll(static_cast<GLfloat>(yoffset));
}

/*
 * Callback: keypress function
 */
void scramble::callback_keyboard(GLFWwindow *gwindow, int key, int scan,
                                 int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(gwindow, GL_TRUE);

	/*
	 * FIXME If an odd input triggers known behaviour, the problem is here!
	 */
	key = abs(key % 1024);

	if (action == GLFW_PRESS)
		keymap[key] = true;
	else if (action == GLFW_RELEASE)
		keymap[key] = false;
}
