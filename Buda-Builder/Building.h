#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"
#include "includes/Mesh_OGL3.h"
class Building
{
public:
	enum class Type { StudioFlat, House, FamiliyHouse, Tower, BlockHouse };

	Building(float x, float z, Type type = Type::StudioFlat);

	const Type type;
	const float x, z;
	ProgramObject program;

	void Draw();

	Texture2D texture;
protected:
	GLsizei numVertices;
	std::unique_ptr<Mesh> mesh;

	void InitShaders();
	void InitTextures();
	void InitStudioFlat();
};

