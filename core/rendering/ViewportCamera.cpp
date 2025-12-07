#include "tetrapc.h"
#include "ViewportCamera.h"

#include "../Core.h"
#include "../utils/Time.h"

using namespace TetraEngine;

// constructor with vectors
ViewportCamera::ViewportCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, unsigned width, unsigned height) :
Front(glm::vec3(0.0f, 0.0f, -1.0f)),
MovementSpeed(SPEED),
MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
position(position),
worldUp(up),
yaw(yaw),
pitch(pitch)
{
    ViewportCamera::SetProjection(glm::radians(45.0), (float)width / (float)height, 0.1f, 100.0f);
    UpdateCameraVectors();
}

ViewportCamera::ViewportCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
ViewportCamera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch)
{}

void ViewportCamera::ProcessMovement(const Event<InputEvent>& ev)
{
    if (InputManager::GetMain()->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {

        auto anyKey = ev.ToType<AnyKeyEvent>();
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

void ViewportCamera::Enable() {
    TETRA_USE_GAME_INPUT
    input->AddListener<ViewportCamera>(AnyKeyEvent::GetLink(GLFW_REPEAT), &ViewportCamera::ProcessMovement, (*this));
    input->AddListener<ViewportCamera>(MouseMoveEvent::GetLink(), &ViewportCamera::ProcessMouseMovement, (*this));
}

void ViewportCamera::Disable() {
    TETRA_USE_GAME_INPUT
    input->RemoveListener<ViewportCamera>(AnyKeyEvent::GetLink(GLFW_REPEAT), &ViewportCamera::ProcessMovement, (*this));
    input->RemoveListener<ViewportCamera>(MouseMoveEvent::GetLink(), &ViewportCamera::ProcessMouseMovement, (*this));
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 ViewportCamera::GetViewMatrix() const
{
    return glm::lookAt(position, position + Front, Up);
}

glm::mat4 ViewportCamera::GetProjection() const {
    return projectionView;
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void ViewportCamera::ProcessMovement(Camera_Movement direction)
{
    float deltaTime = Time::deltaTime;
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        position += glm::normalize(Front - (Front * glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0)) * velocity;
    if (direction == BACKWARD)
        position -= glm::normalize(Front - (Front * glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0)) * velocity;
    if (direction == LEFT)
        position -= Right * velocity;
    if (direction == RIGHT)
        position += Right * velocity;
    if (direction == UP)
        position += glm::vec3(0, 1, 0) * velocity;
    if (direction == DOWN)
        position -= glm::vec3(0, 1, 0) * velocity;
}

void ViewportCamera::ProcessMouseMovement(const Event<InputEvent>& ev)
{
	if (InputManager::GetMain()->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		auto mouse = ev.ToType<MouseMoveEvent>();
        mouse.deltaX *= MouseSensitivity;
        mouse.deltaY *= MouseSensitivity;

        yaw += mouse.deltaX;
        pitch -= mouse.deltaY;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        UpdateCameraVectors();
	}
}

[[deprecated]]
// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void ViewportCamera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void ViewportCamera::ProcessMouseScroll(float yOffset)
{
    Zoom -= yOffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

float ViewportCamera::GetRatio() const {
    return ratio;
}

void ViewportCamera::SetRatio(float pRatio) {
    ratio = pRatio;
    projectionView = glm::perspective(fovY, ratio, nearPlane, farPlane);
}

float ViewportCamera::GetFOV() const {
    return fovY;
}

void ViewportCamera::SetFOV(float pFovY) {
    fovY = pFovY;
    projectionView = glm::perspective(fovY, ratio, nearPlane, farPlane);
}

void ViewportCamera::SetProjection(float pFovY, float pRatio, float pNearPlane, float pFarPlane) {
    fovY = pFovY;
    ratio = pRatio;
    nearPlane = pNearPlane;
    farPlane = pFarPlane;
    projectionView = glm::perspective(fovY, ratio, nearPlane, farPlane);
}

glm::vec3 ViewportCamera::GetPosition() {
    return position;
}

void ViewportCamera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}
