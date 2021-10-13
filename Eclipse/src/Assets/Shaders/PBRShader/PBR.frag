#version 450 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform bool EnableGammaCorrection;
uniform float Exposure;
uniform float gamma;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;

struct PointLight 
{    
    vec3 lightColor;
    vec3 position;  
    float constant;
    float linear;
    float quadratic;  
    float IntensityStrength;
    vec3 RGBColor;
};    

struct DirectionalLight 
{
    vec3 direction;
	vec3 lightColor;
};

struct SpotLight 
{
    vec3 lightColor;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

#define NR_POINT_LIGHTS 15  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int NumberOfPointLights;

#define NR_DIRECTIONAL_LIGHTS 1  
uniform DirectionalLight directionlight[NR_DIRECTIONAL_LIGHTS];

#define NR_SPOTLIGHTS 10  
uniform SpotLight spotLights[NR_SPOTLIGHTS];
uniform int NumberOfSpotLights;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

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

vec3 fresnelSchlick(float cosTheta, vec3 baseReflectivity)
{
    return baseReflectivity + (1.0 - baseReflectivity) * pow((1.0 - cosTheta), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{		
    vec3 albedo     = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic  = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao        = texture(aoMap, TexCoords).r;

    vec3 N = getNormalFromMap(); // no normal map can use vec3(0.1)
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // DIRECTIONALIGHT 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    vec3 L = normalize(-directionlight[0].direction - WorldPos);
    vec3 H = normalize(V + L);
    float distance = length(-directionlight[0].direction - WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = directionlight[0].lightColor * attenuation;
    
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
       
    vec3 numerator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
    float NdotL = max(dot(N, L), 0.0);        
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // POINTLIGHT(S)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < NumberOfPointLights; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(pointLights[i].position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(pointLights[i].position - WorldPos);

	    float dist = length(pointLights[i].position - WorldPos);
        float constant = pointLights[i].constant;
	    float linear = pointLights[i].linear;
        float quadratic = pointLights[i].quadratic;
	    float attenuation = pointLights[i].IntensityStrength / (constant + linear * dist + quadratic * ( dist * dist ) );
        //float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].lightColor * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0);        

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SPOTLIGHTS(S)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < NumberOfSpotLights; ++i)
    {
    	vec3 lightDir = normalize(spotLights[i].position - WorldPos);
	    float theta = dot(lightDir, normalize(-spotLights[i].direction));

        // calculate Intensity
		float intensity = clamp((theta - spotLights[i].outerCutOff) / (spotLights[i].cutOff - spotLights[i].outerCutOff), 0.0, 1.0);

        // calculate per-light radiance
        vec3 L = normalize(spotLights[i].position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(spotLights[i].position - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = spotLights[i].lightColor * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0) * intensity;        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   

        vec3 ambient = vec3(0.03) * albedo * ao;
        vec3 color = ambient + Lo;

        // HDR + GAMMA
        //color = color / (color + vec3(1.0));

        color = vec3(1.0) - exp(-color * Exposure);

        color = pow(color, vec3(1.0/2.5)); 
        FragColor = vec4(color, 1.0);
}