#include <iostream>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

Camera FPS(72.0f, 1200 / 720, 0.1f, 500.0f);

void CamController(GLFWwindow* window, double xpos, double ypos)
{
	FPS.UpdateCursorPosition(xpos, ypos);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* Window = glfwCreateWindow(1200, 720, "DreamRealm", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(Window);

	glfwSetWindowSizeCallback(Window, [](GLFWwindow* Window, int height, int width)
	{
			glViewport(0, 0, height, width);
	});
    
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(Window, CamController);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	glViewport(0, 0, 1200, 720);

	Shader MyShader;
	MyShader.Load("src/Resource/SkyBox.glsl");
	Mesh MeshModel;
	MeshModel.LoadMesh();

	float LastFrame = 0.0f;

	CrunchMath::Mat4x4 Model;
	CrunchMath::Mat4x4 boneTransform;
	FPS.SetCameraPosition(CrunchMath::Vec3(0.0f, 0.0f, 10.0f));

	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(Window))
	{
		float CurrentFrame = glfwGetTime();//static_cast<float>(clock()) / CLOCKS_PER_SEC;
		float dt = 1.0f / 60.0f;//CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		FPS.OnUpdate(Window, dt);

		Model.SetToIdentity();
		Model.Translate(CrunchMath::Vec3(0.0f, 0.0f, 0.0f));
		Model.Scale(CrunchMath::Vec3(1.0f, 1.0f, 1.0f));

		MyShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(MyShader.Program, "Projection"), 1, GL_FALSE, &FPS.GetProjection().Matrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(MyShader.Program, "View"), 1, GL_FALSE, &FPS.GetView().Matrix[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(MyShader.Program, "Model"), 1, GL_FALSE, &Model.Matrix[0][0]);

		std::vector<CrunchMath::Mat4x4> Transforms;
		/*MeshModel.BoneTransform(glfwGetTime(), Transforms);

		for (unsigned int i = 0; i < Transforms.size(); i++) 
		{
			std::string BoneIndex = "gBones[" + std::to_string(i) + "]";
			
			glUniformMatrix4fv(glGetUniformLocation(MyShader.Program, BoneIndex.c_str()), 1, GL_FALSE, &Transforms[i].Matrix[0][0]);
		}*/

		glUniform3f(glGetUniformLocation(MyShader.Program, "DirLight.Ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(MyShader.Program, "DirLight.Diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(MyShader.Program, "DirLight.Specular"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(MyShader.Program, "LightSource.Direction"), 0.0f, 3.0f, 0.0f);
		glUniform1f(glGetUniformLocation(MyShader.Program, "material.Shine"), 32.0f);

		glUniform3f(glGetUniformLocation(MyShader.Program, "ViewPos"), FPS.GetPositiion().x, FPS.GetPositiion().y, FPS.GetPositiion().z);

		MeshModel.Render();

		glfwPollEvents();
		glfwSwapBuffers(Window);
	}
}