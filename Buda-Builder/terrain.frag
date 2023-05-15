#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// ir�ny f�nyforr�s: f�ny ir�nya
uniform vec3 light_dir = vec3(-1,-1,-1);

// f�nytulajdons�gok: ambiens, diff�z, ...
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

uniform float n;
uniform float m;

uniform sampler2D heightMap;
uniform sampler2D patchMap;
uniform sampler2D grass1;
uniform sampler2D grass2;
uniform sampler2D grass3;
uniform sampler2D rock;

void main()
{

	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	
	float cosa = clamp(dot(normal, to_light), 0, 1);

	vec3 diffuse = cosa*Ld;

	vec4 patchMultiply = texture(patchMap, vs_out_tex);
	vec2 texCoord = vec2(n*vs_out_tex.x, m*vs_out_tex.y);
	vec4 patchColor = 
		patchMultiply.r*texture(grass1,texCoord)
		+patchMultiply.g*texture(grass2,texCoord)
		+patchMultiply.b*texture(grass3,texCoord);
	float steepness = clamp(dot(normal, vec3(0.0,1.0,0.0)), 0, 1);
	vec4 rockColor = texture(rock,texCoord);
	vec4 color = steepness*patchColor + (1-steepness)*rockColor;
	fs_out_col = vec4(ambient + diffuse, 1) * color;
}