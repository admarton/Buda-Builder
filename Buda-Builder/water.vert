#version 330 core

in vec2 vs_in_tex;

out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform float n;
uniform float m;
uniform float waterHeight;

uniform float elapsed_time = 0.0;

vec3 GetPos(float u, float v)
{
	vec3 pos = vec3(u, 0.0, v);
	pos.y = sin(sqrt(pow(0.25*(u-2*n),2)+pow(0.25*(v-2*m),2))+elapsed_time) + cos(0.1*v+elapsed_time) + waterHeight;
	return pos;
}

vec3 GetNorm(float u, float v)
{
	vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
	vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

	return normalize(cross(du, dv));
}

vec2 GetTex(float u, float v)
{
	return vec2(u, v);
}

void main()
{
	vec3 vs_in_pos = GetPos(vs_in_tex.x*4*n, vs_in_tex.y*4*m);
	vec3 vs_in_norm = GetNorm(vs_in_tex.x*4*n, vs_in_tex.y*4*m);
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	
	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}
