#version 460 core
out vec4 FragColor;

void main() 
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);

	if(FragColor.a == 0.0)
	{
	discard;
	}
}