#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 mcolor;

out vec3 shcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	gl_Position = perspective * view * model * vec4(pos,  1.0);
	shcolor = mcolor;
	
}