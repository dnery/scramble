//
// Created by danilo on 4/16/16.
//

#ifndef SCRAMBLE_VIEWER_H
#define SCRAMBLE_VIEWER_H

#include <glm/detail/type_vec3.hpp>

namespace scramble {

        struct viewer {

                viewer();

                ~viewer();

                viewer(const viewer& other) = delete;

                viewer& operator=(viewer other) = delete;

                void update();

                void move();

                glm::vec3 position;         // camera: position vector
                glm::vec3 front;            // camera: front vector
                glm::vec3 up;               // camera: up vector
                bool      input_keys[1024]; // active input keys

        private:
                GLfloat delta;              // relative delta
                GLfloat camspeed;           // camera movespeed
                GLfloat lastframe;          // last frame timestamp
                GLfloat currentframe;       // current frame timestamp
        };
}

#endif //SCRAMBLE_VIEWER_H