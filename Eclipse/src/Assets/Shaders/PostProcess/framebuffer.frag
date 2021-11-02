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

void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)
{
	float w = 1.0 / 300;
	float h = 1.0 / 300;

	n[0] = texture2D(tex, coord + vec2( -w, -h));
	n[1] = texture2D(tex, coord + vec2(0.0, -h));
	n[2] = texture2D(tex, coord + vec2(  w, -h));
	n[3] = texture2D(tex, coord + vec2( -w, 0.0));
	n[4] = texture2D(tex, coord);
	n[5] = texture2D(tex, coord + vec2(  w, 0.0));
	n[6] = texture2D(tex, coord + vec2( -w, h));
	n[7] = texture2D(tex, coord + vec2(0.0, h));
	n[8] = texture2D(tex, coord + vec2(  w, h));
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

    }
     else if( Type == 5)
    {		
		// Version one of sorbel
		vec4 n[9];
		make_kernel( n, screenTexture, texCoords.st );
		
		vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
		vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
		//vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));
		
        vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));
        float average = 0.2126 * sobel.r + 0.7152 * sobel.g + 0.0722 * sobel.b;
        FragColor = vec4(vec3(average), 1.0 );

		//FragColor = vec4(sobel.rgb, 1.0 );

    }

}