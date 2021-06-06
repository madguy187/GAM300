#version 450 core

layout (location=0) in vec2 TxtCoord;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec4 uColor;
uniform bool uTextureCheck;

uniform vec4 lightColor;

in vec3 crntPos;
uniform vec3 lightPos;
in vec3 normal_from_vtxShader;
uniform vec3 camPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
//uniform Material material;

vec4 testMaterials()
{
    vec3 ambientA = vec3(0.0215,0.1745,0.0215);
    vec3 diffuseA = vec3(0.07568,0.61424,0.07568);;
    vec3 specularA = vec3(0.633,0.727811,0.633);;
    float shininessA = 0.6f;

    // ambient
    vec3 ambient = vec3(lightColor) * ambientA;
  	
    // diffuse 
    vec3 norm = (normal_from_vtxShader);
    vec3 lightDir = normalize(lightPos - crntPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(lightColor) * (diff *diffuseA);
    
    // specular
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessA);
    vec3 specular = vec3(lightColor) * (spec * specularA);  
        
    vec3 result = ambient + diffuse + specular;
    return (texture(uTex2d, TxtCoord) * vec4(result, 1.0) );
}

vec4 pointLight()
{
	vec3 lightVec = lightPos - crntPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float constant = 1.00;
	float linear = 0.09;
    float quadratic = 0.032f;
	float inten = 1.0f / (constant + linear * dist + quadratic * ( dist * dist ) );

    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * lightColor * inten;

    // diffuse lighting
    vec3 normal = (normal_from_vtxShader);
    vec3 lightDirection = normalize(lightVec);
    float diff = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = diff * vec3(lightColor) * inten;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDirection, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specularStrength * spec * vec3(lightColor) * inten;  

    vec3 result = vec3( vec3(ambient) + diffuse + specular) * vec3(uColor) ;

    return texture(uTex2d, TxtCoord) * lightColor * vec4(result,1.0f);
}

vec4 DirectionalLight()
{
    vec3 ignore = lightPos;

    // Ambient Lighting
    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * lightColor;

    // diffuse lighting
    vec3 normal = (normal_from_vtxShader);
    vec3 lightDirection = normalize(vec3(0.0f,1.0f,1.0f));
    float diff = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = diff * vec3(lightColor);

    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDirection, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specularStrength * spec * vec3(lightColor);  

    vec3 result = vec3( vec3(ambient) + diffuse + specular) * vec3(uColor) ;

    return texture(uTex2d, TxtCoord) * lightColor * vec4(result,1.0f);
}

void main () 
{
	if(!uTextureCheck)
	{
		fFragClr = vec4(uColor);
	}
	else
	{
    vec4 hi  = pointLight();	
    
    fFragClr = texture(uTex2d, TxtCoord) * uColor;
	}
}
