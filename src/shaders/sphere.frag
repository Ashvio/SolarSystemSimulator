R"zzz(#version 330 core
uniform sampler2D textureSampler;
uniform vec4 default_color;
uniform int use_color;
in vec4 vertex_normal;
in vec4 particle_color;
in vec4 light_direction;
in vec2 uv_coords;
layout(location = 0) out vec4 fragment_color;

void main() {
    vec3 texcolor = texture(textureSampler, uv_coords).xyz;
    if (length(texcolor) == 0.0) {
        //float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
	    //dot_nl = clamp(dot_nl, 0.0, 1.0);
        //vec3 color = default_color;
        //vec3 ambient = default_color * 0.3;
        //color = clamp(dot_nl * color + ambient, 0.0, 1.0);
        //fragment_color = vec4(color, 1.0);
        fragment_color = default_color;
        //fragment_color = vec4(vec3(vertex_normal), 1.0);
    } else if (use_color == 1) {
        fragment_color = particle_color;
    } else {
		fragment_color = vec4(texcolor.rgb, 1.0);
    }
}
)zzz"
