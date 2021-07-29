
#Shader Vertex

#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 TexCoord;


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
	gl_Position = Projection * View * Model * vec4(Position, 1.0f);
	
	Texture = TexCoord;
}




#Shader Fragment

#version 330 core
struct Material
{
	vec3 Ambient;
    sampler2D Diffuse;
	sampler2D Specular;
	float Shine;
};
uniform Material material;

struct Lighting
{
	vec3 LightPosition;
	vec3 Direction;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;

	float Cutoff;
	float OuterCutoff;
};
uniform Lighting LightSource;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 Texture;

uniform vec3 Light;
uniform vec3 ObjectColor;
uniform vec3 ViewPos;


void main()
{	
		vec3 Ambient = LightSource.Ambient * texture(material.Diffuse, Texture).rgb;

		//Diffuse
		vec3 SurfaceNormal = normalize(Normal);
		vec3 LightDir = normalize(LightSource.LightPosition - FragPos);
		float Diff = max(dot(SurfaceNormal, LightDir), 0.0);
		vec3 Diffuse = LightSource.Diffuse * Diff * texture(material.Diffuse, Texture).rgb;

		//Specular
		vec3 ViewDir = normalize(ViewPos - FragPos);
		vec3 Reflect = reflect(-LightDir, SurfaceNormal);
		float Spec = pow(max(dot(ViewDir, Reflect), 0.0), material.Shine);
		vec3 Specular = LightSource.Specular * Spec * texture(material.Specular, Texture).rgb;

		float theta = dot(LightDir, normalize(-LightSource.Direction));
		float epsilon = (LightSource.Cutoff - LightSource.OuterCutoff);
		float Intensity = clamp((theta - LightSource.OuterCutoff) / epsilon, 0.0, 1.0);

		Diffuse *= Intensity;
		Specular *= Intensity;

		float Distance = length(LightSource.LightPosition - FragPos);
		float Attenuation = 1.0f / (1.0f + (0.09 * Distance) + (0.032 * (Distance * Distance)));

		Ambient *= Attenuation;
		Diffuse *= Attenuation;
		Specular *= Attenuation;

		vec3 Result = (Specular + Ambient + Diffuse);
		FragColor = vec4(Result, 1.0);

}