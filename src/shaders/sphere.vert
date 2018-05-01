R"zzz(#version 330 core
in vec4 vertex_position;
in vec4 normal;
uniform vec4 light_position;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
out vec4 vs_light_direction;
out vec4 vs_normal;
void main()
{
	gl_Position = vertex_position;
	vs_light_direction = light_position - gl_Position;
	vs_normal = normal;
}
)zzz"