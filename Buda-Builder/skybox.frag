#version 330 core

in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform vec4 color = vec4(0.6, 0.6, 0.6, 1);

void main()
{

	vec4 darkerColor = color - vec4(0.2, 0.2, 0.2, 0);
	vec4 lighterColor = color + vec4(0.2, 0.2, 0.2, 0);

	fs_out_col = mix( darkerColor, lighterColor, (1 + vs_out_pos.y)/2 );
}