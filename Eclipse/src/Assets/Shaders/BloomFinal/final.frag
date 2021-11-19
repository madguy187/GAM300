#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

vec3 aces(vec3 x){
  const float a=2.51;
  const float b=.03;
  const float c=2.43;
  const float d=.59;
  const float e=.14;
  return clamp((x*(a*x+b))/(x*(c*x+d)+e),0.,1.);
}

//void main()
//{
//  vec3 hdrColor = texture(scene,TexCoords).rgb;
//  vec3 bloomColor = texture(bloomBlur,TexCoords).rgb;
//  hdrColor += bloomColor;
//  
//  vec3 result = aces(hdrColor*exposure);
//  FragColor = vec4(result,1.);
//} 

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    
	if(bloom)
	{
	  hdrColor += bloomColor; // additive blending
	}
	
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);    
    result = pow(result, vec3(1.0 / 1.0));
    FragColor = vec4(result, 1.0);
}