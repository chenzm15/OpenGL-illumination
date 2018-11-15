#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 默认设定值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ZOOM = 45.0f;
const float SENSITIVITY = 0.2f; // 视角调整灵敏度
const float SPEED = 2.5f; // 移动速度

enum class Projection_Type // 投影方式
{
	PERSPECTIVE, // 透视投影
	ORTHO        // 正交投影
};

enum class Camera_Movement
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	FORWARD,
	BACKWARD
};

class Camera
{
	float Yaw, Pitch;
	float Zoom;
	float Sensitivity;
	float Speed;
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 globalUp;
	Projection_Type projection_type;

public:
	Camera(const glm::vec3 &pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f), Projection_Type type = Projection_Type::PERSPECTIVE, float pitch = PITCH, float yaw = YAW)
		: Zoom(ZOOM), Sensitivity(SENSITIVITY), Speed(SPEED)
	{
		projection_type = type;
		Pitch = pitch;
		Yaw = yaw;
		Position = pos;
		globalUp = up;

		updateCameraVectors();
	}

	glm::vec3 getCameraPosition() const
	{
		return Position;
	}

	Projection_Type getProjectionType() const
	{
		return projection_type;
	}

	float getFov() const
	{
		return Zoom;
	}

	glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessKeyPressed(Camera_Movement movement, float deltaTime)
	{
		float delta = Speed * deltaTime;

		switch (movement)
		{
		case Camera_Movement::FORWARD:
			Position += Front * delta;
			break;
		case Camera_Movement::BACKWARD:
			Position -= Front * delta;
			break;
		case Camera_Movement::LEFT:
			Position -= Right * delta;
			break;
		case Camera_Movement::RIGHT:
			Position += Right * delta;
			break;
		case Camera_Movement::UP:
			Position += Up * delta;
			break;
		case Camera_Movement::DOWN:
			Position -= Up * delta;
			break;
		}
	}

	void ProcessMouseDragging(float xoffset, float yoffset)
	{
		float delta_x = xoffset * Sensitivity;
		float delta_y = yoffset * Sensitivity;

		Yaw += delta_x;
		Pitch += delta_y;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset)
	{
		if (projection_type == Projection_Type::ORTHO)
			return;

		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, globalUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif
