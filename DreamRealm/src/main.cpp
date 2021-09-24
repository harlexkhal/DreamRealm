#include <iostream>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

Camera FPS(72.0f, 1200 / 720, 0.1f, 5000.0f);

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

	Shader SkyDome_Shader;
	SkyDome_Shader.Load("src/Resource/Sky.glsl");
	Mesh SkyDome_Mesh;
	SkyDome_Mesh.LoadMesh("src/Resource/Models/planet/planet.obj");
	SkyDome_Mesh.LoadTexture("src/Resource/Defaults/Textures/noise5.bmp");
	CrunchMath::Mat4x4 SkyDomeModel(1.0f);
	SkyDomeModel.Translate(CrunchMath::Vec3(0.0f, -3.0f, 0.0f));
	SkyDomeModel.Scale(CrunchMath::Vec3(10.0f, 10.0f, 10.0f));
	
	Shader Sun_Shader;
	Sun_Shader.Load("src/Resource/Sun.glsl");
	CrunchMath::Mat4x4 SunModel(1.0f);
	SunModel.Translate(CrunchMath::Vec3(0.0f, 20.0f, -100.0f));
	SunModel.Scale(CrunchMath::Vec3(5.0f, 5.0f, 5.0f));

	Shader Terrain_Shader;
	Terrain_Shader.Load("src/Resource/StaticShaders.glsl");
	Mesh Terrain_Mesh;
	Terrain_Mesh.LoadMesh();
	Terrain_Mesh.LoadTexture("src/Resource/Defaults/Textures/noise-texture3.png");
	CrunchMath::Mat4x4 TerrainModel(1.0f);
	TerrainModel.Translate(CrunchMath::Vec3(0.0f, 0.0f, 0.0f));
	TerrainModel.Scale(CrunchMath::Vec3(1000.0f, 0.1f, 1000.0f));

	Shader Object1_Shader;
	Object1_Shader.Load("src/Resource/StaticShaders.glsl");
	Mesh Object1_Mesh;
	Object1_Mesh.LoadMesh();
	Object1_Mesh.LoadTexture("src/Resource/Defaults/Textures/wood.png");
	CrunchMath::Mat4x4 Object1Model(1.0f);
	Object1Model.Translate(CrunchMath::Vec3(0.0f, 10.3f, 0.0f));
	Object1Model.Scale(CrunchMath::Vec3(10.0f, 10.0f, 10.0f));

	float LastFrame = 0.1f;	

	CrunchMath::Mat4x4 boneTransform;
	FPS.SetCameraPosition(CrunchMath::Vec3(15.0f, 30.0f, 40.0f));

	glEnable(GL_DEPTH_TEST);
	float bias = -0.5f;
	float nscale = -1.0f;
	float Rotate = 0.0f;
	while (!glfwWindowShouldClose(Window))
	{
		float CurrentFrame = glfwGetTime();//static_cast<float>(clock()) / CLOCKS_PER_SEC;
		float dt = 1.0f / 60.0f;//CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
			bias += 0.1 * dt;
		if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
			bias -= 0.1 * dt;

		nscale += 0.009 * dt;

		if (nscale >= 1.0)
			nscale = -1.0f;

		FPS.OnUpdate(Window, dt);

		glDepthMask(GL_FALSE);
		{
			SkyDomeModel.Rotate(CrunchMath::Vec3(0.0f, 1.0f, 0.0f), Rotate);
			SkyDome_Shader.Use();
			glUniform1f(glGetUniformLocation(SkyDome_Shader.Program, "Bias"), bias);
			glUniform1f(glGetUniformLocation(SkyDome_Shader.Program, "nscale"), nscale);
			glUniformMatrix4fv(glGetUniformLocation(SkyDome_Shader.Program, "Projection"), 1, GL_FALSE, &FPS.GetProjection().Matrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(SkyDome_Shader.Program, "View"), 1, GL_FALSE, &FPS.GetView().Matrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(SkyDome_Shader.Program, "Model"), 1, GL_FALSE, &SkyDomeModel.Matrix[0][0]);
			SkyDome_Mesh.Render();
		}
		glDepthMask(GL_TRUE);

		{
			Sun_Shader.Use();
			glUniformMatrix4fv(glGetUniformLocation(Sun_Shader.Program, "Projection"), 1, GL_FALSE, &FPS.GetProjection().Matrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(Sun_Shader.Program, "View"), 1, GL_FALSE, &FPS.GetView().Matrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(Sun_Shader.Program, "Model"), 1, GL_FALSE, &SunModel.Matrix[0][0]);
			SkyDome_Mesh.Render();
		}

		{
		    Terrain_Shader.Use();
		    glUniform3f(glGetUniformLocation(Terrain_Shader.Program, "DirLight.Ambient"), 0.7f, 0.7f, 0.7f);
		    glUniform3f(glGetUniformLocation(Terrain_Shader.Program, "DirLight.Diffuse"), 0.5f, 0.5f, 0.5f);
		    glUniform3f(glGetUniformLocation(Terrain_Shader.Program, "DirLight.Specular"), 1.0f, 1.0f, 1.0f);
		    glUniform3f(glGetUniformLocation(Terrain_Shader.Program, "LightSource.Direction"), 0.0f, 3.0f, 0.0f);
		    glUniform1f(glGetUniformLocation(Terrain_Shader.Program, "material.Shine"), 32.0f);  
		    glUniform3f(glGetUniformLocation(Terrain_Shader.Program, "ViewPos"), FPS.GetPositiion().x, FPS.GetPositiion().y, FPS.GetPositiion().z);
			
			glUniformMatrix4fv(glGetUniformLocation(Terrain_Shader.Program, "Projection"), 1, GL_FALSE, &FPS.GetProjection().Matrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(Terrain_Shader.Program, "View"), 1, GL_FALSE, &FPS.GetView().Matrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(Terrain_Shader.Program, "Model"), 1, GL_FALSE, &TerrainModel.Matrix[0][0]);
			Terrain_Mesh.Render();
		}

		{
			Object1_Shader.Use();
			glUniform3f(glGetUniformLocation(Object1_Shader.Program, "DirLight.Ambient"), 0.7f, 0.7f, 0.7f);
			glUniform3f(glGetUniformLocation(Object1_Shader.Program, "DirLight.Diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(Object1_Shader.Program, "DirLight.Specular"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(Object1_Shader.Program, "LightSource.Direction"), 0.0f, 3.0f, 0.0f);
			glUniform1f(glGetUniformLocation(Object1_Shader.Program, "material.Shine"), 32.0f);
			glUniform3f(glGetUniformLocation(Object1_Shader.Program, "ViewPos"), FPS.GetPositiion().x, FPS.GetPositiion().y, FPS.GetPositiion().z);

			glUniformMatrix4fv(glGetUniformLocation(Object1_Shader.Program, "Projection"), 1, GL_FALSE, &FPS.GetProjection().Matrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(Object1_Shader.Program, "View"), 1, GL_FALSE, &FPS.GetView().Matrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(Object1_Shader.Program, "Model"), 1, GL_FALSE, &Object1Model.Matrix[0][0]);
			Object1_Mesh.Render();
		}
		
		std::vector<CrunchMath::Mat4x4> Transforms;
		/*MeshModel.BoneTransform(glfwGetTime(), Transforms);

		for (unsigned int i = 0; i < Transforms.size(); i++)
		{
			std::string BoneIndex = "gBones[" + std::to_string(i) + "]";

			glUniformMatrix4fv(glGetUniformLocation(MyShader.Program, BoneIndex.c_str()), 1, GL_FALSE, &Transforms[i].Matrix[0][0]);
		}*/

		glfwPollEvents();
		glfwSwapBuffers(Window);
	}
}