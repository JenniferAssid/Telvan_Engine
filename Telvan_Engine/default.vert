#version 330

uniform mat4 WorldView, WorldProj, ModelTr;

in vec3 position;
in vec3 in_color;

out vec4 color;

void main()
{
	gl_Position = WorldProj*WorldView*ModelTr*vec4(position,1.0);

	color = vec4(in_color, 1.0);
}