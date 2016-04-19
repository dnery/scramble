//
// Created by danilo on 4/17/16.
//

#include "engine.hh"                                        // Defines this

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "ext/unscramble.hh"
#include "ext/platform.hh"
#include "listener.hh"
#include "camera.hh"
#include "shader.hh"

scramble::engine::engine() :
        window(),
        reps {
                glm::vec3(0.0f,  0.0f,    0.0f),
                glm::vec3(2.0f,  5.0f,  -15.0f),
                glm::vec3(-1.5f, -2.2f,   -2.5f),
                glm::vec3(-3.8f, -2.0f,  -12.3f),
                glm::vec3(2.4f, -0.4f,   -3.5f),
                glm::vec3(-1.7f,  3.0f,   -7.5f),
                glm::vec3(1.3f, -2.0f,   -2.5f),
                glm::vec3(1.5f,  2.0f,   -2.5f),
                glm::vec3(1.5f,  0.2f,   -1.5f),
                glm::vec3(-1.3f,  1.0f,   -1.5f),
                glm::vec3(-2.0f, -5.0f,   15.0f),
                glm::vec3(1.5f,  2.2f,    2.5f),
                glm::vec3(3.8f,  2.0f,   12.3f),
                glm::vec3(-2.4f,  0.4f,    3.5f),
                glm::vec3(1.7f, -3.0f,    7.5f),
                glm::vec3(-1.3f,  2.0f,    2.5f),
                glm::vec3(-1.5f, -2.0f,    2.5f),
                glm::vec3(-1.5f, -0.2f,    1.5f),
                glm::vec3(1.3f, -4.0f,    1.5f),
                glm::vec3(5.3f, -1.0f,   -1.5f)
            }
{
        /*
         * GL capabilities setup
         */
        glewExperimental = GL_TRUE;
        unsc_assert(glewInit() == GLEW_OK);
        unsc_assert(GLEW_VERSION_3_3 != 0);

        glViewport(0, 0, scramble::WIN_WIDTH, scramble::WIN_HEIGHT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /*
         * Callback functions
         */
        glfwSetErrorCallback(scramble::callback_err);
        glfwSetKeyCallback(window._handle(), scramble::callback_keyboard);
        glfwSetScrollCallback(window._handle(), scramble::callback_scroll);
        glfwSetCursorPosCallback(window._handle(), scramble::callback_mouse);

        /*
         * Create samples
         */
        std::vector<scramble::shader> shaders;
        shaders.push_back(scramble::shader_from_file(
                resource_path("vert_simple.glsl"), GL_VERTEX_SHADER));
        shaders.push_back(scramble::shader_from_file(
                resource_path("frag_simple.glsl"), GL_FRAGMENT_SHADER));

        program = new scramble::program(shaders);
        cube = new scramble::cube();
}

scramble::engine::~engine()
{
        delete program;
        delete cube;
}

void scramble::engine::engage()
{
        while (glfwWindowShouldClose(window._handle()) == 0) {

                // Process events
                glfwPollEvents();

                // Update procedure
                update();

                // Render procedure
                render();

                // Off-screen to on-screen
                glfwSwapBuffers(window._handle());
        }
}

void scramble::engine::update()
{
        scramble::refresh_delta();
        scramble::refresh_keyput();
}

void scramble::engine::render()
{
        // CLEAR
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);

        // BIND
        program->toggle();
        cube->bind(program);

        // VIEW MATRIX
        glm::mat4 view;
        view = scramble::camera.view_mat();
        program->setUniform("view", view);                  // Thanks TDogl!

        // PROJECTION MATRIX
        glm::mat4 proj;
        proj = glm::perspective(scramble::camera.look_zoom,
                                scramble::ASPECT_RATIO,
                                0.1f, 1000.0f);
        program->setUniform("proj", proj);                  // Thanks TDogl!

        // MODEL MATRIX
        for (GLuint i = 0; i < 20; i++) {

                glm::mat4 model;
                model = glm::translate(model, reps[i]);
                model = glm::rotate(model, (GLfloat)glfwGetTime() *
                                    glm::radians(50.0f) + i,
                                    glm::vec3(1.0f, 1.0f, 0.0f));
                program->setUniform("model", model);        // Thanks TDogl!

                // Draw cube
                cube->draw();
        }

        // UNBIND
        cube->unbind();
        program->toggle();
}
