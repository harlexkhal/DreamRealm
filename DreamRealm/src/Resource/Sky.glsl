
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

		vec4 Result = texture(NoiseTexture, nscale * Texture);

		float sum = ( 3.* Result.r + Result.g + Result.b + Result.a - 2. ) / 2.;
        sum = ( 1. + cos(PI * sum) ) / 2.;
        float t = clamp( Bias + sum, 0., 1. );

        vec3 color = mix( sky.rgb, cloud.rgb, t);

        color *= vec3(0.9, 0.9, 0.9);

		FragColor = vec4(color, 1.0);
}