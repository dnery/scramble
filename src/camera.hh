/*
 * Created by danilo on 4/17/16.
 */

#ifndef SCRAMBLE_CAMERA_H
#define SCRAMBLE_CAMERA_H

#ifdef CLANG_COMPLETE_ONLY
        #define GL_GLEXT_PROTOTYPES
        #include <GL/gl.h>
        #include <GL/glext.h>
#else
        #include <GL/glew.h>
#endif

#include <glm/glm.hpp>

namespace scramble {

        extern const GLfloat PIT;         // default pitch
        extern const GLfloat YAW;         // default yaw
        extern const GLfloat ZOOM;        // default zoom
        extern const GLfloat SPEED;       // default speed
        extern const GLfloat SENSITIVITY; // default sensitivity

        enum movement {
                LEFT = 1,                 // move flag: left
                RIGHT = 2,                // move flag: right
                FORWARD = 4,              // move flag: forward
                BACKWARD = 8              // move flag: backward
        };

        struct camera {

                glm::vec3 position;       // position vector
                glm::vec3 front;          // front-pointing vector
                glm::vec3 right;          // right-pointing vector
                glm::vec3 cam_up;         // camera up vector
                glm::vec3 world_up;       // world up vector

                GLfloat look_pit;         // look pitch rotation
                GLfloat look_yaw;         // look yaw rotation
                GLfloat look_zoom;        // look zoom factor
                GLfloat move_speed;       // movement speed
                GLfloat sensitivity;      // look sensitivity

                camera();

                void keypress(GLuint direction, GLfloat delta_time);

                void mouse_look(GLfloat xoffset, GLfloat yoffset,
                                GLboolean constrain = GL_TRUE);

                void mouse_scroll(GLfloat yoffset);

                void update_vectors();

                glm::mat4 view_mat();
        };
}

#endif // SCRAMBLE_CAMERA_H
