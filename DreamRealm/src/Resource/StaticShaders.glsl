
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

struct DirectionalLight 
{
    vec3 Direction;
	
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};
uniform DirectionalLight DirLight;


out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 Texture;

uniform vec3 Light;
uniform vec3 ViewPos;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main()
{	
		vec3 Ambient = DirLight.Ambient * texture(material.Diffuse, Texture).rgb;

		//Diffuse
		vec3 SurfaceNormal = normalize(Normal);
		vec3 LightDir = normalize(ViewPos - FragPos);

		vec3 Result = CalculateDirectionalLight(DirLight, SurfaceNormal, LightDir);
		FragColor = vec4(Result, 0.1);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shine);
    vec3 ambient = light.Ambient * vec3(texture(material.Diffuse, Texture));
    vec3 diffuse = light.Diffuse * diff * vec3(texture(material.Diffuse, Texture));
    vec3 specular = light.Specular * spec * vec3(texture(material.Specular, Texture));
    return (ambient + diffuse + specular);
}