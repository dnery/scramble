//
// Created by danilo on 4/16/16.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "viewer.hh"

scramble::viewer::viewer() :
        position(glm::vec3(0.0f, 0.0f, 3.0f)),
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        up(glm::vec3(0.0f, 1.0f, 0.0f)),
        input_keys{},
        delta(0.0f), camspeed(0.0f),
        lastframe(0.0f), currentframe(0.0f)
{
}

scramble::viewer::~viewer()
{
}

void scramble::viewer::update()
{
        currentframe = glfwGetTime();
        delta = currentframe - lastframe;
        lastframe = currentframe;
}

void scramble::viewer::move()
{
        camspeed = 5.0f * delta;

        if (input_keys[GLFW_KEY_W] == true)
                position += camspeed * front;
        if (input_keys[GLFW_KEY_S] == true)
                position -= camspeed * front;
        if (input_keys[GLFW_KEY_A] == true)
                position -= glm::normalize(glm::cross(front, up)) * camspeed;
        if (input_keys[GLFW_KEY_D] == true)
                position += glm::normalize(glm::cross(front, up)) * camspeed;
}