R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in vec4 vs_light_direction[];
in vec4 vs_normal[];
in vec2 vs_uv[];
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float radius;
uniform float scaleFactor;
uniform vec4 planet_position;
out vec4 light_direction;
out vec4 vertex_normal;
out vec2 uv_coords;
void main() {
	for (int n = 0; n < gl_in.length(); n++) {
		vec4 pos = gl_in[n].gl_Position;
		vec4 center = vec4(0.0, 0.0, 0.0, 1.0);
		vec4 equidistant = normalize(pos - center) * radius * scaleFactor;
		gl_Position = projection * view * model * (planet_position + equidistant);
		light_direction = normalize(vs_light_direction[n]);
		vertex_normal = vs_normal[n];
		uv_coords = vs_uv[n];
        EmitVertex();
	}
	EndPrimitive();
}
)zzz"
