#pragma once
#include <GL/glew.h>
#include "includes/ProgramObject.h"

class Terrain
{
public:
	Terrain(GLuint n = 256, GLuint m = 256);
	~Terrain();

	void Draw();
	void FillHeightMap(float offsetX, float offsetY, float increment);
	void ChangeHeightMap(float offsetX, float offsetY, float increment);

	const GLuint n, m;
	ProgramObject program;

protected:
	GLuint vertexArrayObject;
	GLuint vertexBuffer;
	GLuint indexBuffer;

	GLuint heightTexture;
	std::vector<float> heightMapData;

	void InitSurface();
	void InitShaders();
	void InitTextures();
};

