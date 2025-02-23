#include "tetrapc.h"
#include "Camera.h"
#include "../utils/Time.h"

using namespace TetraEngine;

Camera* Camera::main = NULL;

// constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, unsigned width, unsigned height) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), width(width), height(height)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;

    updateCameraVectors();

    if (Camera::main == NULL) Camera::main = this;
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();

    if (Camera::main == NULL) Camera::main = this;
}

void Camera::ProcessMovement(const Event<InputInfo>& ev)
{
    if (InputManager::GetMain()->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {

        AnyKeyEvent anyKey = ev.ToType<AnyKeyEvent>();
        if (anyKey.key == GLFW_KEY_W)
            ProcessMovement(FORWARD);
        if (anyKey.key == GLFW_KEY_S)
            ProcessMovement(BACKWARD);
        if (anyKey.key == GLFW_KEY_D)
            ProcessMovement(RIGHT);
        if (anyKey.key == GLFW_KEY_A)
            ProcessMovement(LEFT);
        if (anyKey.key == GLFW_KEY_SPACE)
            ProcessMovement(UP);
        if (anyKey.key == GLFW_KEY_LEFT_SHIFT)
            ProcessMovement(DOWN);
    }
}
void Camera::SetMain(Camera* cam)
{
    if (Camera::main != nullptr)
    {
		InputManager::GetMain()->keyDispatcher.RemoveListener<Camera>(InputInfo(GLFW_REPEAT, TETRA_INPUT_KEY_MODE), &Camera::ProcessMovement, (*Camera::main));
		InputManager::GetMain()->keyDispatcher.RemoveListener<Camera>(InputInfo(TETRA_INPUT_MOUSE_MOVE_MODE), &Camera::ProcessMouseMovement, (*Camera::main));
    }
	Camera::main = cam;
    InputManager::GetMain()->keyDispatcher.AddListener<Camera>(InputInfo(GLFW_REPEAT, TETRA_INPUT_KEY_MODE), &Camera::ProcessMovement, (*cam));
    InputManager::GetMain()->keyDispatcher.AddListener<Camera>(InputInfo(TETRA_INPUT_MOUSE_MOVE_MODE), &Camera::ProcessMouseMovement, (*cam));
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessMovement(Camera_Movement direction)
{
    float deltaTime = Time::deltaTime;
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += glm::normalize(Front - (Front * glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0)) * velocity;
    if (direction == BACKWARD)
        Position -= glm::normalize(Front - (Front * glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0)) * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += glm::vec3(0, 1, 0) * velocity;
    if (direction == DOWN)
        Position -= glm::vec3(0, 1, 0) * velocity;
}

void Camera::ProcessMouseMovement(const Event<InputInfo>& ev)
{
	if (TETRA_INPUT_MODE(ev.GetType()) == TETRA_INPUT_MOUSE_MOVE_MODE
        && InputManager::GetMain()->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		MouseMoveEvent mouse = ev.ToType<MouseMoveEvent>();
        mouse.deltaX *= MouseSensitivity;
        mouse.deltaY *= MouseSensitivity;

        Yaw += mouse.deltaX;
        Pitch -= mouse.deltaY;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        updateCameraVectors();
	}
}

[[deprecated]]
// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}
void Camera::SetProjection(glm::mat4 proj) {
    projectionView = proj;
}

void Camera::SetProjection(float fovy, float ratio, float nearPlane, float farPlane) {
    projectionView = glm::perspective(fovy, ratio, nearPlane, farPlane);
}
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}