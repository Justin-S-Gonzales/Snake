#version 330

layout (location = 0) in vec3 vertex;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}