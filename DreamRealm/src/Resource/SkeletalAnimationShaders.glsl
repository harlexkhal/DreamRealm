
#Shader Vertex
#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

out vec3 Normal;
out vec3 FragPos;
out vec2 Texture;

const int MAX_BONES = 100;

uniform vec3 LightPos;
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform mat4 gBones[MAX_BONES];

void main()
{
	mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
	BoneTransform += gBones[BoneIDs[1]] * Weights[1];
	BoneTransform += gBones[BoneIDs[2]] * Weights[2];
	BoneTransform += gBones[BoneIDs[3]] * Weights[3];

	gl_Position = Projection * View * Model * BoneTransform * vec4(Position, 1.0);

	FragPos = vec3(Model * BoneTransform * vec4(Position, 1.0));
	Normal = mat3(transpose(inverse(Model * BoneTransform))) * aNormal;

	Texture = TexCoord;
}

#Shader Fragment

#version 330 core
struct Material
{
	 //not necessary to set ambient. ambient usually depends on intensity of the light
	//vec3 Ambient;
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