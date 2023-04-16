#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera()
{
	updateCameraVector();
}

Camera::Camera(glm::vec3 position, glm::vec3 worldup, float yaw, float pitch)
{
	Position = position;
	WorldUp = worldup;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVector();
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::GetCameraPosition()
{
	return Position;
}

glm::vec3 Camera::GetCameraFront()
{
	return Front;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (FORWARD == direction) Position += Front * cameraSpeed;
	if (BACKWARD == direction) Position -= Front * cameraSpeed;
	if (LEFT == direction) Position -= Right * cameraSpeed;
	if (RIGHT == direction) Position += Right * cameraSpeed;
	if (UP == direction) Position += WorldUp * cameraSpeed;
	if (DOWN == direction) Position -= WorldUp * cameraSpeed;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch/* = true*/)
{
	// ÉèÖÃÊó±êÁéÃô¶È
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	// ÐÞ¸Ä¸©Ñö½ÇºÍÆ«º½½Ç
	Yaw += xoffset;
	Pitch += yoffset;

	// ÏÞÖÆ¸©Ñö½ÇºÍÆ«º½½ÇµÄ·¶Î§
	if (constrainPitch && Pitch > 89.0f) Pitch = 89.0f;
	if (constrainPitch && Pitch < -89.0f) Pitch = -89.0f;

	updateCameraVector();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 80.0f)
		Zoom = 80.0f;
}

void Camera::updateCameraVector()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}