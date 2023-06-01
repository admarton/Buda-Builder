#version 330 core

in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform vec4 sky_color = vec4(0.6, 0.6, 0.6, 1);
uniform vec4 star_color = vec4(1.0);
uniform vec3 light_dir = vec3(-1,-1,-1);

void main()
{
	vec4 darkerColor = sky_color - vec4(0.2, 0.2, 0.2, 0);
	vec4 lighterColor = sky_color + vec4(0.2, 0.2, 0.2, 0);

	float dist = distance(vs_out_pos,light_dir);
	float light_factor = smoothstep(0.05, 0.1, dist);
	dist = distance(vs_out_pos,-light_dir);
	light_factor *= smoothstep(0.05, 0.1, dist);

	fs_out_col = (1-light_factor)*star_color + light_factor*mix( darkerColor, lighterColor, (1 + vs_out_pos.y)/2 );
}