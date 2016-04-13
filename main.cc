#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "objects.hh"
#include "platform.hh"
#include "unscramble.hh"
#include "shader_loader.hh"
#include "shader_linker.hh"

/*
 * Globals and constants
 */
const             GLuint WINSIZEW = 800; // Window size W
const             GLuint WINSIZEH = 600; // Window size H

GLFWwindow        *gwindow = nullptr;    // Context window
scramble::program *program = nullptr;    // Shader program

GLuint             vbo = 0;              // Vert buffer object
GLuint             ebo = 0;              // Elmt buffer object
GLuint             vao = 0;              // Vert array object

/*
 * Callback: keypress function
 */
void callback_key(GLFWwindow *gwindow, int key, int scan, int action, int mode)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(gwindow, GL_TRUE);
}

/*
 * Callback: on error function
 */
void callback_err(int code, const char *msg)
{
        throw std::runtime_error(msg);
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
        glfwSetKeyCallback(gwindow, callback_key);
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

        // Set viewport
        glViewport(0, 0, WINSIZEW, WINSIZEH);
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
 * Render full scene
 */
inline void render()
{
        // Set clear color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the program
        glUseProgram(program->get());

        // Bind VAO
        glBindVertexArray(vao);

        // Draw object
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbind VAO
        glBindVertexArray(0);

        // Unbind program
        glUseProgram(0);
}

/*
 * Main procedure
 */
int main(int argc, char *argv[])
{
        // Initialize GLFW
        engage_glfw();

        // Initialize Glew
        engage_glew();

        // Debug info
        std::cout << "OpenGL version: " <<
                glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " <<
                glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "Renderer: " <<
                glGetString(GL_RENDERER) << std::endl;
        std::cout << "Vendor: " <<
                glGetString(GL_VENDOR) << std::endl;

        // Initialize program
        try {
                engage_prog();

        } catch (std::exception& e) {

                std::cerr << "Error: " << e.what() << std::endl;
                return EXIT_FAILURE;
        }

        // Load the damn triangle
        scramble::load_square(vao, ebo, vbo);

        // Rendering loop (don't edit this)
        while (!glfwWindowShouldClose(gwindow)) {

                // Process events
                glfwPollEvents();

                // Render procedure
                render();

                // Off-screen to on-screen
                glfwSwapBuffers(gwindow);
        }

        // Destroy vertex containers
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);

        // Destroy GLFW window
        glfwDestroyWindow(gwindow);

        // Free context
        glfwTerminate();

        return EXIT_SUCCESS;
}