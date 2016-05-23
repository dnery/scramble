/*
 * Created by danilo on 4/17/16.
 */

#include <glm/gtc/matrix_transform.hpp>
#include "camera.hh"

const GLfloat scramble::PIT = 0.0f;         // default pitch
const GLfloat scramble::YAW = -90.0f;       // default yaw
const GLfloat scramble::ZOOM = 45.0f;       // default zoom
const GLfloat scramble::SPEED = 5.0f;       // default speed
const GLfloat scramble::SENSITIVITY = 0.1f; // default sensitivity

scramble::camera::camera() :
        /*
         * Vector member data
         */
        position(glm::vec3(0.0f, 0.0f, 3.0f)),
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        right(glm::vec3(1.0f, 0.0f, 0.0f)),
        cam_up(glm::vec3(0.0f, 1.0f, 0.0f)),
        world_up(glm::vec3(0.0f, 1.0f, 0.0f)),
        /*
         * Scalar member data
         */
        look_pit(PIT),
        look_yaw(YAW),
        look_zoom(ZOOM),
        move_speed(SPEED),
        sensitivity(SENSITIVITY)
{
}

void scramble::camera::keypress(GLuint direction, GLfloat delta_time)
{
        GLfloat final_speed = move_speed * delta_time;

        /*
         * Name deduction in nested name specifier is a very particular
         * C++11 extension, and clang_check might flag the following
         * conditional statements even with -std=c++11 switch set.
         *
         * Surrounding the code with the following clang pragmas will
         * silence static analyzer:
         *
         *      #pragma clang diagnostic push
         *      #pragma clang diagnostic ignored "-Wc++11-extensions"
         *      ...
         *      #pragma clang diagnostic pop
         *
         * The correct thing to do, if the need arises, though, is to
         * swap the standard, wherever necessary (including, possibly,
         * the .syntastic_clang_check_config file), from -std=c++11
         * to -std=c++1y (what in c++11 is an extension, in c++1y
         * is provided as standard, and so forth).
         *
         * This will almost certainly be necessary if decltype(auto) or
         * default initializers for aggregates are used, for example.
         */

        if (direction == movement::LEFT)
                position -= right * final_speed;

        if (direction == movement::RIGHT)
                position += right * final_speed;

        if (direction == movement::FORWARD)
                position += front * final_speed;

        if (direction == movement::BACKWARD)
                position -= front * final_speed;
}

void scramble::camera::mouse_look(GLfloat xoffset,
                                  GLfloat yoffset,
                                  GLboolean constrain)
{
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        look_yaw += xoffset;
        look_pit += yoffset;

        if (constrain) {
                look_pit = (look_pit > 89.0f ? 89.0f : look_pit);
                look_pit = (look_pit < -89.0f ? -89.0f : look_pit);
        }

        update_vectors();
}

void scramble::camera::mouse_scroll(GLfloat yoffset)
{
        look_zoom -= (yoffset * sensitivity);
        look_zoom = (look_zoom < 1.0f ? 1.0f : look_zoom);
        look_zoom = (look_zoom > 90.0f ? 90.0f : look_zoom);
}

void scramble::camera::update_vectors()
{
        /*
         * Re-calculate front vector TODO sketch this procedure!
         */
        front.x = (cos(glm::radians(look_yaw)) * cos(glm::radians(look_pit)));
        front.y = (sin(glm::radians(look_pit)));
        front.z = (sin(glm::radians(look_yaw)) * cos(glm::radians(look_pit)));
        front = glm::normalize(front);

        /*
         * rightVector = frontVector X globalUpVector
         */
        right = glm::normalize(glm::cross(front, world_up));

        /*
         * camUpVector = frontVector X rightVector
         */
        cam_up = glm::normalize(glm::cross(right, front));
}

glm::mat4 scramble::camera::view_mat()
{
        return glm::lookAt(position, position + front, cam_up);
}
