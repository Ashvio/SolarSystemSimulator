R"zzz(#version 400 core
layout (vertices = 3) out;
uniform float tess_level_inner;
uniform float tess_level_outer;
//in vec4 vs_light_direction[];
//out vec4 tcs_vs_light_direction[];
void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	if (gl_InvocationID == 0) {
		gl_TessLevelInner[0] = tess_level_inner;
    	gl_TessLevelOuter[0] = tess_level_outer;
    	gl_TessLevelOuter[1] = tess_level_outer;
    	gl_TessLevelOuter[2] = tess_level_outer;
	}
	//tcs_vs_light_direction[gl_InvocationID] = vs_light_direction[gl_InvocationID];
}
)zzz";