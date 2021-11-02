#version 450 core

layout (location=0) out vec4 fFragClr;

in vec3 Normal;
in vec3 crntPos;

uniform vec3 Colour;
uniform vec3 texDiff;
uniform vec3 camPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main () 
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - crntPos);    
    vec3 F0 = vec3(0.04); 

    F0 = mix(F0, vec3(0.8,0.8,0.8), 0.5);
    vec3 Lo = vec3(0.0);

    vec3 L = normalize(camPos - crntPos);
    vec3 H = normalize(V + L);
    float distance = length(camPos - crntPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = vec3(300) * attenuation;

    float NDF = DistributionGGX(N, H, 0.5);   
    float G   = GeometrySmith(N, V, L, 0.5);      
    vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
    vec3 numerator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - 0.5;	  
    float NdotL = max(dot(N, L), 0.0);        
    Lo += (kD * vec3(0.8) / PI + specular) * radiance * NdotL;  
    
    vec3 ambient = vec3(0.03) * vec3(0.8) * vec3(1.0);
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    fFragClr =  vec4(color, 1.0);
}