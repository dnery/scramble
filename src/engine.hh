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
                /*
                 * FIXME Every member data here is hard-coded.
                 *
                 * Get rid of this as soon as possible!
                 */
                scramble::window window;           // GL context window
                scramble::program *object_program; // Object shader program
                scramble::program *caster_program; // Caster shader program
                scramble::object *cube;            // Sample cube object
                scramble::object *lamp;            // Light caster object
        };
}

#endif // SCRAMBLE_ENGINE_H
