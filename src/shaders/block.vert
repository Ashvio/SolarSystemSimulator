R"zzz(
#version 330 core
uniform vec4 light_position;
uniform vec3 camera_position;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
in vec4 vertex_position;
in vec4 instance_center_position;
out vec4 vs_light_direction;
out vec4 vs_normal;
out vec2 vs_uv;
out vec4 vs_camera_direction;
out vec4 center;



void main() {
	gl_Position =  vec4((instance_center_position.xyz + vertex_position.xyz), 1.0);
  //  if (abs(vertex_position.x) < .01 || abs(vertex_position.y) < .01 || abs(vertex_position.z) < 0.01) {
    //    color = vec4(0.0, 0.0, 0.0, 1.0);
    //} else {
    //}
    vs_light_direction = light_position - gl_Position;
	//vs_camera_direction = vec4(camera_position, 1.0) - gl_Position;
	//vs_normal = normal;
	//vs_uv = uv;
    center = instance_center_position;
}
)zzz"
