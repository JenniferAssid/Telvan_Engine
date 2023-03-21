#version 330 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D tex;
uniform vec3 tex_color;

void main()
{
	color = vec4(tex_color, 1.0) * texture(tex, texCoord);
}