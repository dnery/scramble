/*
 * Created by danilo on 4/17/16.
 */

#include "window.hh"

#include <iostream>
#include "ext/unscramble.hh"

// Global default width
GLuint  scramble::WIN_WIDTH = 1024;
// Global default height
GLuint  scramble::WIN_HEIGHT = 768;
// Global definer for aspect ratio
GLfloat scramble::ASPECT_RATIO = static_cast<GLfloat>(scramble::WIN_WIDTH) /
                                 static_cast<GLfloat>(scramble::WIN_HEIGHT);

scramble::window::window(GLuint width, GLuint height, std::string title) :
	title(title),
	width(width),
	height(height),
	handle(nullptr)
{
	glfwInit();

	// Core profile critical
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create GLFW context window
	handle = glfwCreateWindow(width, height, title.c_str(),
	                          nullptr, nullptr);

	// Check return, set context
	unsc_assert(handle != nullptr);
	glfwMakeContextCurrent(handle);

	// Set input modes
	glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

scramble::window::~window()
{
	glfwDestroyWindow(handle);
	glfwTerminate();
}

GLuint scramble::window::_width() const
{
	return width;
}

GLuint scramble::window::_height() const
{
	return height;
}

GLFWwindow *scramble::window::_handle() const
{
	return handle;
}

void scramble::window::debug_info() const
{
	std::cout << "OpenGL version: " <<
	glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " <<
	glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Renderer: " <<
	glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vendor: " <<
	glGetString(GL_VENDOR) << std::endl;
}
