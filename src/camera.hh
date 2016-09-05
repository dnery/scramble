#ifndef SCRAMBLE_CAMERA_H
#define SCRAMBLE_CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>

extern const GLdouble FOV;                      // Default fov
extern const GLdouble YAW;                      // Default yaw
extern const GLdouble PITCH;                    // Default pitch
extern const GLdouble SPEED;                    // Default movespeed
extern const GLdouble SENSITIVITY;              // Default sensitivity

class camera {

        glm::vec3 m_vec_position;               // Position vector
        glm::vec3 m_vec_front;                  // Front normal
        glm::vec3 m_vec_right;                  // Right normal
        glm::vec3 m_vec_cam_up;                 // Cam up normal
        glm::vec3 m_vec_world_up;               // World up normal

        GLdouble m_scalar_fov;                  // Look zoom factor
        GLdouble m_scalar_yaw;                  // Look yaw rotation
        GLdouble m_scalar_pitch;                // Look pitch rotation
        GLdouble m_scalar_movespeed;            // Movement speed multiplier
        GLdouble m_scalar_sensitivity;          // Look sensitivity multiplier

        void update();

public:
        enum movement {
                LEFT = 1,                       // Move type: left
                RIGHT = 2,                      // Move type: right
                FORWARD = 4,                    // Move type: forward
                BACKWARD = 8                    // Move type: backward
        };

        camera();

        const glm::vec3& position() const
        {
                return m_vec_position;
        }

        const glm::vec3& front() const
        {
                return m_vec_front;
        }

        /* Orientation manipulators */
        void zoom(GLdouble yoffset);
        void move(GLuint direction, GLdouble delta);
        void look(GLdouble xoffset, GLdouble yoffset, GLboolean clamp = GL_TRUE);

        /* Orientation matrix getters */
        glm::mat4 view();
        glm::mat4 projection(GLdouble vp_aspect_ratio);
        glm::mat4 view_projection(GLdouble vp_aspect_ratio);
};

#endif /* ifndef SCRAMBLE_CAMERA_H */
