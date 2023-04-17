#version 330 core

layout (location = 0) in vec4 vertex; // <pos x, pos y, texCoord x, texCoord y

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
}