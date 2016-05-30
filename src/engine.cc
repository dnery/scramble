/*
 * Created by danilo on 4/17/16.
 */

#include <iostream>
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>
#include "debug.hh"
#include "camera.hh"
#include "engine.hh"
#include "locator.tt"
#include "listener.hh"
#include "ext/platform.hh"

/*
 * Abbreviations
 */
using camService = scramble::locator<scramble::camera>;

scramble::engine::engine() : window()
{
        // GL/GLEW capabilities setup
        glewExperimental = GL_TRUE;
        check(glewInit() == GLEW_OK);
        check(GLEW_VERSION_3_3 != 0);

        // Handle viewport dimensions
        glViewport(0, 0, scramble::WIN_WIDTH, scramble::WIN_HEIGHT);

        // Z-buffer depth query
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Backface poly culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Alpha channel blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Callback functions
        glfwSetErrorCallback(scramble::callback_err);
        glfwSetKeyCallback(window._handle(), scramble::callback_keyboard);
        glfwSetScrollCallback(window._handle(), scramble::callback_scroll);
        glfwSetCursorPosCallback(window._handle(), scramble::callback_mouse);

        // Print debug info
        window.debug_info();

        /*
         * Provide global services.
         *
         * Any global objects will be provided through the service-locator
         * pattern. The bottom line is: It brings (even though marginally)
         * some method to the madness of globals. Read more on the design
         * pattern here:
         *
         *      http://bit.ly/1q9BMUN
         *
         * This is also pertinent to the local implementation:
         *
         *      http://bit.ly/1Now6kY
         *
         * Looking at the local implementation, it might be immediately strange.
         * That is due to the fact that it is templated, and any service will be
         * imediatelly available upon a provide() call just like the ones below.
         */
        camService::provide(new scramble::camera());

        /*
         * Create hard-coded samples.
         *
         * This will catch a throw by the shader compiler object or the
         * shader linker object, append a specific message and re-throw.
         *
         * It's worth reminding that throwing an exception is the _only_
         * way to exit a failed constructor elegantly.
         */
        try {
                std::vector<scramble::shader> object_shaders;
                object_shaders.push_back(scramble::shader_from_file(
                                        resource_path("glsl/vert_simple.glsl"),
                                        GL_VERTEX_SHADER)
                                );
                object_shaders.push_back(scramble::shader_from_file(
                                        resource_path("glsl/frag_simple.glsl"),
                                        GL_FRAGMENT_SHADER)
                                );

                std::vector<scramble::shader> caster_shaders;
                caster_shaders.push_back(scramble::shader_from_file(
                                        resource_path("glsl/vert_caster.glsl"),
                                        GL_VERTEX_SHADER)
                                );
                caster_shaders.push_back(scramble::shader_from_file(
                                        resource_path("glsl/frag_caster.glsl"),
                                        GL_FRAGMENT_SHADER)
                                );

                object_program = new scramble::program(object_shaders);
                caster_program = new scramble::program(caster_shaders);

        } catch (std::runtime_error &e) {

                throw std::runtime_error(
                                std::string("Shader compilation/linkage: ") +
                                e.what()
                                );
        }

        // Objects do not depend on shaders for creation, only drawing
        cube = new scramble::cube;
        lamp = new scramble::cube;
}

scramble::engine::~engine()
{
        delete object_program;
        delete caster_program;
        delete cube;
        delete lamp;
}

void scramble::engine::engage()
{
        while (glfwWindowShouldClose(window._handle()) == 0) {

                /*
                 * Process user input events.
                 *
                 * Initially, the callbacks set on the constructor are called.
                 * Those are all implemented as free functions also in the
                 * 'scramble' namespace, specifically in the listener.cc unit.
                 */
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
        glClearColor(0.25f, 0.03f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // PREAMBLE
        // Static proj matrix
        glm::mat4 proj;
        proj = glm::perspective(camService::current()->look_zoom,
                        scramble::ASPECT_RATIO,
                        0.1f, 1000.0f);

        // Static view matrix
        glm::mat4 view;
        view = camService::current()->view_mat();

        // TODO Model matrix changes!
        glm::mat4 model;

        // TODO Normal matrix changes!
        glm::mat4 normat;

        // CASTER SHADER
        caster_program->toggle();
        lamp->bind(caster_program);

        // Set static properties
        caster_program->setUniform("proj", proj);
        caster_program->setUniform("view", view);
        caster_program->setUniform("color", glm::vec3(1.0f, 0.6f, 0.8f));

        // Define draw positions
        glm::vec3 caster_positions[] = {
                glm::vec3(0.0f, 3.3f, 0.0f),
                glm::vec3(3.0f, 2.0f, -8.5f),
                glm::vec3(-4.0f, -2.5f, 7.0f)
        };

        // Draw all instances
        for (GLuint i = 0; i < 3; i++) {

                // Model matrix
                model = glm::translate(glm::mat4(), caster_positions[i]);
                caster_program->setUniform("model", model);

                // Draw :D
                lamp->draw();
        }

        // Done
        lamp->unbind();
        caster_program->toggle();

        // OBJECT SHADER
        object_program->toggle();
        cube->bind(object_program);

        // Set static properties
                // Set object material properties
        object_program->setUniform("material.shininess", 64.0f);

                // Flashlight properties
        object_program->setUniform("spotlights[0].position", camService::current()->position);
        object_program->setUniform("spotlights[0].direction", camService::current()->front);
        object_program->setUniform("spotlights[0].mincutoff", glm::cos(glm::radians(12.5f)));
        object_program->setUniform("spotlights[0].maxcutoff", glm::cos(glm::radians(17.5f)));

        if (flashlight) {
                object_program->setUniform("spotlights[0].ambient", glm::vec3(0.1f));
                object_program->setUniform("spotlights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
                object_program->setUniform("spotlights[0].specular", glm::vec3(1.0f));
        } else {
                object_program->setUniform("spotlights[0].ambient", glm::vec3(0.0f));
                object_program->setUniform("spotlights[0].diffuse", glm::vec3(0.0f));
                object_program->setUniform("spotlights[0].specular", glm::vec3(0.0f));
        }

                // Light caster 1 properties
        object_program->setUniform("pointlights[0].position", caster_positions[0]);
        object_program->setUniform("pointlights[0].ambient", glm::vec3(0.1f));
        object_program->setUniform("pointlights[0].diffuse", glm::vec3(1.0f, 0.6f, 0.8f));
        object_program->setUniform("pointlights[0].specular", glm::vec3(1.0f));
        object_program->setUniform("pointlights[0].constant", 1.0f);
        object_program->setUniform("pointlights[0].linear", 0.14f);
        object_program->setUniform("pointlights[0].quad", 0.07f);

                // Light caster 2 properties
        object_program->setUniform("pointlights[1].position", caster_positions[1]);
        object_program->setUniform("pointlights[1].ambient", glm::vec3(0.1f));
        object_program->setUniform("pointlights[1].diffuse", glm::vec3(1.0f, 0.6f, 0.8f));
        object_program->setUniform("pointlights[1].specular", glm::vec3(1.0f));
        object_program->setUniform("pointlights[1].constant", 1.0f);
        object_program->setUniform("pointlights[1].linear", 0.14f);
        object_program->setUniform("pointlights[1].quad", 0.07f);

                // Light caster 3 properties
        object_program->setUniform("pointlights[2].position", caster_positions[2]);
        object_program->setUniform("pointlights[2].ambient", glm::vec3(0.1f));
        object_program->setUniform("pointlights[2].diffuse", glm::vec3(1.0f, 0.6f, 0.8f));
        object_program->setUniform("pointlights[2].specular", glm::vec3(1.0f));
        object_program->setUniform("pointlights[2].constant", 1.0f);
        object_program->setUniform("pointlights[2].linear", 0.14f);
        object_program->setUniform("pointlights[2].quad", 0.07f);

                // Viewer properties
        object_program->setUniform("proj", proj);
        object_program->setUniform("view", view);
        object_program->setUniform("viewer_pos", camService::current()->position);

        // Define draw positions
        glm::vec3 object_positions[] = {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f, 3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f, 2.0f, -2.5f),
                glm::vec3(1.5f, 0.2f, -1.5f),
                glm::vec3(-1.3f, 1.0f, -1.5f),
                glm::vec3(-2.0f, -5.0f, 15.0f),
                glm::vec3(1.5f, 2.2f, 2.5f),
                glm::vec3(3.8f, 2.0f, 12.3f),
                glm::vec3(-2.4f, 0.4f, 3.5f),
                glm::vec3(1.7f, -3.0f, 7.5f),
                glm::vec3(-1.3f, 2.0f, 2.5f),
                glm::vec3(-1.5f, -2.0f, 2.5f),
                glm::vec3(-1.5f, -0.2f, 1.5f),
                glm::vec3(1.3f, -4.0f, 1.5f),
                glm::vec3(5.3f, -1.0f, -1.5f)
        };

        // Draw all instances
        for (GLuint i = 0; i < 20; i++) {

                // Model matrix
                model = glm::translate(glm::mat4(), object_positions[i]);
                //model = glm::rotate(model, (GLfloat) (glfwGetTime() * 0.9f + i),
                //              glm::vec3(1.0f, 1.0f, 0.0f));
                object_program->setUniform("model", model);

                // Normal matrix
                normat = glm::transpose(glm::inverse(model));
                object_program->setUniform("normat", normat);

                // Draw :D
                cube->draw();
        }

        // Done
        cube->unbind();
        object_program->toggle();
}
