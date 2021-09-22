#include "Camera.h"

Camera::Camera(float fov, float aspectratio, float near, float far)
	:FOV(fov), AspectRatio(aspectratio), NearPlane(near), FarPlane(far)
{
	Velocity = 60.0f;
	Up = CrunchMath::Vec3(0.0f, 1.0f, 0.0f);
	Position = CrunchMath::Vec3(0.0f, 0.0f, 0.0f);
	Front = CrunchMath::Vec3(0.0f, 0.0f, -1.0f);
}

Camera::~Camera(){}

void Camera::UpdateCursorPosition(float x, float y)
{
	if (FirstCursorUpdate)
	{
		CursorLastX = x;
		CursorLastY = y;
		FirstCursorUpdate = false;
	}

	float Xoffset = x - CursorLastX;
	float Yoffset = CursorLastY - y;

	CursorLastX = x;
	CursorLastY = y;

	float Sensitivity = 0.05;
	Xoffset *= Sensitivity;
	Yoffset *= Sensitivity;

	Heading += Xoffset;
	Pitch += Yoffset;

	if (Pitch >= 89.0f)
		Pitch = 89.0f;
	if (Pitch <= -89.0f)
		Pitch = -89.0f;

	Front.x = cos(CrunchMath::Radian(Heading)) * cos(CrunchMath::Radian(Pitch));
	Front.y = sin(CrunchMath::Radian(Pitch));
	Front.z = sin(CrunchMath::Radian(Heading)) * cos(CrunchMath::Radian(Pitch));
	Front.Normalize();

	CrunchMath::Vec3 Right = CrunchMath::CrossProduct(CrunchMath::Vec3(0.0f, 1.0f, 0.0f), Front);
	Up = CrunchMath::CrossProduct(Front, Right);
	Up.Normalize();
}

void Camera::SetCameraPosition(CrunchMath::Vec3 pos)
{
	Position = pos;
}

void Camera::SetFOV(float fov)
{
	FOV = fov;
}

void Camera::CameraControl(GLFWwindow* window, float dt)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += Front * (Velocity * dt);

	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= Front * (Velocity * dt);

	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += CrunchMath::CrossProduct(Front, Up) * (Velocity * dt);

	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position += CrunchMath::CrossProduct(Up, Front) * (Velocity * dt);
}

void Camera::OnUpdate(GLFWwindow* window, float dt)
{
	CameraControl(window, dt);
	Calculate();
}

void Camera::Calculate()
{
	CrunchMath::Vec3 CameraTarget = Position + Front;

	View.SetToLookAt(Position, CameraTarget, Up);
	Projection.SetPerspective(CrunchMath::Radian(FOV), AspectRatio, NearPlane, FarPlane);

	//ProjectionView = Projection * View;
}