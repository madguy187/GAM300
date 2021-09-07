#version 450 core

layout (location=0) in vec2 TxtCoord;
layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec4 uColor;
uniform bool uTextureCheck;
in vec3 crntPos;
uniform vec3 lightPos;
in vec3 normal_from_vtxShader;
uniform vec3 camPos;
uniform vec4 sdiffuse;
uniform vec4 sspecular;
uniform bool TEST;
uniform bool useBlinn;
uniform float gamma;
uniform bool EnableGammaCorrection;
uniform bool CheckApplyLighting;

// Structs
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform int noTex;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float MaximumShininess;
}; 
  
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

#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int NumberOfPointLights;

#define NR_SPOTLIGHTS 4  
uniform SpotLight spotLights[NR_SPOTLIGHTS];
uniform int NumberOfSpotLights;

#define NR_DIRECTIONAL_LIGHTS 1  
uniform DirectionalLight directionlight[NR_DIRECTIONAL_LIGHTS];

uniform Material material;

// Function Headers
vec3 CalcPointLight(PointLight light, vec3 normala, vec3 fragPos, vec3 viewDira, vec4 texDiff, vec4 texSpec);
vec3 CalcDirLight(DirectionalLight light, vec3 normala, vec3 viewDira, vec4 texDiff, vec4 texSpec);
vec3 CalcSpotLight(SpotLight light, vec3 normala, vec3 fragPos, vec3 viewDira , vec4 texDiff, vec4 texSpec);


void main () 
{
	if(!uTextureCheck)
	{
		fFragClr = vec4(uColor) ;
	}
	else
	{
  
     vec3 result;

     // properties
     vec3 norm = normalize(normal_from_vtxShader);
     vec3 viewDir = normalize(camPos - crntPos);

    vec4 texDiff;
	vec4 texSpec;

	if (noTex == 1) 
    {
      	//texDiff = texture(uTex2d, TxtCoord);
    	//texSpec = texture(uTex2d, TxtCoord);

		texDiff = texture(uTex2d, TxtCoord) * sdiffuse;
		texSpec = texture(uTex2d, TxtCoord) * sspecular;      
	} 
    else 
    {
		texDiff = texture(diffuse0, TxtCoord);
		texSpec = texture(specular0, TxtCoord);
	}

    if( CheckApplyLighting == true )
    {
        if( directionlight[0].AffectsWorld == true )
        {
            result = CalcDirLight(directionlight[0], norm, viewDir,texDiff, texSpec);
        }

        for(int i = 0 ; i < NumberOfPointLights ; i++ )
        {
             result += CalcPointLight( pointLights[i], norm, crntPos, viewDir , texDiff, texSpec);
        }

        for(int i = 0 ; i < NumberOfSpotLights ; i++ )
        {
             result += CalcSpotLight( spotLights[i], norm, crntPos, viewDir , texDiff, texSpec);
        }
    }

        fFragClr = vec4(result,1.0f);

        if(EnableGammaCorrection == true )
        {
            fFragClr.rgb = pow(fFragClr.rgb, vec3(1.0/gamma));
        }
        
    }
}


// Pointllght Calculation
vec3 CalcPointLight(PointLight light, vec3 normala, vec3 fragPos, vec3 viewDira , vec4 texDiff, vec4 texSpec)
{
	// ambient
	vec4 ambient = vec4(light.ambient,1.0) * texDiff;

	// diffuse
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normala, lightDir), 0.0);
	vec4 diffuse = vec4(light.diffuse,1.0) * (diff * texDiff);

	// specular
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	if (diff > 0) 
    {
		float dotProd = 0.0;

		if (useBlinn) 
        {
			vec3 halfwayDir = normalize(lightDir + viewDira);
			dotProd = dot(normala, halfwayDir);
		}
		else
        {
			// calculate using Phong model
			vec3 reflectDir = reflect(-lightDir, normala);
			dotProd = dot(viewDira, reflectDir);
		}

		float spec = pow(max(dotProd, 0.0), material.shininess * material.MaximumShininess);
		specular = vec4(light.specular,1.0) * (spec * texSpec);
	}

	float dist = length(light.position - fragPos);
    float constant = light.constant;
	float linear = light.linear;
    float quadratic = light.quadratic;
	float attenuation = light.IntensityStrength / (constant + linear * dist + quadratic * ( dist * dist ) );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

    return ( vec3(ambient) + vec3(diffuse) + vec3(specular) ) * vec3(light.lightColor);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normala, vec3 fragPos, vec3 viewDira , vec4 texDiff, vec4 texSpec )
{
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));

    // ambient
	vec4 ambient = vec4(light.ambient,1.0) * texDiff;


    if (theta > light.outerCutOff) 
    {
     
      // diffuse
	  float diff = max(dot(normala, lightDir), 0.0);
	  vec4 diffuse = vec4(light.diffuse,1.0) * (diff * texDiff);

      vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

        if (diff > 0) 
        {
          float dotProd = 0.0;

			if (useBlinn) 
            {
				vec3 halfwayDir = normalize(lightDir + viewDira);
				dotProd = dot(normala, halfwayDir);
            }
            else
            {
  				vec3 reflectDir = reflect(-lightDir, normala);
				dotProd = dot(viewDira, reflectDir);          
            }

			float spec = pow(max(dotProd, 0.0), material.shininess * material.MaximumShininess);
			specular = vec4(light.specular,1.0) * (spec * texSpec);
        }

		// calculate Intensity
		float intensity = clamp((theta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);
		diffuse *= intensity;
		specular *= intensity;

		float dist = length(light.position - fragPos);
	    float constant = light.constant;
	    float linear = light.linear;
        float quadratic = light.quadratic;
	    float attenuation = light.IntensityStrength / (constant + linear * dist + quadratic * ( dist * dist ) );

		// apply attenuation
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

        return ( vec3(ambient) + vec3(diffuse) + vec3(specular) )  * light.lightColor ;
     }
     else
     {
          return vec3(ambient) * light.SurroundingAttenuationLevel;
     }
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirectionalLight light, vec3 normala, vec3 viewDira , vec4 texDiff, vec4 texSpec)
{
	// ambient
	vec3 ambient = light.ambient * vec3(texDiff);

	// diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normala, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texDiff));

    // specular
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

	if (diff > 0) 
    {
		// if diff <= 0, object is "behind" light

		float dotProd = 0.0;

		if (useBlinn) 
        {
			vec3 halfwayDir = normalize(lightDir + viewDira);
			dotProd = dot(normala, halfwayDir);
		}
		else 
        {
			// calculate using Phong model
			vec3 reflectDir = reflect(-lightDir, normala);
			dotProd = dot(viewDira, reflectDir);
		}

		float spec = pow(max(dotProd, 0.0), material.shininess * material.MaximumShininess);
		specular = vec4(light.specular,1.0) * (spec * texSpec);
	}

    return ( vec3(ambient) + vec3(diffuse) + vec3(specular) ) * vec3(light.lightColor);
}
