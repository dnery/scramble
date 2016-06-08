#include "camera.hh"
#include <glm/gtc/matrix_transform.hpp>

const GLdouble FOV = 45.0f;              // default fov
const GLdouble YAW = -90.0f;             // default yaw
const GLdouble PITCH = 0.0f;             // default pitch
const GLdouble SPEED = 5.0f;             // default movespeed
const GLdouble SENSITIVITY = 0.1f;       // default sensitivity

camera::camera() :
        /*
         * Vector member data
         */
        m_vec_position(glm::vec3(0.0f, 0.0f, 3.0f)),
        m_vec_front(glm::vec3(0.0f, 0.0f, -1.0f)),
        m_vec_right(glm::vec3(1.0f, 0.0f, 0.0f)),
        m_vec_cam_up(glm::vec3(0.0f, 1.0f, 0.0f)),
        m_vec_world_up(glm::vec3(0.0f, 1.0f, 0.0f)),
        /*
         * Scalar member data
         */
        m_scalar_fov(FOV),
        m_scalar_yaw(YAW),
        m_scalar_pitch(PITCH),
        m_scalar_movespeed(SPEED),
        m_scalar_sensitivity(SENSITIVITY)
{
}

void camera::zoom(GLdouble yoffset)
{
        m_scalar_fov -= (yoffset * m_scalar_sensitivity);
        m_scalar_fov = (m_scalar_fov < 1.0f ? 1.0f : m_scalar_fov);
        m_scalar_fov = (m_scalar_fov > 90.0f ? 90.0f : m_scalar_fov);
}

void camera::move(GLuint direction, GLdouble delta)
{
        GLfloat final_speed = m_scalar_movespeed * delta;

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
                m_vec_position -= m_vec_right * final_speed;

        if (direction == movement::RIGHT)
                m_vec_position += m_vec_right * final_speed;

        if (direction == movement::FORWARD)
                m_vec_position += m_vec_front * final_speed;

        if (direction == movement::BACKWARD)
                m_vec_position -= m_vec_front * final_speed;
}

void camera::look(GLdouble xoffset, GLdouble yoffset, GLboolean clamp)
{
        xoffset *= m_scalar_sensitivity;
        yoffset *= m_scalar_sensitivity;

        m_scalar_yaw += xoffset;
        m_scalar_pitch += yoffset;

        if (clamp) {
                m_scalar_pitch = (m_scalar_pitch > 89.0f ? 89.0f : m_scalar_pitch);
                m_scalar_pitch = (m_scalar_pitch < -89.0f ? -89.0f : m_scalar_pitch);
        }

        update();
}

void camera::update()
{
        /*
         * Re-calculate front vector TODO sketch this procedure!
         */
        m_vec_front.x = (cos(glm::radians(m_scalar_yaw)) *
                        cos(glm::radians(m_scalar_pitch)));

        m_vec_front.y = (sin(glm::radians(m_scalar_pitch)));

        m_vec_front.z = (sin(glm::radians(m_scalar_yaw)) *
                        cos(glm::radians(m_scalar_pitch)));

        m_vec_front = glm::normalize(m_vec_front);

        /*
         * rightVector = frontVector X globalUpVector
         */
        m_vec_right = glm::normalize(glm::cross(m_vec_front, m_vec_world_up));

        /*
         * camUpVector = frontVector X rightVector
         */
        m_vec_cam_up = glm::normalize(glm::cross(m_vec_right, m_vec_front));
}

glm::mat4 camera::view()
{
        return glm::lookAt(m_vec_position, m_vec_position + m_vec_front, m_vec_cam_up);
}

glm::mat4 camera::projection(GLdouble vp_aspect_ratio)
{
        GLfloat var = static_cast<GLfloat>(vp_aspect_ratio);
        GLfloat fov = static_cast<GLfloat>(m_scalar_fov);

        return glm::perspective(fov, var, 0.1f, 1000.0f);
}

glm::mat4 camera::view_projection(GLdouble vp_aspect_ratio)
{
        return projection(vp_aspect_ratio) * view();
}
