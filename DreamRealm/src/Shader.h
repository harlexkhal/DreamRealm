#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	Shader();
	void Load(const char* ShadersFilePath);
	Shader operator=(const Shader& shader) { Program = shader.Program; return shader; }
	~Shader();
	void Use();

	void SetBool(const char* String, bool Value)const;
	void SetInt(const char* String, int Value)const;
	void SetFloat(const char* String, float Value)const;

	unsigned int Program;
};