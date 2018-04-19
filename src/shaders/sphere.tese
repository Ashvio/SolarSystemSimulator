R"zzz(#version 430 core
layout (triangles, equal_spacing, cw) in;
//in vec4 tcs_vs_light_direction[];
//out vec4 vs_light_direction;
void main(void){ 
	gl_Position=(gl_TessCoord.x*gl_in[0].gl_Position +
				 gl_TessCoord.y*gl_in[1].gl_Position +
				 gl_TessCoord.z*gl_in[2].gl_Position);

	//vs_light_direction = (gl_TessCoord.x*tcs_vs_light_direction[0] + 
	//					  gl_TessCoord.y*tcs_vs_light_direction[1] + 
	//					  gl_TessCoord.z*tcs_vs_light_direction[2]);
}
)zzz";