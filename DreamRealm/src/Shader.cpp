#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Shader.h"

Shader::Shader()
{
}

void Shader::Load(const char* ShaderFilePath)
{
	enum class ShaderType {
		NON = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string Line;
	std::ifstream ShaderFile(ShaderFilePath);
	std::stringstream VertexStream, FragmentStream;
	ShaderType Type = ShaderType::NON;

	while (getline(ShaderFile, Line))
	{
		if (Line.find("#Shader") != -1)
		{
			if (Line.find("Vertex") != -1)
			{
				Type = ShaderType::VERTEX;
			}
			else
			{
				Type = ShaderType::FRAGMENT;
			}
		}

		else
		{
			if (Type == ShaderType::VERTEX)
				VertexStream << Line << std::endl;

			else
				FragmentStream << Line << std::endl;
		}
	}

	//converting Shaders Streams to Strings, thats the only way to Parse them
	std::string VertexShaderSource = VertexStream.str();
	std::string FragmentShaderSource = FragmentStream.str();
	//converting back to c strings 
	const char* VertexShadersrc = VertexShaderSource.c_str();
	const char* FragmentShadersrc = FragmentShaderSource.c_str();

	// Compiling Shader Code Full

	unsigned int Shader_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int Shader_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Shader_VertexShader, 1, &VertexShadersrc, nullptr);
	glShaderSource(Shader_FragmentShader, 1, &FragmentShadersrc, nullptr);
	glCompileShader(Shader_VertexShader);
	glCompileShader(Shader_FragmentShader);

	//Error handling of Vertex Shaders Compilations incase of any errors; incase of Errors during compilations its obvious lol
	int Compiled;
	int Length;
	glGetShaderiv(Shader_VertexShader, GL_COMPILE_STATUS, &Compiled);

	if (!Compiled)
	{
		glGetShaderiv(Shader_VertexShader, GL_INFO_LOG_LENGTH, &Length);
		char* Message = (char*)alloca(Length * sizeof(char));
		glGetShaderInfoLog(Shader_VertexShader, Length, &Length, Message);
		std::cerr << "VERTEX SHADER FAILED TO COMPILE\n" << Message << "\n";

		glDeleteShader(Shader_VertexShader);

		this->Program = -1;
	}

	//Error check Handling for Fragment Shaders Compilations; incase of errors during compilations 
	glGetShaderiv(Shader_FragmentShader, GL_COMPILE_STATUS, &Compiled);

	if (!Compiled)
	{
		glGetShaderiv(Shader_FragmentShader, GL_INFO_LOG_LENGTH, &Length);
		char* Message = (char*)alloca(Length * sizeof(char));
		glGetShaderInfoLog(Shader_FragmentShader, Length, &Length, Message);
		std::cerr << "FRAGMENT SHADER FAILED TO COMPILE\n" << Message << "\n";

		glDeleteShader(Shader_FragmentShader);

		this->Program = -1;
	}
	//Linking the VERTEX AND FRAGMENT SHADERS INTO A PROGRAM
	this->Program = glCreateProgram();
	glAttachShader(this->Program, Shader_VertexShader);
	glAttachShader(this->Program, Shader_FragmentShader);
	glLinkProgram(this->Program);

	//Checking for any linking errors ***********************
	int Linked;
	glGetProgramiv(this->Program, GL_LINK_STATUS, &Linked);

	if (!Linked)
	{
		glGetProgramiv(this->Program, GL_INFO_LOG_LENGTH, &Length);
		char* Message = (char*)alloca(Length * sizeof(char));
		glGetProgramInfoLog(this->Program, Length, &Length, Message);
		std::cerr << "LINKING ERROR\n" << Message << "\n";

		glDeleteShader(Shader_VertexShader);
		glDeleteShader(Shader_FragmentShader);
		glDeleteProgram(this->Program);

		this->Program = -1;
	}

	glDeleteShader(Shader_VertexShader);
	glDeleteShader(Shader_FragmentShader);
}

void Shader::Use()
{
	glUseProgram(this->Program);
}

void Shader::SetBool(const char* String, bool Value)const
{
	glUniform1i(glGetUniformLocation(this->Program, String), (int)Value);
}
void Shader::SetInt(const char* String, int Value)const
{
	glUniform1i(glGetUniformLocation(this->Program, String), Value);
}
void Shader::SetFloat(const char* String, float Value)const
{
	glUniform1f(glGetUniformLocation(this->Program, String), Value);
}

Shader::~Shader()
{
	glDeleteProgram(this->Program);
}