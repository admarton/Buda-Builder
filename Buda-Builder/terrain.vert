#version 330 core

// VBO-ból érkezõ változók
in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader külsõ paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

void main()
{
	vec3 pos = vec3(vs_in_tex.x, 0, vs_in_tex.y);
	vec3 norm = vec3(0,1,0);
	gl_Position = MVP * vec4( pos, 1 );
	
	vs_out_pos = (world * vec4(pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}
