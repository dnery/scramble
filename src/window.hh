/*
 * Created by danilo on 4/17/16.
 */

#ifndef SCRAMBLE_WINDOW_H
#define SCRAMBLE_WINDOW_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace scramble {

	extern GLuint WIN_WIDTH;
	extern GLuint WIN_HEIGHT;
	extern GLfloat ASPECT_RATIO;

	struct window {

		window(GLuint width = WIN_WIDTH,
		       GLuint height = WIN_HEIGHT,
		       std::string title = "Scramble");

		~window();

		GLuint _width() const;

		GLuint _height() const;

		GLFWwindow *_handle() const;

		void debug_info() const;

	private:
		std::string title;
		GLuint width;
		GLuint height;
		GLFWwindow *handle;
	};
}

#endif /* SCRAMBLE_WINDOW_H */