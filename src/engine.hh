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
		scramble::window window;           // GL context

		glm::vec3 reps[20];                // draw n-times
		scramble::program *object_program; // object shader program
		scramble::program *caster_program; // caster shader program
		scramble::object *cube;            // sample cube object
		scramble::object *lamp;            // light caster object
	};
}

#endif /* SCRAMBLE_ENGINE_H */