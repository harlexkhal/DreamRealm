
#Shader Vertex

#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 Texture;

uniform vec3 LightPos;
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	FragPos = vec3(Model * vec4(Position, 1.0));
	Normal =  mat3(transpose(inverse(Model))) * aNormal;
	gl_Position = Projection * mat4(mat3(View)) * vec4(Position, 1.0f);
	
	Texture = Position;
}


#Shader Fragment

#version 330 core
struct Material
{
     //not necessary to set ambient. ambient usually depends on intensity of the light
	//vec3 Ambient;
    samplerCube Diffuse;
	samplerCube Specular;
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
in vec3 Texture;

uniform vec3 Light;
uniform vec3 ViewPos;

vec4 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
const vec4 FOG = vec4( 0.5, 0.5, 0.5, 0.1 );
void main()
{	
		//Diffuse
		vec3 SurfaceNormal = normalize(Normal);
		vec3 LightDir = normalize(ViewPos - FragPos);

		vec4 Result = CalculateDirectionalLight(DirLight, SurfaceNormal, LightDir);
		float size = Result.r + Result.g + Result.b + Result.a; // [1.,3.]
        size -= 2.; // [-1.,+1.]
        float deltaz = 1.0 * size;
		float fogFactor = ((1.0 + deltaz) - 1.0) / (1.0 - 1.0);
		
		fogFactor = clamp( fogFactor, 0., 1. );
		fogFactor = smoothstep( 0., 1., fogFactor );
		vec3 rgb = mix( vec3(1.0, 1.0, 1.0)  * vec3(.9, .9, .9), FOG.rgb, fogFactor );

		//Result *= vec3(normalize(vec3(223, 208, 86)));
		FragColor = vec4(rgb, 1.0);
}

vec4 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shine);
    vec4 ambient = vec4(texture(material.Diffuse, 1.0 * Texture));
    vec3 diffuse = light.Diffuse * diff * vec3(texture(material.Diffuse, Texture));
    vec3 specular = light.Specular * spec * vec3(texture(material.Specular, 0.0*Texture));
    return (ambient);
}