#include "scene.hh"

#include "ext/platform.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include "debug.hh"

scene::scene() :
        /*
         * Input
         */
        m_mouse(),
        m_camera(),
        m_keyboard(),
        /*
         * Clock
         */
        m_delta(),
        m_prev_frame(),
        m_vp_aspect_ratio(),
        /*
         * Elements
         */
        m_object(nullptr),
        m_caster(nullptr),
        m_object_program(nullptr),
        m_caster_program(nullptr)
{
}

void scene::init(GLFWwindow *window)
{
        // Set viewport aspect ratio
        int w;
        int h;
        glfwGetWindowSize(window, &w, &h);
        m_vp_aspect_ratio = static_cast<GLdouble>(w) /
                            static_cast<GLdouble>(h);

        // Set up mouse input stuff
        m_mouse.m_first_enter_viewport = GL_TRUE;

        // Set the clear color
        glClearColor(0.25f, 0.03f, 0.08f, 1.0f);

        // Z-buffer depth query
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Backface poly culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Alpha channel blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Create sample shaders
        try {
                std::vector<shader> object_shaders;
                object_shaders.push_back(shader_from_file(
                                        resource_path("glsl/vert_simple.glsl"),
                                        GL_VERTEX_SHADER)
                                );
                object_shaders.push_back(shader_from_file(
                                        resource_path("glsl/frag_simple.glsl"),
                                        GL_FRAGMENT_SHADER)
                                );

                std::vector<shader> caster_shaders;
                caster_shaders.push_back(shader_from_file(
                                        resource_path("glsl/vert_caster.glsl"),
                                        GL_VERTEX_SHADER)
                                );
                caster_shaders.push_back(shader_from_file(
                                        resource_path("glsl/frag_caster.glsl"),
                                        GL_FRAGMENT_SHADER)
                                );

                m_object_program = new program(object_shaders);
                m_caster_program = new program(caster_shaders);

        } catch (std::runtime_error &e) {

                throw std::runtime_error(
                                std::string("Shader compilation/linkage: ") +
                                e.what()
                                );
        }

        // Objects do not depend on shaders for creation
        m_object = new cube;
        m_caster = new cube;
}

void scene::update(GLdouble millis)
{
        // Update delta step
        m_delta = millis - m_prev_frame;
        m_prev_frame = millis;

        // Propagate smooth keyboard input
        using movetype = camera::movement;

        if (m_keyboard.m_keymap[GLFW_KEY_A])
                m_camera.move(movetype::LEFT, m_delta);
        if (m_keyboard.m_keymap[GLFW_KEY_D])
                m_camera.move(movetype::RIGHT, m_delta);
        if (m_keyboard.m_keymap[GLFW_KEY_W])
                m_camera.move(movetype::FORWARD, m_delta);
        if (m_keyboard.m_keymap[GLFW_KEY_S])
                m_camera.move(movetype::BACKWARD, m_delta);
}

void scene::display()
{
        // CLEAR
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // PREAMBLE
        glm::mat4 model_matrix;
        glm::mat4 normal_matrix;

        // CASTER SHADER
        m_caster_program->toggle();
        m_caster->bind(m_caster_program);

        // Set static properties
        m_caster_program->setUniform("proj", m_camera.projection(m_vp_aspect_ratio));
        m_caster_program->setUniform("view", m_camera.view());
        m_caster_program->setUniform("color", glm::vec3(1.0f, 0.6f, 0.8f));

        // Define draw positions
        glm::vec3 caster_positions[] = {
                glm::vec3(0.0f, 3.3f, 0.0f),
                glm::vec3(3.0f, 2.0f, -8.5f),
                glm::vec3(-4.0f, -2.5f, 7.0f)
        };

        // Draw all instances
        for (GLuint i = 0; i < 3; i++) {

                // Model matrix
                model_matrix = glm::translate(glm::mat4(), caster_positions[i]);
                m_caster_program->setUniform("model", model_matrix);

                // Draw :D
                m_caster->draw();
        }

        // Done
        m_caster->unbind();
        m_caster_program->toggle();

        // OBJECT SHADER
        m_object_program->toggle();
        m_object->bind(m_object_program);

        // Set static properties

                // Set object material properties
        m_object_program->setUniform("material.shininess", 64.0f);

                // Flashlight properties
        m_object_program->setUniform("spotlights[0].position", m_camera.position());
        m_object_program->setUniform("spotlights[0].direction", m_camera.front());
        m_object_program->setUniform("spotlights[0].mincutoff", glm::cos(glm::radians(12.5f)));
        m_object_program->setUniform("spotlights[0].maxcutoff", glm::cos(glm::radians(17.5f)));

        if (m_keyboard.m_flashlight_active) {
                m_object_program->setUniform("spotlights[0].ambient", glm::vec3(0.1f));
                m_object_program->setUniform("spotlights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
                m_object_program->setUniform("spotlights[0].specular", glm::vec3(1.0f));
        } else {
                m_object_program->setUniform("spotlights[0].ambient", glm::vec3(0.0f));
                m_object_program->setUniform("spotlights[0].diffuse", glm::vec3(0.0f));
                m_object_program->setUniform("spotlights[0].specular", glm::vec3(0.0f));
        }

                // Light caster 1 properties
        m_object_program->setUniform("pointlights[0].position", caster_positions[0]);
        m_object_program->setUniform("pointlights[0].ambient", glm::vec3(0.1f));
        m_object_program->setUniform("pointlights[0].diffuse", glm::vec3(1.0f, 0.6f, 0.8f));
        m_object_program->setUniform("pointlights[0].specular", glm::vec3(1.0f));
        m_object_program->setUniform("pointlights[0].constant", 1.0f);
        m_object_program->setUniform("pointlights[0].linear", 0.14f);
        m_object_program->setUniform("pointlights[0].quad", 0.07f);

                // Light caster 2 properties
        m_object_program->setUniform("pointlights[1].position", caster_positions[1]);
        m_object_program->setUniform("pointlights[1].ambient", glm::vec3(0.1f));
        m_object_program->setUniform("pointlights[1].diffuse", glm::vec3(1.0f, 0.6f, 0.8f));
        m_object_program->setUniform("pointlights[1].specular", glm::vec3(1.0f));
        m_object_program->setUniform("pointlights[1].constant", 1.0f);
        m_object_program->setUniform("pointlights[1].linear", 0.14f);
        m_object_program->setUniform("pointlights[1].quad", 0.07f);

                // Light caster 3 properties
        m_object_program->setUniform("pointlights[2].position", caster_positions[2]);
        m_object_program->setUniform("pointlights[2].ambient", glm::vec3(0.1f));
        m_object_program->setUniform("pointlights[2].diffuse", glm::vec3(1.0f, 0.6f, 0.8f));
        m_object_program->setUniform("pointlights[2].specular", glm::vec3(1.0f));
        m_object_program->setUniform("pointlights[2].constant", 1.0f);
        m_object_program->setUniform("pointlights[2].linear", 0.14f);
        m_object_program->setUniform("pointlights[2].quad", 0.07f);

                // Viewer properties
        m_object_program->setUniform("view", m_camera.view());
        m_object_program->setUniform("proj", m_camera.projection(m_vp_aspect_ratio));
        m_object_program->setUniform("viewer_pos", m_camera.position());

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
                model_matrix = glm::translate(glm::mat4(), object_positions[i]);
                model_matrix = glm::rotate(model_matrix, (GLfloat) (glfwGetTime() * 0.9f + i),
                                glm::vec3(1.0f, 1.0f, 0.0f));
                m_object_program->setUniform("model", model_matrix);

                // Normal matrix
                normal_matrix = glm::transpose(glm::inverse(model_matrix));
                m_object_program->setUniform("normat", normal_matrix);

                // Draw :D
                m_object->draw();
        }

        // Done
        m_object->unbind();
        m_object_program->toggle();
}

void scene::drop()
{
        delete m_object_program;
        delete m_caster_program;
        delete m_object;
        delete m_caster;
}

void scene::handle_cursor_move(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset)
{
        if (m_mouse.m_first_enter_viewport) {
                m_mouse.m_prev_xpos = static_cast<GLdouble>(xoffset);
                m_mouse.m_prev_ypos = static_cast<GLdouble>(yoffset);
                m_mouse.m_first_enter_viewport = GL_FALSE;
        }

        // New camera state
        m_camera.look(xoffset - m_mouse.m_prev_xpos,    // X delta
                      m_mouse.m_prev_ypos - yoffset);   // Y delta

        m_mouse.m_prev_xpos = static_cast<GLdouble>(xoffset);
        m_mouse.m_prev_ypos = static_cast<GLdouble>(yoffset);
}

void scene::handle_keypress(GLFWwindow *window, GLint key, GLint action, GLint mode)
{
        // Check for application exit event (escape)
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);

        // Flashlight toggle has to be sluggish
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
                if (!m_keyboard.m_flashlight_active)
                        m_keyboard.m_flashlight_active = GL_TRUE;
                else
                        m_keyboard.m_flashlight_active = GL_FALSE;
        }

        // Smooth input triggers
        if (key < 0 || key > 1023)
                return;
        else if (action == GLFW_PRESS)
                m_keyboard.m_keymap[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
                m_keyboard.m_keymap[key] = GL_FALSE;
}

void scene::handle_scroll(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset)
{
        m_camera.zoom(yoffset);
}
