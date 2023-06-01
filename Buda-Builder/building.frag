#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D texImage;

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
uniform float shininess = 16.0;


vec3 GetAmbient() {
	return La * Ka;
}

vec3 GetDiffuse(vec3 normal, vec3 to_light, float attenuation) {
	float diffuse_factor =  clamp(dot(to_light,normal), 0.0, 1.0) * attenuation;
	return diffuse_factor * Ld * Kd;
}

vec3 GetSpecular(vec3 normal, vec3 to_light, float attenuation) {
	vec3 viewDir = normalize( camera_pos - vs_out_pos );
	vec3 reflectDir = reflect( -to_light, normal );

	float specular_factor = pow(clamp( dot( viewDir, reflectDir) ,0.0, 1.0), shininess) * attenuation;
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
	vec3 specular = GetSpecular(normal, to_light, attenuation) * 0.5 + 0.5;

	fs_out_col = vec4(ambient + diffuse + specular, 1) * texture(texImage, vs_out_tex);
}