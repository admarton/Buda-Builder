#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec2 vs_in_tex;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader k�ls� param�terei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;


uniform float n;
uniform float m;
uniform float minHeight;
uniform float maxHeight;
uniform sampler2D heightMap;

void main()
{
	float heightDiff = maxHeight - minHeight;
	float height = texture(heightMap, vs_in_tex).r * heightDiff + minHeight;
	vec3 pos = vec3(vs_in_tex.x*n, height, vs_in_tex.y*m);
	
	float sx = 1.0/n;
	float sy = 1.0/m;
	vec2 uv = vs_in_tex;
	float u = texture(heightMap, uv + sx * vec2(0.0, -1.0)).r * heightDiff + minHeight;
    float r = texture(heightMap, uv + sx * vec2(-1.0, 0.0)).r * heightDiff + minHeight;
    float l = texture(heightMap, uv + sx * vec2(1.0, 0.0)).r * heightDiff + minHeight;
    float d = texture(heightMap, uv + sx * vec2(0.0, 1.0)).r * heightDiff + minHeight;

	vec3 norm = normalize(vec3(
		sx*(r-l),
		(sx+sy)/2.0,
		sy*(u-d)
	));

	gl_Position = MVP * vec4( pos, 1 );
	
	vs_out_pos = (world * vec4(pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}
