R"zzz(#version 330 core
uniform vec3 camera_position;
in vec4 f_color;
in vec3 tri_vertex;
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 camera_direction;
in vec2 uv_coords;
in vec4 world_position;
out vec4 fragment_color;


float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


vec2[256] get_gradient(vec4 light_direction) {
	vec2[256] gradient_grid;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			gradient_grid[j*16+i] = normalize(vec2(rand(vec2(i+light_direction.x, i+light_direction.y)), rand(vec2(j+light_direction.z, j+light_direction.x))));
		}
	}
	return gradient_grid;
}

float interpolate(float a, float b, float factor) {
    return (1.0 - factor) * a + factor * b;
}
float dot_gradient(int xi, int yi, float xf, float yf) {
    float xd = xf - float(xi);
    float yd = yf - float(yi);

    float fin = dot(vec2(xd, yd), normalize(
		vec2(
			sin(rand(vec2(xi, yi))),
			cos(rand(vec2(xi, yi))))));
    // std::cout << fin << "\n";
    return fin;

}
float perlin(float x, float y) {
    x = mod(x, 16);
    y = mod(y, 16);
    
    //Which grid cells?
    int x0 = int(floor(x));
    int x1 = x0 + 1;
    int y0 = int(floor(y));
    int y1 = y0 + 1;

    //Interpolation weights
    float wx = x -  float(x0);
    float wy =  y - float(y0);

    float g0 = dot_gradient(x0, y0, x, y);
    float g1 = dot_gradient(x1, y0, x, y);
    
    float i0 = interpolate(g0, g1, wx);
    
    float g2 = dot_gradient(x0, y1, x, y);
    float g3 = dot_gradient(x1, y1, x, y);
    
    float i1 = interpolate(g2, g3, wx);
    
    float fin = interpolate(i0, i1, wy);
    
    return fin;
}

void main() {
	//  color
	float closest = min(tri_vertex.x, min(tri_vertex.y, tri_vertex.z));
	
	float thresh = .01;

	float u = 0;
	float v = 0;
	bool soil = true;
	if (abs(face_normal.y - 1.0) < 0.01 || abs(1.0 + face_normal.y ) < 0.01) {
		u = world_position.x;
		v = world_position.z;
		soil = false;
	} else if (abs(face_normal.z - 1.0) < 0.01 || abs(1.0 + face_normal.z ) < 0.01) {
		u = world_position.x;
		v = world_position.y;
	} else if (abs(face_normal.x - 1.0) < 0.01 || abs(1.0 + face_normal.x ) < 0.01) {
		u = world_position.y;
		v = world_position.z;
	}
   	bool water  = false;
	float perlin_uv = perlin(u, v);
	if (false && closest  < thresh) {
		
		fragment_color = vec4(0.0, 0.0, 1.0, 1.0);
	}else {
		vec3 color = vec3(.1643, .6715, .1643);

		//float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
		//dot_nl = clamp(dot_nl, 0.0, 1.0);
		//color = clamp(dot_nl * color, 0.0, 1.0);
		//color[0] = color[0] * 8 * perlin_uv;
		//color[1] = color[1] * 8 * perlin(v, u+v);
		//color[2] = color[2] * 8 * perlin(v+u, u-v);
		float x = tri_vertex.x;
		float y = tri_vertex.y;
		if (world_position.y > 10) {
			color = vec3(0, .3216, .8314);
			color =(1.0 - clamp(perlin_uv, 0, .99)) * 1.8 * color;

		} else if (world_position.y > 5) {
			bool stone = false;
						float xPeriod = 5.0; //defines repetition of marble lines in x direction
			float yPeriod = 10.0; //defines repetition of marble lines in y direction
			//turbPower = 0 ==> it becomes a normal sine pattern
			float turbPower = 65.0; //makes twists
			float turbSize = 32.0; //initial size of the turbulence

			//if (mod(floor(world_position.x), 4) == 0 && mod(floor(world_position.z), 4) == 0) {
				float fl = perlin(floor(1 + world_position.x)*2/100.0, floor(1 + world_position.z)*2/100.0);
				if (mod(floor(1 + world_position.x)*2, floor(50*fl)) == 0 && mod(floor(1 + world_position.z)*2, floor(50*fl)) == 0) {
					soil = true;
					yPeriod = 30;
					turbPower = 20;
				} else if (mod(floor(1 + world_position.x)*2, floor(75*fl)) == 0 && mod(floor(1+world_position.z)*2, floor(75*fl)) == 0) {
					stone = true;
					turbPower = 1000;
				}
			//}

	    	float xyValue = x * xPeriod / 2 + y * yPeriod / 2 + turbPower * perlin_uv ;
    		float sineValue =  abs(sin(xyValue * 3.14159));
			if (soil) {
				color = vec3(117.0/256.0, 82.0/256.0, 54.0/256.0) * clamp(sineValue, .65, 1);

			} else if (stone) {
				color = vec3(169.0/256.0, 163.0/256.0, 163.0/256.0) * clamp(sineValue, .7, 1);

			} else {
				color = vec3(94.0/256.0, 157.0/256.0, 52.0/256.0) * clamp(sineValue, .7, 1);
			}
			//color = clamp(perlin_uv, .05, 1) * 5 * color;

		}else if (world_position.y > 2) {
			color = vec3(0, .7373, .8314);
			color = clamp(perlin_uv, .1,1) * 5 * color;
			water = true;
		}else if (world_position.y > 0) {
			color = vec3(205.0/256.0, 194.0/256.0, 153.0/256.0);
			color = clamp(perlin_uv, .1,1) * 5 * color;
		}

		//fragment_color = vec4(color, 1.0);
		//fragment_color = vec4(u, v, 0.0, 1.0);
		float dot_nl = dot(normalize(light_direction), normalize(face_normal));
		dot_nl = clamp(dot_nl, 0.0, 1.0);

		vec4 eye = vec4(camera_position, 1.0);

		vec4 ambient = vec4(1.0,1.0,1.0, 1.0) * 0.17;
		vec4 emissive = vec4(0.0,0.306,0.22, 1.0) * 0.20;
		vec4 diffuse = vec4(color, 1.0);
		vec4 r = reflect(normalize(-light_direction), normalize(face_normal));
		vec4 v = eye - world_position;
		float dot_vr = dot(normalize(v), normalize(r));
		dot_vr = clamp(dot_vr, 0.0, 1.0);
		vec4 specular = vec4(0.318,0.498,0.098,1.0) * dot_vr;
		//vec4 color_f =  vec4(1.0,1.0,1.0,1.0) * (diffuse + specular );
		fragment_color = clamp(ambient + dot_nl * vec4(color, 1.0), 0.0, 1.0);
		fragment_color[3] = 1.0;
		if (water) {
			fragment_color[3] = .95;
		}
	}
}
)zzz"
