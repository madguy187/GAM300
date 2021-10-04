#version 450 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform int Type;

const float offset_x = 1.0f / 800.0f;  
const float offset_y = 1.0f / 800.0f;  

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

float kernel[9] = float[]
(
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

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
}