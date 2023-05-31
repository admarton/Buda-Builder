#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"
#include "Rect.hpp"

class Terrain
{
public:
	Terrain(GLuint n = 256, GLuint m = 256);
	~Terrain();

	void SetUniforms();
	void Draw();
	void FillHeightMap(float offsetX, float offsetY, float increment);
	void ChangeHeightMap(float offsetX, float offsetY, float increment);
	void UpdateHeightMap();
	void FillPatchMap(float offsetX, float offsetY, float increment);
	void ChangePatchMap(float offsetX, float offsetY, float increment);
	void UpdatePatchMap();
	void AddFoundation(const Rect& found);

	GLuint GetHeightTexture() { return heightTexture; }

	const GLuint n, m;
	ProgramObject program;
	float minHeight = -10.f, maxHeight = 40.f;
	float sandHeight = 5.f, snowHeight = 30.f;

protected:
	GLuint vertexArrayObject;
	GLuint vertexBuffer;
	GLuint indexBuffer;

	GLuint heightTexture;
	std::vector<float> heightMapData;
	GLuint patchTexture;
	std::vector<glm::vec4> patchMapData;
	Texture2D grass1;
	Texture2D grass2;
	Texture2D grass3;
	Texture2D paving;
	Texture2D rock;
	Texture2D snow;
	Texture2D sand;

	void InitSurface();
	void InitShaders();
	void InitTextures();

	std::vector<std::pair<Rect, float>> foundations;
	float ApplyFoundationOnHeightMap(const Rect& found);
	void ApplyFoundationOnPatchMap(const Rect& found);

	glm::vec3 Ka = glm::vec3(1.0);
	glm::vec3 Kd = glm::vec3(1.0);
	glm::vec3 Ks = glm::vec3(1.0);
	float shininess = 0.1;
	
};

