#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

// GL handling
#include <GL/glew.h>                                        // GL Ext Wrangler
#include <SOIL/SOIL.h>                                      // SO Image loader
#include <GLFW/glfw3.h>                                     // Window & events

// Matrix transforms
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Project locals
#include "camera.hh"
#include "objects.hh"
#include "platform.hh"
#include "unscramble.hh"
#include "shader_loader.hh"
#include "shader_linker.hh"

/*
 * Window & program
 */
const GLuint WINSIZEW = 800;                                // Window size W
const GLuint WINSIZEH = 600;                                // Window size H
GLFWwindow        *gwindow = nullptr;                       // Context window
scramble::program *program = nullptr;                       // Shader program

/*
 * Camera related stuff
 */
scramble::camera camera;
GLfloat          lastxpos = WINSIZEW / 2.0f;                // last x look
GLfloat          lastypos = WINSIZEH / 2.0f;                // last y look

/*
 * Input & normalizations
 */
bool    keys_in[1024];                                      // GLFW key bitmap
bool    mouse_in = true;                                    // First mouse input
GLfloat delta_time = 0.0f;                                  // last frame time diff
GLfloat last_frame = 0.0f;                                  // last frame timestamp

/*
 * Sample object & instances
 */
scramble::cube *cube;
glm::vec3 reps[] = {
        glm::vec3( 0.0f,  0.0f,    0.0f),
        glm::vec3( 2.0f,  5.0f,  -15.0f),
        glm::vec3(-1.5f, -2.2f,   -2.5f),
        glm::vec3(-3.8f, -2.0f,  -12.3f),
        glm::vec3( 2.4f, -0.4f,   -3.5f),
        glm::vec3(-1.7f,  3.0f,   -7.5f),
        glm::vec3( 1.3f, -2.0f,   -2.5f),
        glm::vec3( 1.5f,  2.0f,   -2.5f),
        glm::vec3( 1.5f,  0.2f,   -1.5f),
        glm::vec3(-1.3f,  1.0f,   -1.5f)
};

/*
 * Callback: on error function
 */
void callback_err(int code, const char *msg)
{
        throw std::runtime_error(msg);
}

/*
 * Callback: keypress function
 */
void callback_keys(GLFWwindow *gwindow, int key, int scan, int action, int mode)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(gwindow, GL_TRUE);

        if (key >= 0 && key < 1024) {

                if (action == GLFW_PRESS)
                        keys_in[key] = true;

                else if (action == GLFW_RELEASE)
                        keys_in[key] = false;
        }
}

/*
 * Callback: mouse-input function
 */
void callback_mouse(GLFWwindow *gwindow, double xpos, double ypos)
{
        if (mouse_in == true) {
                lastxpos = xpos;
                lastypos = ypos;
                mouse_in = false;
        }

        camera.mouse_look(xpos - lastxpos, lastypos - ypos);

        lastxpos = xpos;
        lastypos = ypos;
}

/*
 * Callback: mouse-scroll function
 */
void callback_scroll(GLFWwindow *gwindow, double xoffset, double yoffset)
{
        camera.mouse_scroll(yoffset);
}

/*
 * Fully initialize GLFW
 */
void engage_glfw()
{
        glfwInit();

        // Core profile critical
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Create GLFW context window
        gwindow = glfwCreateWindow(WINSIZEW, WINSIZEH, "Scramble",
                nullptr, nullptr);

        // Check return, set context
        unsc_assert(gwindow != nullptr);
        glfwMakeContextCurrent(gwindow);

        // Set event callbacks
        glfwSetErrorCallback(callback_err);
        glfwSetKeyCallback(gwindow, callback_keys);
        glfwSetScrollCallback(gwindow, callback_scroll);
        glfwSetCursorPosCallback(gwindow, callback_mouse);

        // Set input modes
        glfwSetInputMode(gwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/*
 * Fully initialize Glew
 */
void engage_glew()
{
        // Glew config
        glewExperimental = GL_TRUE;
        unsc_assert(glewInit() == GLEW_OK);
        unsc_assert(GLEW_VERSION_3_3 != 0);

        // GLsetup: viewport
        glViewport(0, 0, WINSIZEW, WINSIZEH);

        // GLsetup: depthtesting
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // GLsetup: alphablending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void engage_prog()
{
        std::vector<scramble::shader> shaders;

        // Build the shader vector
        shaders.push_back(scramble::shader_from_file(
                resource_path("vert_simple.glsl"), GL_VERTEX_SHADER));
        shaders.push_back(scramble::shader_from_file(
                resource_path("frag_simple.glsl"), GL_FRAGMENT_SHADER));

        // Create the shader program
        program = new scramble::program(shaders);
}

/*
 * Update state
 */
inline void update()
{
        GLfloat curr_frame = glfwGetTime();                 // Frame timestamp

        delta_time = curr_frame - last_frame;
        last_frame = curr_frame;

        if (keys_in[GLFW_KEY_A] == true)
                camera.keyboard(scramble::movement::LEFT, delta_time);
        if (keys_in[GLFW_KEY_D] == true)
                camera.keyboard(scramble::movement::RIGHT, delta_time);
        if (keys_in[GLFW_KEY_W] == true)
                camera.keyboard(scramble::movement::FORWARD, delta_time);
        if (keys_in[GLFW_KEY_S] == true)
                camera.keyboard(scramble::movement::BACKWARD, delta_time);
}

/*
 * Render state
 */
inline void render()
{
        // set clear color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);

        // Bind the program
        program->toggle();

        // Bind cube
        cube->bind(program);

        // VIEW MATRIX
        glm::mat4 view;
        view = camera.view_mat();
        glUniformMatrix4fv(program->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

        // PROJECTION MATRIX
        glm::mat4 proj;
        proj = glm::perspective(camera.look_zoom, static_cast<float>(WINSIZEW) / static_cast<float>(WINSIZEH), 0.1f, 100.0f);
        glUniformMatrix4fv(program->uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));

        // MODEL MATRIX
        for (GLuint i = 0; i < 10; i++) {

                glm::mat4 model;
                model = glm::translate(model, reps[i]);
                model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f) + i, glm::vec3(1.0f, 1.0f, 0.0f));
                glUniformMatrix4fv(program->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));

                // Draw cube
                cube->draw();
        }

        // Unbind cube
        cube->unbind();

        // Unbind program
        program->toggle();
}

/*
 * Main procedure
 */
int main(int argc, char *argv[])
{
        // Initialize program
        try {
                engage_glfw();
                engage_glew();
                engage_prog();

        } catch (std::exception& e) {

                std::cerr << "Error: " << e.what() << std::endl;
                return EXIT_FAILURE;
        }

        // Sample object
        cube = new scramble::cube();

        // Debug info
        std::cout << "OpenGL version: " <<
                glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " <<
                glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "Renderer: " <<
                glGetString(GL_RENDERER) << std::endl;
        std::cout << "Vendor: " <<
                glGetString(GL_VENDOR) << std::endl;

        // Rendering loop (don't edit this)
        while (glfwWindowShouldClose(gwindow) == 0) {

                // Process events
                glfwPollEvents();

                // Update procedure
                update();

                // Render procedure
                render();

                // Off-screen to on-screen
                glfwSwapBuffers(gwindow);
        }

        // Destroy GLFW window
        glfwDestroyWindow(gwindow);

        // Free context
        glfwTerminate();

        return EXIT_SUCCESS;
}