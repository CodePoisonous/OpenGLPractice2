#pragma once

#include <glm/glm.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// ŷ����ԭ�����ɵ��������
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 worldup, float yaw, float pitch);
	~Camera();

public:
	// ��ȡ�۲����
	glm::mat4 GetViewMatrix();

public:
	// ������Ӧ
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

private:
	// �����������������
	void updateCameraVector();

public:
	float Zoom = 45.0f;				// ͶӰ�Ƕȷ�Χ

private:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);	// �����λ��
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);		// ���������
	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);	// ��������

private:
	glm::vec3 Up;					// ���������
	glm::vec3 Right;				// ���������

private:
	float Yaw = -90.0f;				// ƫ����(0��ָ��x��������-90��ָ��z�Ḻ����)
	float Pitch = 0.0;				// ������

private:
	float MovementSpeed = 2.5f;		// ������ƶ��ٶ�
	float MouseSensitivity = 0.1f;	// ���������
};