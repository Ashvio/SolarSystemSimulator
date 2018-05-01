R"zzz(#version 430 core
#define M_PI 3.1415926535897932384626433832795
layout (triangles, equal_spacing, cw) in;
in vec4 tcs_vs_light_direction[];
in vec4 tcs_normal[];
out vec4 vs_light_direction;
out vec4 vs_normal;
out vec2 vs_uv;
void main(void){ 
	gl_Position=(gl_TessCoord.x*gl_in[0].gl_Position +
				 gl_TessCoord.y*gl_in[1].gl_Position +
				 gl_TessCoord.z*gl_in[2].gl_Position);

	vs_light_direction = (gl_TessCoord.x*tcs_vs_light_direction[0] + 
						  gl_TessCoord.y*tcs_vs_light_direction[1] + 
						  gl_TessCoord.z*tcs_vs_light_direction[2]);
	vs_normal = (gl_TessCoord.x * tcs_normal[0] +
				 gl_TessCoord.y * tcs_normal[1] + 
				 gl_TessCoord.z * tcs_normal[2]);

	vec4 position = normalize(gl_Position);
	vs_uv[0] = atan(position[2], position[0])/(2.0*M_PI) + 0.5;
	vs_uv[1] = acos(position[1])/(M_PI);
	if (vs_uv[0] < 0.75 && vs_uv[1] > 0.75) {
		vs_uv[0] += 1.0;
	} else if (vs_uv[0] > 0.75 && vs_uv[0] < 0.75) {
		vs_uv[0] -= 1.0;
	}
}
)zzz";