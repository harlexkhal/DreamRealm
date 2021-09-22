#Shader Vertex

#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec2 Texture;

uniform vec3 LightPos;
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	FragPos = vec3(Model * vec4(Position, 1.0));
	Normal =  mat3(transpose(inverse(Model))) * aNormal;
	gl_Position = Projection * mat4(mat3(View)) * Model * vec4(Position, 1.0f);
	
	Texture = TexCoord;
}


#Shader Fragment
#version 330 core

out vec4 FragColor;
in vec2 Texture;
uniform sampler2D NoiseTexture;
const float PI = 3.14159265;

uniform float Bias;
uniform float nscale;
vec4 sky = vec4(.3, .5, .8, 1.0);
vec4 cloud = vec4(1.0, 1.0, 1, 1.0);
void main()
{	

	FragColor = vec4(.9,.9,.9, 1.0);
}