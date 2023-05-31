#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform float n;
uniform float m;
uniform float sandHeight;
uniform float snowHeight;

uniform sampler2D heightMap;
uniform sampler2D patchMap;
uniform sampler2D grass1;
uniform sampler2D grass2;
uniform sampler2D grass3;
uniform sampler2D paving;
uniform sampler2D rock;
uniform sampler2D sand;
uniform sampler2D snow;

// Light calculations
uniform vec3 camera_pos;

// Light parameters
uniform vec3 light_dir = vec3(-1,-1,-1);

uniform vec3 La = vec3(0.0, 0.0, 0.0 );
uniform vec3 Ld = vec3(1.0, 1.0, 1.0 );
uniform vec3 Ls = vec3(1.0, 1.0, 1.0 );

uniform float lightConstantAttenuation    = 1.0;
uniform float lightLinearAttenuation      = 0.0;
uniform float lightQuadraticAttenuation   = 0.0;

// Material parameters
uniform vec3 Ka = vec3( 1.0 );
uniform vec3 Kd = vec3( 1.0 );
uniform vec3 Ks = vec3( 1.0 );
uniform float shininess = 0.1;


vec3 GetAmbient() {
	return La * Ka;
}

vec3 GetDiffuse(vec3 normal, vec3 to_light, float attenuation) {
	float diffuse_factor =  max(dot(to_light,normal), 0.0) * attenuation;
	return diffuse_factor * Ld * Kd;
}

vec3 GetSpecular(vec3 normal, vec3 to_light, float attenuation) {
	vec3 viewDir = normalize( camera_pos - vs_out_pos );
	vec3 reflectDir = reflect( -to_light, normal );

	float specular_factor = pow(max( dot( viewDir, reflectDir) ,0.0), shininess) * attenuation;
	return specular_factor*Ls*Ks;
}

void main()
{
	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	float light_distance = 0.0;
	float attenuation = 1.0 / ( lightConstantAttenuation + lightLinearAttenuation * light_distance + lightQuadraticAttenuation * light_distance * light_distance);

	vec3 ambient = GetAmbient();
	vec3 diffuse = GetDiffuse(normal, to_light, attenuation);
	vec3 specular = GetSpecular(normal, to_light, attenuation);

	vec4 patchMultiply = texture(patchMap, vs_out_tex);
	vec2 texCoord = vec2(vs_out_tex.x*(n/16), vs_out_tex.y*(m/16));
	vec4 color = 
		patchMultiply.r*texture(grass1,texCoord)
		+patchMultiply.g*texture(grass2,texCoord)
		+patchMultiply.b*texture(grass3,texCoord)
		+patchMultiply.a*texture(paving,texCoord);
	vec4 snowColor = texture(snow,texCoord);
	float snowStrength = clamp(vs_out_pos.y-snowHeight, 0, 2)/2.0;
	color = snowStrength*snowColor + (1-snowStrength)*color;
	vec4 sandColor = texture(sand,texCoord);
	float sandStrength = 1-clamp(vs_out_pos.y-sandHeight, 0, 1);
	color = sandStrength*sandColor + (1-sandStrength)*color;
	float steepness = clamp(dot(normal, vec3(0.0,1.0,0.0))*2.0, 0, 1);
	vec4 rockColor = texture(rock,texCoord);
	color = steepness*color + (1-steepness)*rockColor;

	fs_out_col = vec4(ambient + diffuse + specular, 1) * color;
}