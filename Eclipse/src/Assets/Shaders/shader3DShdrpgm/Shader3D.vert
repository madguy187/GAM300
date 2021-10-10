#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec2 TextureCoord;
layout (location=2) in vec3 normal_from_vtxbuffer; 
layout (location=3) in vec3 Tangents;

layout (location=0) out vec2 TxtCoord;

struct PointLight 
{    
    vec3 lightColor;
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;  
    float IntensityStrength;
};  

struct DirectionalLight 
{
    vec3 direction;
	vec3 lightColor;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool AffectsWorld;
};

struct SpotLight 
{
    float IntensityStrength;
    vec3 lightColor;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
    float SurroundingAttenuationLevel;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 15  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int NumberOfPointLights;

#define NR_SPOTLIGHTS 10  
uniform SpotLight spotLights[NR_SPOTLIGHTS];
uniform int NumberOfSpotLights;

#define NR_DIRECTIONAL_LIGHTS 1  
uniform DirectionalLight directionlight[NR_DIRECTIONAL_LIGHTS];

uniform mat4 uModelToNDC;
uniform mat4 model;
uniform bool TEST;
uniform vec3 camPos;

out vec3 crntPos;
out vec2 TexCoord;
out vec3 normal_from_vtxShader;
out vec3 TangentSpotLight[NR_SPOTLIGHTS];
out vec3 TangentPointLight[NR_POINT_LIGHTS];
out vec3 TangentDirectionalLight[NR_DIRECTIONAL_LIGHTS];
out TANGENT_VAR{
	vec3 TangentViewPos;
	vec3 TangentCrntPos;
} tangent_varVS;


void main() 
{	 
		crntPos = vec3(model * vec4(vVertexPosition, 1.0));
		
		mat3 normalMatrix = mat3(transpose(inverse(model)));
		normal_from_vtxShader = normalMatrix * normal_from_vtxbuffer;  
				
		TxtCoord = TextureCoord;
		
		vec3 T = normalize(normalMatrix * Tangents);
		vec3 N = normalize(normalMatrix * normal_from_vtxbuffer);
		T = normalize(T - dot(T, N) * N);
		vec3 B = cross(N,T);
		
		mat3 TBN = transpose(mat3(T,B,N));
		
		//PointLights
		for(int i = 0; i < NumberOfPointLights; ++i)
		{
			TangentPointLight[i] = TBN * pointLights[i].position; 
		}
		
		//SpotLights
		for(int i = 0; i < NumberOfSpotLights; ++i)
		{
			TangentSpotLight[i] = TBN * spotLights[i].position;
		}
		
		//Directional Light
		TangentDirectionalLight[0] = TBN * directionlight[0].direction;
		
		tangent_varVS.TangentViewPos = TBN * camPos;
		tangent_varVS.TangentCrntPos = TBN * crntPos;
		
		gl_Position =  uModelToNDC * vec4(vVertexPosition, 1.0f);
}
