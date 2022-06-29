#version 330 core

out vec4 fragcolor;

in vec3 shcolor;


void main()
{
	fragcolor = vec4(shcolor, 1);
} 