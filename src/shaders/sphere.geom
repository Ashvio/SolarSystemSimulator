R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in vec4 vs_light_direction[];
in vec4 vs_normal[];
in vec2 vs_uv[];
in vec4 vs_p_position[];
in vec4 vs_p_color[];
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform int use_color;
uniform float radius;
uniform float scaleFactor;
uniform vec4 planet_position;
out vec4 light_direction;
out vec4 vertex_normal;
out vec2 uv_coords;
out vec4 particle_color;
void main() {
	for (int n = 0; n < gl_in.length(); n++) {
		vec4 pos = gl_in[n].gl_Position;
		vec4 center = vec4(0.0, 0.0, 0.0, 1.0);
		vec4 equidistant = normalize(pos - center) * radius * scaleFactor;
		if (use_color == 0) {
			gl_Position = projection * view * model * (planet_position + equidistant);
		} else {
			gl_Position = projection * view * model * (vs_p_position[n] + equidistant);
		}
		light_direction = normalize(vs_light_direction[n]);
		vertex_normal = vs_normal[n];
		uv_coords = vs_uv[n];
		particle_color = vs_p_color[n];
        EmitVertex();
	}
	EndPrimitive();
}
)zzz"
