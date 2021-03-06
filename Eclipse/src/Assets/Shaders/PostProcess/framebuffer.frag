#version 450 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform int Type;

const float offset_x = 1.0f / 800.0f;  
const float offset_y = 1.0f / 800.0f;  

uniform int Height;
uniform int Width;
uniform float FadeInTimer;
uniform float iTime;

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)
{
	float w = 1.0 / Width;
	float h = 1.0 / Height;

	n[0] = texture(tex, coord + vec2( -w, -h));
	n[1] = texture(tex, coord + vec2(0.0, -h));
	n[2] = texture(tex, coord + vec2(  w, -h));
	n[3] = texture(tex, coord + vec2( -w, 0.0));
	n[4] = texture(tex, coord);
	n[5] = texture(tex, coord + vec2(  w, 0.0));
	n[6] = texture(tex, coord + vec2( -w, h));
	n[7] = texture(tex, coord + vec2(0.0, h));
	n[8] = texture(tex, coord + vec2(  w, h));
}


float kernel[9] = float[]
(
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

float blurkernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

const float amountMin = 0.0;
const float amountMax = 100.0;

const vec3 luminanceWeighting = vec3(0.5, 0.0, 0.0);

vec3 tintAdjust(vec3 color, vec3 mapBlackTo, vec3 mapWhiteTo, float amount) 
{
    float luminance = dot(color, luminanceWeighting);
    return mix(color, mix(mapBlackTo, mapWhiteTo, luminance), amount);
}

const float flashlightRange = 0.4;
const float rotationSpeed = 0.4;
const vec2 rotationOrigin = vec2( 0.5, 0.5 );

vec2 rotateVertex( in vec2 _vertex, in vec2 _origin, in float _angle )
{
    vec2 result;
    
    //result.x = _origin.x + ( _vertex.x - _origin.x ) * cos( _angle ) - ( _vertex.y - _origin.y ) * sin( _angle );
	//result.y = _origin.y + ( _vertex.y - _origin.y ) * cos( _angle ) + ( _vertex.x - _origin.x ) * sin( _angle );
    
    return result;
}

float mouseSize = 0.5;
float smoothObjectPadding = 0.33;

void main()
{
    if(Type ==  1)
    {
        FragColor = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);
    }
    else if( Type == 2 )
    {
        FragColor = texture(screenTexture, texCoords);
        float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
        FragColor = vec4(average, average, average, 1.0);    
    }
    else if( Type == 3)
    {
        vec3 color = vec3(0.0f);

        for(int i = 0; i < 9; i++)
        {
          color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];
        }

        FragColor = vec4(color, 1.0f);
    }
    else if( Type == 4)
    {		
        vec3 color = vec3(0.0f);
		
        for(int i = 0; i < 9; i++)
        {
          color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * blurkernel[i];
        }
		
        FragColor = vec4(color, 1.0f);
    }
     else if( Type == 5)
    {		
		vec4 n[9];
		make_kernel( n, screenTexture, texCoords.st );	
		vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
		vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
        vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));
        float average = 0.2126 * sobel.r + 0.7152 * sobel.g + 0.0722 * sobel.b;
        FragColor = vec4(vec3(average), FadeInTimer );
    }
    else if( Type == 6)
    {	
        vec2 uv = texCoords/vec2(1270,593);
        vec4 c = texture(screenTexture, texCoords);

        float value = abs(sin(iTime * 0.5));
        float amount = mix(amountMin, amountMax, value);
        float amountNormal = amount/(amountMax - amountMin);
        float targetAmount = mix(0.0, 1.0, amountNormal);
        
        vec4 mapBlackTo = vec4(1, 0, 0, 1);
        vec4 mapWhiteTo = vec4(0, 0, 1, 1);
        
        c.rgb = tintAdjust(c.rgb, mapBlackTo.rgb, mapWhiteTo.rgb, targetAmount);
        
        FragColor = c;
    }

}