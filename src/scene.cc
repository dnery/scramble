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
        m_object_program(nullptr)
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
        glClearColor(0.1f, 0.02f, 0.05f, 1.0f);

        // Z-buffer depth query
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Backface poly culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Alpha channel blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // On-Screen MSAA
        //glEnable(GL_MULTISAMPLE);

        // Create sample shaders
        try {
                put("\n==============\n");
                put("ASSETS: MODELS\n\n");
                //m_object = new model(resource_path("models/jeanity/LabComLoop.obj"));
                m_object = new model(resource_path("models/nanosuit/nanosuit.obj"));

                put("\n===============\n");
                put("ASSETS: SHADERS\n\n");
                std::vector<shader> object_shaders;
                object_shaders.push_back(shader_from_file(
                                        resource_path("glsl/vert_simple.glsl"),
                                        GL_VERTEX_SHADER)
                                );
                object_shaders.push_back(shader_from_file(
                                        resource_path("glsl/frag_simple.glsl"),
                                        GL_FRAGMENT_SHADER)
                                );

                m_object_program = new program(object_shaders);

        } catch (std::runtime_error &e) {

                throw std::runtime_error(
                                std::string("Model or Shader: ") +
                                e.what()
                                );
        }
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

        // Declarations
        glm::mat4 model_matrix;
        glm::mat4 normal_matrix;
        glm::vec3 caster_positions[] = {
                glm::vec3(2.3f, -1.6f, -3.0f),
                //glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(-1.7f, 0.9f, 1.0f),
                //glm::vec3(10.0f, 1.0f, -10.0f),
        };

        // OBJECT SHADER
        m_object_program->toggle();

        /*
         * VERTEX SHADER UNIFORMS
         */
        // view & projection matrix
        m_object_program->setUniform("view_matrix", m_camera.view());
        m_object_program->setUniform("proj_matrix", m_camera.projection(m_vp_aspect_ratio));

        // model matrix
        model_matrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -1.75f, 0.0f));
        //model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::scale(model_matrix, glm::vec3(0.2f, 0.2f, 0.2f));
        m_object_program->setUniform("model_matrix", model_matrix);

        // normal matrix
        normal_matrix = glm::transpose(glm::inverse(model_matrix));
        m_object_program->setUniform("normal_matrix", normal_matrix);

        /*
         * FRAGMENT SHADER UNIFORMS
         */
        // Set the camera position
        m_object_program->setUniform("viewer_pos", m_camera.position());

        // Flashlight properties
        m_object_program->setUniform("spotlights[0].position", m_camera.position());
        m_object_program->setUniform("spotlights[0].direction", m_camera.front());
        m_object_program->setUniform("spotlights[0].mincutoff", glm::cos(glm::radians(12.5f)));
        m_object_program->setUniform("spotlights[0].maxcutoff", glm::cos(glm::radians(17.5f)));

        if (m_keyboard.m_flashlight_active) {
                m_object_program->setUniform("spotlights[0].ambient", glm::vec3(0.05f));
                m_object_program->setUniform("spotlights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
                m_object_program->setUniform("spotlights[0].specular", glm::vec3(1.0f));
        } else {
                m_object_program->setUniform("spotlights[0].ambient", glm::vec3(0.0f));
                m_object_program->setUniform("spotlights[0].diffuse", glm::vec3(0.0f));
                m_object_program->setUniform("spotlights[0].specular", glm::vec3(0.0f));
        }

        // Light caster 1 properties
        m_object_program->setUniform("pointlights[0].position", caster_positions[0]);
        m_object_program->setUniform("pointlights[0].ambient", glm::vec3(0.05f));
        m_object_program->setUniform("pointlights[0].diffuse", glm::vec3(0.8f, 0.8f, 1.0f));
        m_object_program->setUniform("pointlights[0].specular", glm::vec3(1.0f));
        m_object_program->setUniform("pointlights[0].constant", 1.0f);
        m_object_program->setUniform("pointlights[0].linear", 0.009f);
        m_object_program->setUniform("pointlights[0].quadratic", 0.0032f);

        // Light caster 2 properties
        m_object_program->setUniform("pointlights[1].position", caster_positions[1]);
        m_object_program->setUniform("pointlights[1].ambient", glm::vec3(0.05f));
        m_object_program->setUniform("pointlights[1].diffuse", glm::vec3(1.0f, 1.0f, 0.8f));
        m_object_program->setUniform("pointlights[1].specular", glm::vec3(1.0f));
        m_object_program->setUniform("pointlights[1].constant", 1.0f);
        m_object_program->setUniform("pointlights[1].linear", 0.009f);
        m_object_program->setUniform("pointlights[1].quadratic", 0.0032f);

        // Draw
        m_object->draw(*m_object_program);

        // Done
        m_object_program->toggle();
}

void scene::drop()
{
        delete m_object_program;
        delete m_object;
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
                        m_mouse.m_prev_ypos - yoffset); // Y delta

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
