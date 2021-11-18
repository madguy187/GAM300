#version 450 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform float Transparency;
uniform bool EmissiveMaterial;
uniform vec3 EmissiveColour;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform int IsNormalMap;
uniform vec3 SurfaceColour;

// material parameters
uniform vec3  AlbedoConstant;
uniform float MetallicConstant;
uniform float RoughnessConstant;
uniform float AoConstant;
uniform int HasInstance;
uniform vec3 BaseReflectivity;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;
const float PI = 3.14159265359;

uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

// IBL
uniform samplerCube irradianceMap;

struct PointLight 
{    
    vec3 lightColor;
    vec3 position;  
    float constant;
    float linear;
    float quadratic;  
    float IntensityStrength;
    vec3 RGBColor;
    int AffectsWorld;
};    

struct DirectionalLight 
{
    vec3 direction;
	vec3 lightColor;
    vec3 position;
    int AffectsWorld;
    vec3 AmbientSettings;
};

struct SpotLight 
{
    vec3 lightColor;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;  
    float IntensityStrength;
    int AffectsWorld;
    vec3 RGBColor;
};

#define NR_POINT_LIGHTS 15  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int NumberOfPointLights;

#define NR_DIRECTIONAL_LIGHTS 1  
uniform DirectionalLight directionlight[NR_DIRECTIONAL_LIGHTS];

#define NR_SPOTLIGHTS 10  
uniform SpotLight spotLights[NR_SPOTLIGHTS];
uniform int NumberOfSpotLights;

uniform int Directional;
uniform int EnableShadows;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(directionlight[0].position - WorldPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
    {
            shadow = 0.0;
    }
        
    return shadow;
}

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
    if(EmissiveMaterial == true)
    {
      FragColor = vec4(EmissiveColour, 1.0);
      
      float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
      if(brightness > 1.0)
          BrightColor = vec4(FragColor.rgb, 1.0);
	  else
	  	BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
    vec3 AmbientSettings = directionlight[0].AmbientSettings; // if i want abit of ambient ill give it 0.03 , lets see as our game need this

    vec3 N;
    vec3 V = normalize(camPos - WorldPos);
    //vec3 N = getNormalFromMap(); // no normal map can use vec3(0.1) or we normalize 
     
    // Variables declared first;
    vec3 albedo;
    float metallic,roughness,ao;

    // base relfect
    vec3 F0 = BaseReflectivity; 

    if(HasInstance == 1)
    {   
        if( IsNormalMap == 1)
        {
            N = getNormalFromMap();
        }
        else
        {
            N = normalize(Normal);
        }

        albedo     = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
        metallic  = texture(metallicMap, TexCoords).r;
        roughness = texture(roughnessMap, TexCoords).r;
        ao        = texture(aoMap, TexCoords).r;
        F0 = mix(F0, albedo, metallic);  
    }
    else
    {
        N = normalize(Normal); 
        F0 = mix(F0, AlbedoConstant, MetallicConstant);
    }

    // reflectance equation
    vec3 Lo = vec3(0.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // DIRECTIONALIGHT 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(Directional == 1)
    {
        if(directionlight[0].AffectsWorld == 1)
        {
            vec3 L = normalize(directionlight[0].position - WorldPos);
            vec3 H = normalize(V + L);
            float distance = length(directionlight[0].position - WorldPos);
            vec3 radiance = directionlight[0].lightColor;
            
            // Cook-Torrance BRDF
            float NDF , G;
            
            if(HasInstance == 1)
            {
                NDF = DistributionGGX(N, H, roughness);   
                G   = GeometrySmith(N, V, L, roughness);           
            }  
            else
            {
                NDF = DistributionGGX(N, H, RoughnessConstant);   
                G   = GeometrySmith(N, V, L, RoughnessConstant);            
            }
            
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
               
            vec3 numerator    = NDF * G * F; 
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
            vec3 specular = numerator / denominator;
            
            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;
            
            if(HasInstance == 1)
            {
                kD *= 1.0 - metallic;	 
                float NdotL = max(dot(N, L), 0.0);        
                Lo += (kD * albedo / PI + specular) * radiance * NdotL;
            } 
            else
            {
                kD *= 1.0 - MetallicConstant;	 
                float NdotL = max(dot(N, L), 0.0);        
                Lo += (kD * AlbedoConstant / PI + specular) * radiance * NdotL;        
            }
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // POINTLIGHT(S)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < NumberOfPointLights; ++i)
    {
        if(pointLights[i].AffectsWorld == 1)
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
            vec3 radiance = pointLights[i].lightColor * attenuation * pointLights[i].RGBColor;

            // Cook-Torrance BRDF
            float NDF , G;

            if(HasInstance == 1)
            {
                NDF = DistributionGGX(N, H, roughness);   
                G   = GeometrySmith(N, V, L, roughness);   
            }  
            else
            {
                NDF = DistributionGGX(N, H, RoughnessConstant);   
                G   = GeometrySmith(N, V, L, RoughnessConstant);            
            }
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
               
            vec3 numerator    = NDF * G * F; 
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
            vec3 specular = numerator / denominator;
            
            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;

            if(HasInstance == 1)
            {
                kD *= 1.0 - metallic;	 
                float NdotL = max(dot(N, L), 0.0);        
                Lo += (kD * albedo / PI + specular) * radiance * NdotL;            
            } 
            else
            {
                kD *= 1.0 - MetallicConstant;	 
                float NdotL = max(dot(N, L), 0.0);        
                Lo += (kD * AlbedoConstant / PI + specular) * radiance * NdotL;        
            }
        }
    }   
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SPOTLIGHTS(S)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < NumberOfSpotLights; ++i)
    {
        if(spotLights[i].AffectsWorld == 1)
        {
    	    vec3 lightDir = normalize(spotLights[i].position - WorldPos);
	        float theta = dot(lightDir, normalize(-spotLights[i].direction));

            // calculate Intensity
		    float intensity = clamp((theta - spotLights[i].outerCutOff) / (spotLights[i].cutOff - spotLights[i].outerCutOff), 0.0, 1.0);

            // calculate per-light radiance
            vec3 L = normalize(spotLights[i].position - WorldPos);
            vec3 H = normalize(V + L);
            float distance = length(spotLights[i].position - WorldPos);
            float dist = length(spotLights[i].position - WorldPos);
            float constant = spotLights[i].constant;
	        float linear = spotLights[i].linear;
            float quadratic = spotLights[i].quadratic;
	        float attenuation = spotLights[i].IntensityStrength / (constant + linear * dist + quadratic * ( dist * dist ) );
            vec3 radiance = spotLights[i].lightColor * attenuation * spotLights[i].RGBColor;

            // Cook-Torrance BRDF
            float NDF , G;

            if(HasInstance == 1)
            {
                NDF = DistributionGGX(N, H, roughness);   
                G   = GeometrySmith(N, V, L, roughness);                
            }  
            else
            {
                NDF = DistributionGGX(N, H, RoughnessConstant);   
                G   = GeometrySmith(N, V, L, RoughnessConstant);            
            }
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
               
            vec3 numerator    = NDF * G * F; 
            float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
            vec3 specular = numerator / denominator;

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;

            if(HasInstance == 1)
            {
                kD *= 1.0 - metallic;	 
                float NdotL = max(dot(N, L), 0.0) * intensity;        
                Lo += (kD * albedo / PI + specular) * radiance * NdotL;            
            } 
            else
            {
                kD *= 1.0 - MetallicConstant;	 
                float NdotL = max(dot(N, L), 0.0) * intensity;        
                Lo += (kD * AlbedoConstant / PI + specular) * radiance * NdotL;        
            }
        }
    }   

       float shadow = 0.0;

       if(Directional == 1 && directionlight[0].AffectsWorld == 1)
       {
           shadow = ShadowCalculation(FragPosLightSpace);    
       }
       else
       {
           shadow = 1.0;
       }

       if(EnableShadows == 0)
       {
           shadow = 1.0;
       }

       if( HasInstance == 1 )
       {  
          vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
          vec3 kD = 1.0 - kS;
          kD *= 1.0 - metallic;	  
          vec3 irradiance =    texture(irradianceMap, N).rgb;
          vec3 diffuse      =  irradiance * albedo;
          vec3 ambient =  (kD * diffuse) * ao;

          //vec3 ambient =  ( AmbientSettings + (1.0 - shadow) ) * albedo * ao;
          vec3 color = ambient + Lo ;
          color = color / (color + vec3(1.0));
          color = pow(color, vec3(1.0/2.2)); 
          FragColor = vec4(color, Transparency);            
        }
        else
        {
           vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
           vec3 kD = 1.0 - kS;
           kD *= 1.0 - MetallicConstant;	  
           vec3 irradiance =    texture(irradianceMap, N).rgb;
           vec3 diffuse      =  irradiance * AlbedoConstant;
           vec3 ambient = (kD * diffuse) * AoConstant;

          //vec3 ambient = ( AmbientSettings + (1.0 - shadow)) * AlbedoConstant * AoConstant;
          //vec3 ambient = vec3(0.03) * AlbedoConstant * AoConstant;
          vec3 color = ambient + Lo;
          color = color / (color + vec3(1.0));  
          color = pow(color, vec3(1.0/2.2)); 
          FragColor = vec4(color, Transparency);      
        }
        }
}