R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float radius;
void main() {
	for (int n = 0; n < gl_in.length(); n++) {
		vec4 position = gl_in[n].gl_Position;
		vec4 center = vec4(0.0, 0.0, 0.0, 1.0);
		vec4 equidistant = normalize(position - center) * 1.0;
		gl_Position = projection * view * model * (center + equidistant);
        EmitVertex();
	}
	EndPrimitive();
}
)zzz"
