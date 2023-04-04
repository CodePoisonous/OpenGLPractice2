#pragma once

#include <glm/glm.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// 欧拉角原理抽象成的摄像机类
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 worldup, float yaw, float pitch);
	~Camera();

public:
	// 获取观察矩阵
	glm::mat4 GetViewMatrix();

public:
	// 键鼠响应
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

private:
	// 更新相机坐标轴向量
	void updateCameraVector();

public:
	float Zoom = 45.0f;				// 投影角度范围

private:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);	// 摄像机位置
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);		// 摄像机方向
	glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);	// 世界上轴

private:
	glm::vec3 Up;					// 摄像机上轴
	glm::vec3 Right;				// 摄像机右轴

private:
	float Yaw = -90.0f;				// 偏航角(0°指向x轴正方向，-90°指向z轴负方向)
	float Pitch = 0.0;				// 俯仰角

private:
	float MovementSpeed = 2.5f;		// 摄像机移动速度
	float MouseSensitivity = 0.1f;	// 鼠标灵敏度
};