#include "window_manager.hh"

#include "debug.hh"

window_manager::window_manager(int width, int height,
                const char *title, scene_type *scene) :
        m_width(width), m_height(height),
        m_title(title), m_window(nullptr),
        m_scene(scene)
{
        // Initialize GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        //glfwWindowHint(GLFW_SAMPLES, 8);

        // Build window
        m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

        // Check, set context
        check(m_window == nullptr);
        glfwMakeContextCurrent(m_window);

        // Set cursor input mode
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Set window element callbacks
        glfwSetWindowUserPointer(m_window, this);
        glfwSetWindowSizeCallback(m_window, callback_window_resize);
        glfwSetWindowRefreshCallback(m_window, callback_window_refresh);

        // Set device input handling callbacks
        glfwSetKeyCallback(m_window, callback_keypress);
        glfwSetScrollCallback(m_window, callback_scroll);
        glfwSetCursorPosCallback(m_window, callback_cursor_move);

        // Swap Interval != 0 might avoid tearing
        glfwSwapInterval(1);

        // Initialize GLEW
        glewExperimental = GL_TRUE;
        check(glewInit() != GLEW_OK);
        check(GLEW_VERSION_3_3 == 0);

        // Define viewport dimensions
        glViewport(0, 0, m_width, m_height);

        // Output debug info
        put("\n=======\n");
        put("CONTEXT\n\n");
        info("OpenGL version: %s\n", glGetString(GL_VERSION));
        info("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        info("Renderere: %s\n", glGetString(GL_RENDERER));
        info("Vendor: %s\n", glGetString(GL_VENDOR));
}

window_manager::~window_manager()
{
        glfwSetWindowUserPointer(m_window, m_window);
        glfwDestroyWindow(m_window);
        glfwTerminate();
}

void window_manager::viewport_did_resize(int width, int height)
{
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
}

void window_manager::render_scene()
{
        // Prepare scene
        m_scene->init(m_window);

        while (glfwWindowShouldClose(m_window) == 0) {

                // Process inut events with callbacks
                glfwPollEvents();

                // Obtain current clocktime
                GLdouble millis = static_cast<GLdouble>(glfwGetTime());

                // Update procedure
                m_scene->update(millis);

                // Render procedure
                m_scene->display();

                // Off-screen to on-screen
                glfwSwapBuffers(m_window);
        }

        // Drop scene
        m_scene->drop();
}
