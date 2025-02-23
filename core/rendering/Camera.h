#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "../InputManager.h"

template <typename T>
class Event;
//extern template class Event<GLFWContext::KeyInfo>;

namespace TetraEngine
{
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    // Default camera values
    const float YAW = 0.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;

    const unsigned int WIDTH = 1920;
    const unsigned int HEIGHT = 1080;

    const glm::mat4 PROJ = glm::ortho(0.f, (float)WIDTH, (float)HEIGHT, 0.f, 0.f, 1000.f);


    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera
    {
    public:
        static Camera* main;

        // camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        // euler Angles
        float Yaw;
        float Pitch;
        // camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        float width;
        float height;

        glm::mat4 projectionView;

        // constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, unsigned width = WIDTH, unsigned height = HEIGHT);
        // constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        void ProcessMovement(const Event<InputInfo>& ev);

        static void SetMain(Camera* cam);
        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix();

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessMovement(Camera_Movement direction);

        void ProcessMouseMovement(const Event<InputInfo>& ev);

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset);

        void SetProjection(glm::mat4 proj);

        void SetProjection(float fovy, float ratio, float nearPlane, float farPlane);
    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();
    };

}
