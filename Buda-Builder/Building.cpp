#include "Building.h"
#include "includes/ObjParser_OGL3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


BuildingContainer::BuildingContainer()
{
	InitShaders();
	InitTextures();
}

void BuildingContainer::InitShaders()
{
	program.Init(
		{
			{ GL_VERTEX_SHADER, "building.vert" },
			{ GL_FRAGMENT_SHADER, "building.frag" }
		},
		{
			{ 0, "vs_in_pos" },
			{ 1, "vs_in_norm" },
			{ 2, "vs_in_tex" },
		}
	);
}

void BuildingContainer::InitTextures()
{
	texture.FromFile("assets/House1_Diffuse.png");
}

void BuildingContainer::Draw(glm::mat4 viewProj)
{
	program.SetTexture("texImage", 1, texture);

	for (auto& building : buildings) {
		glm::mat4 buildingWorld = glm::translate(glm::vec3{ building.x,0.f,building.z }) * glm::mat4(1.f);
		program.SetUniform("MVP", viewProj * buildingWorld);
		program.SetUniform("world", buildingWorld);
		program.SetUniform("worldIT", glm::inverse(glm::transpose(buildingWorld)));
		program.SetUniform("x", building.x);
		program.SetUniform("z", building.z);
		building.Draw();
	}
}

bool BuildingContainer::CanBuildingBeAdded(Rect area)
{
	for (const auto& building : buildings) {
		if (intersect(area, building.GetBoundingArea())) return false;
	}
	return true;
}

void BuildingContainer::AddBuilding(float x, float z, BuildingType type)
{
	buildings.push_back(Building{ x,z,type });
}

Building::Building(float x, float z, BuildingType type)
	: x{ x }, z{ z }, type{ type }
{
	switch (type)
	{
	case BuildingType::StudioFlat:
		InitStudioFlat();
		break;
	case BuildingType::House:
		InitHouse();
		break;
	case BuildingType::FamiliyHouse:
		InitFamilyHouse();
		break;
	case BuildingType::Tower:
		InitTower();
		break;
	case BuildingType::BlockHouse:
		InitBlockHouse();
		break;
	default:
		break;
	}
}

void Building::Draw()
{
	mesh->draw();
}

Rect Building::GetBoundingArea(float x, float z, BuildingType type)
{
	glm::vec2 translate{ x,z };
	switch (type)
	{
	case BuildingType::FamiliyHouse:
		return Rect{4.f, -2.f, -6.f, 2.f} + translate;
	case BuildingType::BlockHouse:
		return Rect{3.f, -3.f, -2.f, 2.f} + translate;
	case BuildingType::StudioFlat:
	case BuildingType::House:
	case BuildingType::Tower:
	default:
		return Rect{2.f, -2.f, -2.f, 2.f} + translate;
	}
}

Rect Building::GetFoundationArea(float x, float z, BuildingType type)
{
	glm::vec2 translate{ x,z };
	switch (type)
	{
	case BuildingType::FamiliyHouse:
		return Rect{ 5.f, -3.f, -7.f, 3.f } + translate;
	case BuildingType::BlockHouse:
		return Rect{ 4.f, -4.f, -3.f, 3.f } + translate;
	case BuildingType::StudioFlat:
	case BuildingType::House:
	case BuildingType::Tower:
	default:
		return Rect{ 3.f, -3.f, -3.f, 3.f } + translate;
	}
}

void Building::InitStudioFlat()
{
	mesh = std::make_unique<Mesh>();

	InitBox(2.f, 1.f, 2.f);

	mesh->initBuffers();
}

void Building::InitHouse()
{
	mesh = std::make_unique<Mesh>();

	InitBox(2.f, 1.f, 2.f, false);

	// Roof
	int idx = 4 * 4;

	// Normals
	glm::vec3 north = glm::normalize(glm::vec3(  0.f, 1.f, -1.f ));
	glm::vec3 south = glm::normalize(glm::vec3(  0.f, 1.f,  1.f ));
	glm::vec3  east = glm::normalize(glm::vec3( -1.f, 1.f,  0.f ));
	glm::vec3  west = glm::normalize(glm::vec3(  1.f, 1.f,  0.f ));
	
	// Points
	glm::vec3    center{  0.f, 2.f,  0.f };
	glm::vec3   farLeft{  1.f, 1.f, -1.f };
	glm::vec3  farRight{ -1.f, 1.f, -1.f };
	glm::vec3  nearLeft{  1.f, 1.f,  1.f };
	glm::vec3 nearRight{ -1.f, 1.f,  1.f };

	// Texture
	glm::vec2 first{0.5f, 0.f};
	glm::vec2 second{1.0f, 0.f};
	glm::vec2 third{0.75f, 0.5f};

	// Triangle North
	mesh->addVertex({ farLeft, north, first });
	mesh->addVertex({ farRight, north, second });
	mesh->addVertex({ center, north, third });
	mesh->addIndex(idx++);
	mesh->addIndex(idx++);
	mesh->addIndex(idx++);

	// Triangle South
	mesh->addVertex({ nearRight, south, first });
	mesh->addVertex({ nearLeft, south, second });
	mesh->addVertex({ center, south, third });
	mesh->addIndex(idx++);
	mesh->addIndex(idx++);
	mesh->addIndex(idx++);

	// Triangle East
	mesh->addVertex({ farRight, east, first });
	mesh->addVertex({ nearRight, east, second });
	mesh->addVertex({ center, east, third });
	mesh->addIndex(idx++);
	mesh->addIndex(idx++);
	mesh->addIndex(idx++);

	// Triangle West
	mesh->addVertex({ nearLeft, west, first });
	mesh->addVertex({ farLeft, west, second });
	mesh->addVertex({ center, west, third });
	mesh->addIndex(idx++);
	mesh->addIndex(idx++);
	mesh->addIndex(idx++);

	mesh->initBuffers();
}

void Building::InitFamilyHouse()
{
	mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/FamilyHouse.obj"));

	mesh->initBuffers();
}

void Building::InitTower()
{
	mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Tower.obj"));

	mesh->initBuffers();
}

void Building::InitBlockHouse()
{
	mesh = std::make_unique<Mesh>();

	InitBox(4.f, 4.f, 2.f);

	mesh->initBuffers();
}

void Building::InitBox(float width, float height, float length, bool withRoof)
{
	if (!mesh) return;

	float bot = 0.f, top = height;
	float left = width / 2.f, right = width / -2.f;
	float near = length / 2.f, far = length / -2.f;

	// Front wall
	mesh->addVertex({ glm::vec3(right, bot, near), glm::vec3(0, 0, 1), glm::vec2(0.0, 0) });
	mesh->addVertex({ glm::vec3( left, bot, near), glm::vec3(0, 0, 1), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3(right, top, near), glm::vec3(0, 0, 1), glm::vec2(0.0, 1) });
	mesh->addVertex({ glm::vec3( left, top, near), glm::vec3(0, 0, 1), glm::vec2(0.5, 1) });

	// Back wall
	mesh->addVertex({ glm::vec3( left, bot,  far), glm::vec3(0, 0, -1), glm::vec2(0.0, 0) });
	mesh->addVertex({ glm::vec3(right, bot,  far), glm::vec3(0, 0, -1), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3( left, top,  far), glm::vec3(0, 0, -1), glm::vec2(0.0, 1) });
	mesh->addVertex({ glm::vec3(right, top,  far), glm::vec3(0, 0, -1), glm::vec2(0.5, 1) });

	// Left wall
	mesh->addVertex({ glm::vec3( left, bot, near), glm::vec3(1, 0, 0), glm::vec2(0.0, 0) });
	mesh->addVertex({ glm::vec3( left, bot,  far), glm::vec3(1, 0, 0), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3( left, top, near), glm::vec3(1, 0, 0), glm::vec2(0.0, 1) });
	mesh->addVertex({ glm::vec3( left, top,  far), glm::vec3(1, 0, 0), glm::vec2(0.5, 1) });

	// Right wall
	mesh->addVertex({ glm::vec3(right, bot,  far), glm::vec3(-1, 0, 0), glm::vec2(0.0, 0) });
	mesh->addVertex({ glm::vec3(right, bot, near), glm::vec3(-1, 0, 0), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3(right, top,  far), glm::vec3(-1, 0, 0), glm::vec2(0.0, 1) });
	mesh->addVertex({ glm::vec3(right, top, near), glm::vec3(-1, 0, 0), glm::vec2(0.5, 1) });

	// Roof
	if (withRoof) {
		mesh->addVertex({ glm::vec3(right, top, near), glm::vec3(0, 1, 0), glm::vec2(0.5, 0) });
		mesh->addVertex({ glm::vec3( left, top, near), glm::vec3(0, 1, 0), glm::vec2(1.0, 0) });
		mesh->addVertex({ glm::vec3(right, top,  far), glm::vec3(0, 1, 0), glm::vec2(0.5, 1) });
		mesh->addVertex({ glm::vec3( left, top,  far), glm::vec3(0, 1, 0), glm::vec2(1.0, 1) });
	}

	for (int i = 0; i < (4 + withRoof) * 4; i += 4)
	{
		mesh->addIndex(i + 0);
		mesh->addIndex(i + 1);
		mesh->addIndex(i + 2);
		mesh->addIndex(i + 1);
		mesh->addIndex(i + 3);
		mesh->addIndex(i + 2);
	}
}
