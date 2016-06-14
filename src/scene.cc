#include "scene.hh"

#include "ext/platform.hh"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <random>
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
        glClearColor(0.1, 0.09, 0.08, 1.0f);

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
                m_object = new model(resource_path("models/room/room.obj"));

                put("\n===============\n");
                put("ASSETS: SHADERS\n\n");
                std::vector<shader> object_shaders;
                object_shaders.push_back(shader_from_file(
                                        resource_path("glsl/nmap.vert"),
                                        GL_VERTEX_SHADER)
                                );
                object_shaders.push_back(shader_from_file(
                                        resource_path("glsl/nmap.frag"),
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

        // OBJECT SHADER
        m_object_program->toggle();

        // Matrix transformations
        m_object_program->setUniform("proj_matrix", m_camera.projection(m_vp_aspect_ratio));
        m_object_program->setUniform("view_matrix", m_camera.view());
        model_matrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -1.75f, 0.0f));
        //model_matrix = glm::scale(model_matrix, glm::vec3(0.2f, 0.2f, 0.2f));
        //model_matrix = glm::rotate(model_matrix, (GLfloat)glfwGetTime() * 1.1f, glm::vec3(0.0f, 1.0f, 0.0f));
        m_object_program->setUniform("model_matrix", model_matrix);
        m_object_program->setUniform("normal_matrix", glm::transpose(glm::inverse(model_matrix)));

        // Unique viewer position
        m_object_program->setUniform("viewer_pos", m_camera.position());

        // Retrieve active states
        GLfloat rotating_omnis = static_cast<GLfloat>(m_keyboard.m_rotating_omnis);
        GLfloat global_omni = static_cast<GLfloat>(m_keyboard.m_global_omni);
        GLfloat center_omni = static_cast<GLfloat>(m_keyboard.m_center_omni);
        GLfloat flashlight = static_cast<GLfloat>(m_keyboard.m_flashlight);

        // Rotating Omnilight 1
        model_matrix = glm::rotate(glm::mat4(), (GLfloat)glfwGetTime() * 1.1f, glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, -7.0f));
        m_object_program->setUniform("omnilight[0].position", glm::vec3(model_matrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
        m_object_program->setUniform("omnilight[0].Ia", rotating_omnis * glm::vec3(0.05f));
        m_object_program->setUniform("omnilight[0].Id", rotating_omnis * glm::vec3(0.75f, 0.75f, 1.0f));
        m_object_program->setUniform("omnilight[0].Is", rotating_omnis * glm::vec3(1.0f, 1.0f, 1.0f));
        m_object_program->setUniform("omnilight[0].Kc", 1.0f);
        m_object_program->setUniform("omnilight[0].Kl", 0.22f);
        m_object_program->setUniform("omnilight[0].Kq", 0.20f);

        // Rotating Omnilight 2
        model_matrix = glm::rotate(glm::mat4(), (GLfloat)glfwGetTime() * 1.1f, glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f,  7.0f));
        m_object_program->setUniform("omnilight[1].position", glm::vec3(model_matrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
        m_object_program->setUniform("omnilight[1].Ia", rotating_omnis * glm::vec3(0.05f));
        m_object_program->setUniform("omnilight[1].Id", rotating_omnis * glm::vec3(0.75f, 1.0f, 0.75f));
        m_object_program->setUniform("omnilight[1].Is", rotating_omnis * glm::vec3(1.0f, 1.0f, 1.0f));
        m_object_program->setUniform("omnilight[1].Kc", 1.0f);
        m_object_program->setUniform("omnilight[1].Kl", 0.22f);
        m_object_program->setUniform("omnilight[1].Kq", 0.20f);

        // Global Omnilight
        m_object_program->setUniform("omnilight[2].position", glm::vec3(0.0f, 3.0f, 0.0f));
        m_object_program->setUniform("omnilight[2].Ia", global_omni * glm::vec3(0.05f));
        m_object_program->setUniform("omnilight[2].Id", global_omni * glm::vec3(1.0f, 1.0f, 1.0f));
        m_object_program->setUniform("omnilight[2].Is", global_omni * glm::vec3(1.0f, 1.0f, 1.0f));
        m_object_program->setUniform("omnilight[2].Kc", 1.0f);
        m_object_program->setUniform("omnilight[2].Kl", 0.07f);
        m_object_program->setUniform("omnilight[2].Kq", 0.017f);

        // Center Omnilight
        GLfloat pulse = glm::cos(glfwGetTime() * 1.5f) / 2.0f + 0.5f;
        m_object_program->setUniform("omnilight[3].position", glm::vec3(0.0f, 2.0f, 0.0f));
        m_object_program->setUniform("omnilight[3].Ia", pulse * center_omni * glm::vec3(0.05f));
        m_object_program->setUniform("omnilight[3].Id", pulse * center_omni * glm::vec3(1.0f, 0.65f, 0.65f));
        m_object_program->setUniform("omnilight[3].Is", pulse * center_omni * glm::vec3(1.0f, 1.0f, 1.0f));
        m_object_program->setUniform("omnilight[3].Kc", 1.0f);
        m_object_program->setUniform("omnilight[3].Kl", 0.14f);
        m_object_program->setUniform("omnilight[3].Kq", 0.07f);

        // Flashlight
        m_object_program->setUniform("spotlight[0].position", m_camera.position());
        m_object_program->setUniform("spotlight[0].front", m_camera.front());
        m_object_program->setUniform("spotlight[0].Ia", flashlight * glm::vec3(0.0f));
        m_object_program->setUniform("spotlight[0].Id", flashlight * glm::vec3(1.0f, 1.0f, 1.0f));
        m_object_program->setUniform("spotlight[0].Is", flashlight * glm::vec3(1.0f, 1.0f, 1.0f));
        m_object_program->setUniform("spotlight[0].Kc", 1.0f);
        m_object_program->setUniform("spotlight[0].Kl", 0.14f);
        m_object_program->setUniform("spotlight[0].Kq", 0.07f);
        m_object_program->setUniform("spotlight[0].mincutoff", glm::cos(glm::radians(1.5f)));
        m_object_program->setUniform("spotlight[0].maxcutoff", glm::cos(glm::radians(19.5f)));

        // Draw
        m_object->draw(*m_object_program);

        // Done
        m_object_program->toggle();
}

void scene::drop()
{
        delete m_object;
        delete m_object_program;
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

        // Sluggish toggle flashlight
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
                if (!m_keyboard.m_flashlight)
                        m_keyboard.m_flashlight = GL_TRUE;
                else
                        m_keyboard.m_flashlight = GL_FALSE;
        }

        // Sluggish toggle center omnilight
        if (key == GLFW_KEY_C && action == GLFW_PRESS) {
                if (!m_keyboard.m_center_omni)
                        m_keyboard.m_center_omni = GL_TRUE;
                else
                        m_keyboard.m_center_omni = GL_FALSE;
        }

        // Sluggish toggle global omnilight
        if (key == GLFW_KEY_G && action == GLFW_PRESS) {
                if (!m_keyboard.m_global_omni)
                        m_keyboard.m_global_omni = GL_TRUE;
                else
                        m_keyboard.m_global_omni = GL_FALSE;
        }

        // Sluggish toggle rotating omnilights
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
                if (!m_keyboard.m_rotating_omnis)
                        m_keyboard.m_rotating_omnis = GL_TRUE;
                else
                        m_keyboard.m_rotating_omnis = GL_FALSE;
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
