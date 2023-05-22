#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader k�ls� param�terei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform float x;
uniform float z;

uniform float n;
uniform float m;
uniform float minHeight;
uniform float maxHeight;
uniform sampler2D heightMap;

void main()
{
	float heightDiff = maxHeight - minHeight;
	vec2 heightMapCoord = vec2(x/n, z/m);
	float height = texture(heightMap, heightMapCoord).r * heightDiff + minHeight;
	vec4 pos = vec4(vs_in_pos.x, vs_in_pos.y+height, vs_in_pos.z, 1);

	gl_Position = MVP * pos;
	
	vs_out_pos = (world * pos).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}