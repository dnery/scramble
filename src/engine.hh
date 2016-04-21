/*
 * Created by danilo on 4/17/16.
 */

#ifndef SCRAMBLE_ENGINE_H
#define SCRAMBLE_ENGINE_H

#include <glm/glm.hpp>
#include "window.hh"
#include "object.hh"
#include "program.hh"

namespace scramble {

	struct engine {

		engine();

		~engine();

		engine(const engine& other) = delete;

		engine& operator=(engine other) = delete;

		void engage();

		void update();

		void render();

	private:
		scramble::window window;    // GL context

		glm::vec3 reps[20];         // draw n-times
		scramble::program *program; // sample shader program
		scramble::object *cube;     // sample container object
	};
}

#endif /* SCRAMBLE_ENGINE_H */