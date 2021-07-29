#pragma once
#include "CrunchMath/CrunchMath.h"

//Temporary
#include "GLFW/glfw3.h"

class Camera
{
public:
	Camera(float fov, float aspectratio, float near, float far);
	~Camera();

	const CrunchMath::Mat4x4& GetProjection() const { return Projection; }
	const CrunchMath::Mat4x4& GetView() const { return View; }
	const CrunchMath::Mat4x4& GetProjectionView() const { return ProjectionView; }
	const CrunchMath::Vec3& GetPositiion() const { return Position; }
	const CrunchMath::Vec3& GetCameraFront() const { return Front; }

	void UpdateCursorPosition(float x, float y);
	void SetCameraPosition(CrunchMath::Vec3 pos);
	void SetFOV(float fov);

	void OnUpdate(GLFWwindow* window, float dt);

private:
	void Calculate();
	void CameraControl(GLFWwindow* window, float dt);

	CrunchMath::Mat4x4 ProjectionView;
	CrunchMath::Mat4x4 Projection;
	CrunchMath::Mat4x4 View;

	CrunchMath::Vec3 Position;
	CrunchMath::Vec3 Front;
	CrunchMath::Vec3 Up;

	float FOV;
	float AspectRatio;
	float NearPlane;
	float FarPlane;

	float CursorLastX;
	float CursorLastY;
	float Heading;
	float Pitch;
	float FirstCursorUpdate = true;

	float Velocity;
};

