#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

void main()
{
	fs_out_col = vec4(vs_out_tex, 0, 1);
}