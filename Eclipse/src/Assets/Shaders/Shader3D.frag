#version 450 core

layout(location = 0) in vec2 TxtCoord;
layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec4 uColor;
uniform bool uTextureCheck;
uniform vec4 lightColor; in vec3 crntPos;
uniform vec3 lightPos; in vec3 normal_from_vtxShader;
uniform vec3 camPos;

// Structs
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
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

struct DirLight {
    vec3 direction;
    vec3 lightColor;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 lightColor;
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float IntensityStrength;
    float cutOff;
    float outerCutOff;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int NumberOfPointLights;

#define NR_DIRECTIONAL_LIGHTS 1
uniform DirLight directionlight[NR_DIRECTIONAL_LIGHTS];

#define NR_SPOT_LIGHTS 1
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform int NumberOfSpotLights;

// Function Headers
vec3 CalcPointLight(PointLight light, vec3 normala, vec3 fragPos, vec3 viewDira);
vec3 CalcDirLight(DirLight light, vec3 normala, vec3 viewDira);
vec3 CalcSpotLight(SpotLight light, vec3 normala, vec3 fragPos, vec3 viewDira);

vec4 testMaterials() 
{
    vec3 ambientA = vec3(0.0215, 0.1745, 0.0215);
    vec3 diffuseA = vec3(0.07568, 0.61424, 0.07568);
    vec3 specularA = vec3(0.633, 0.727811, 0.633);
    float shininessA = 0.6f;

    // ambient
    vec3 ambient = vec3(lightColor) * ambientA;

    // diffuse 
    vec3 norm = (normal_from_vtxShader);
    vec3 lightDir = normalize(lightPos - crntPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(lightColor) * (diff * diffuseA);

    // specular
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessA);
    vec3 specular = vec3(lightColor) * (spec * specularA);

    vec3 result = ambient + diffuse + specular;
    return (texture(uTex2d, TxtCoord) * vec4(result, 1.0));
}

void main() 
{

    if (!uTextureCheck) 
    {
        fFragClr = vec4(uColor);
    } 
    else 
    {
        vec3 result = vec3(0, 0, 0);

        vec4 ignore = lightColor;
        //vec3 ignore1 = lightPos;

        // properties
        vec3 norm = (normal_from_vtxShader);
        vec3 viewDir = normalize(camPos - crntPos);

        result = CalcDirLight(directionlight[0], norm, viewDir);

        for (int i = 0; i < NumberOfPointLights; i++) {
            result += CalcPointLight(pointLights[i], norm, crntPos, viewDir);
        }

        for (int i = 0; i < NumberOfSpotLights; i++) {
            result += CalcSpotLight(spotLights[i], norm, crntPos, viewDir);
        }

        fFragClr = texture(uTex2d, TxtCoord) * vec4(uColor) * vec4(result, 1.0f);
    }
}

// Pointllght Calculation
vec3 CalcPointLight(PointLight light, vec3 normala, vec3 fragPos, vec3 viewDira) 
{
    vec3 lightVec = (light.position - fragPos);

    // intensity of light with respect to distance
    float dist = length(lightVec);
    float constant = light.constant;
    float linear = light.linear;
    float quadratic = light.quadratic;
    float inten = light.IntensityStrength / (constant + linear * dist + quadratic * (dist * dist));

    vec3 ambientStrength = light.ambient;
    vec4 ambient = vec4(light.lightColor, 1.0) * vec4(ambientStrength, 1.0) * inten;

    // diffuse lighting
    vec3 normal = normalize(normal_from_vtxShader);
    vec3 lightDirection = normalize(lightVec);
    //float diff = max(dot(normal, lightDirection), 0.0f); 
    vec3 diffuse = light.lightColor * light.diffuse * inten;

    vec3 specularStrength = light.specular;
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDira, reflectDir), 0.0), 32); // 32 is material shiness
    vec3 specular = specularStrength * spec * light.lightColor * inten;

    vec3 result = vec3(vec3(ambient) + diffuse + specular);

    return vec3(texture(uTex2d, TxtCoord)) * result;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normala, vec3 fragPos, vec3 viewDira) 
{
    // controls how big the area that is lit up is
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    // amibient
    vec3 ambientStrength = light.ambient;
    vec4 ambient = vec4(light.lightColor, 1.0) * vec4(ambientStrength, 1.0);

    // diffuse lighting
    vec3 normal = normalize(normal_from_vtxShader);
    vec3 lightDirection = normalize(light.position - fragPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular
    vec3 specularStrength = light.specular;
    vec3 reflectDir = reflect(-light.position, normal);
    float spec = pow(max(dot(viewDira, reflectDir), 0.0), 256); // 32 is material shiness
    vec3 specular = spec * specularStrength * light.lightColor;

    float angle = dot(vec3(0.0f, 0.0f, -6.0f), -lightDirection);
    float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    vec3 result = vec3(ambient) + diffuse + specular;
    return vec3(texture(uTex2d, TxtCoord)) * result * intensity;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normala, vec3 viewDira) 
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normala, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normala);
    float spec = pow(max(dot(viewDira, reflectDir), 0.0), 2);

    // combine results
    vec3 ambient = light.ambient * vec3(light.lightColor) * vec3(texture(uTex2d, TxtCoord));
    vec3 diffuse = light.diffuse * vec3(light.lightColor) * diff * vec3(texture(uTex2d, TxtCoord));
    vec3 specular = light.specular * vec3(light.lightColor) * spec * vec3(texture(uTex2d, TxtCoord));

    return (ambient + diffuse + specular);
}