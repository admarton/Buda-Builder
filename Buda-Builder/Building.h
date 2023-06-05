#pragma once
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"
#include "includes/Mesh_OGL3.h"
#include "Rect.hpp"

enum class BuildingType { StudioFlat, House, FamiliyHouse, Tower, BlockHouse };

class Building
{
public:
	Building(float x, float z, BuildingType type = BuildingType::StudioFlat);

	const BuildingType type;
	const float x, z;

	void Draw();
	static Rect GetBoundingArea(float x, float z, BuildingType type);
	Rect GetBoundingArea() const { return GetBoundingArea(x, z, type); };
	static Rect GetFoundationArea(float x, float z, BuildingType type);
	Rect GetFoundationArea() const { return GetFoundationArea(x, z, type); };

protected:
	GLsizei numVertices;
	std::unique_ptr<Mesh> mesh;

	void InitStudioFlat();
	void InitHouse();
	void InitFamilyHouse();
	void InitTower();
	void InitBlockHouse();

	void InitBox(float width, float height, float length, bool withRoof = true);
};

class BuildingContainer
{
public:
	BuildingContainer();

	void InitShaders();
	void InitTextures();

	void Draw(glm::mat4 viewProj);
	bool CanBuildingBeAdded(Rect area);
	void AddBuilding(float x, float z, BuildingType type = BuildingType::StudioFlat);

	size_t size() { return buildings.size(); }

	ProgramObject program;
protected:
	Texture2D texture;
	std::vector<Building> buildings;
};

