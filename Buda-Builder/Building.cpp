#include "Building.h"
#include "includes/ObjParser_OGL3.h"

Building::Building(float x, float z, Type type)
	: x{ x }, z{ z }, type{ type }
{
	InitShaders();
	InitTextures();
	switch (type)
	{
	case Building::Type::StudioFlat:
		InitStudioFlat();
		break;
	case Building::Type::House:
		break;
	case Building::Type::FamiliyHouse:
		break;
	case Building::Type::Tower:
		break;
	case Building::Type::BlockHouse:
		break;
	default:
		break;
	}
}

void Building::Draw()
{
	program.SetTexture("texImage", 0, texture);

	mesh->draw();
}

void Building::InitShaders()
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

void Building::InitTextures()
{
	texture.FromFile("assets/House1_Diffuse.png");
}

void Building::InitStudioFlat()
{
	mesh = std::make_unique<Mesh>();

	// Front wall
	mesh->addVertex({ glm::vec3(-1.0, -0.0, +1.0), glm::vec3(0, 0, 1), glm::vec2(0.0, 0) });
	mesh->addVertex({ glm::vec3(+1.0, -0.0, +1.0), glm::vec3(0, 0, 1), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3(-1.0, +1.0, +1.0), glm::vec3(0, 0, 1), glm::vec2(0.0, 1) });
	mesh->addVertex({ glm::vec3(+1.0, +1.0, +1.0), glm::vec3(0, 0, 1), glm::vec2(0.5, 1) });

	// Back wall
	mesh->addVertex({ glm::vec3(+1.0, -0.0, -1.0), glm::vec3(0, 0, -1), glm::vec2(0.0, 0) });
	mesh->addVertex({ glm::vec3(-1.0, -0.0, -1.0), glm::vec3(0, 0, -1), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3(+1.0, +1.0, -1.0), glm::vec3(0, 0, -1), glm::vec2(0.0, 1) });
	mesh->addVertex({ glm::vec3(-1.0, +1.0, -1.0), glm::vec3(0, 0, -1), glm::vec2(0.5, 1) });

	// Left wall
	mesh->addVertex({ glm::vec3(+1.0, -0.0, +1.0), glm::vec3(1, 0, 0), glm::vec2(0.0, 0) });
	mesh->addVertex({ glm::vec3(+1.0, -0.0, -1.0), glm::vec3(1, 0, 0), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3(+1.0, +1.0, +1.0), glm::vec3(1, 0, 0), glm::vec2(0.0, 1) });
	mesh->addVertex({ glm::vec3(+1.0, +1.0, -1.0), glm::vec3(1, 0, 0), glm::vec2(0.5, 1) });

	// Right wall
	mesh->addVertex({ glm::vec3(-1.0, -0.0, -1.0), glm::vec3(-1, 0, 0), glm::vec2(0.0, 0) });
	mesh->addVertex({ glm::vec3(-1.0, -0.0, +1.0), glm::vec3(-1, 0, 0), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3(-1.0, +1.0, -1.0), glm::vec3(-1, 0, 0), glm::vec2(0.0, 1) });
	mesh->addVertex({ glm::vec3(-1.0, +1.0, +1.0), glm::vec3(-1, 0, 0), glm::vec2(0.5, 1) });

	// Roof
	mesh->addVertex({ glm::vec3(-1.0, +1.0, +1.0), glm::vec3(0, 1, 0), glm::vec2(0.5, 0) });
	mesh->addVertex({ glm::vec3(+1.0, +1.0, +1.0), glm::vec3(0, 1, 0), glm::vec2(1.0, 0) });
	mesh->addVertex({ glm::vec3(-1.0, +1.0, -1.0), glm::vec3(0, 1, 0), glm::vec2(0.5, 1) });
	mesh->addVertex({ glm::vec3(+1.0, +1.0, -1.0), glm::vec3(0, 1, 0), glm::vec2(1.0, 1) });

	for (int i = 0; i < 5 * 4; i += 4)
	{
		mesh->addIndex(i + 0);
		mesh->addIndex(i + 1);
		mesh->addIndex(i + 2);
		mesh->addIndex(i + 1);
		mesh->addIndex(i + 3);
		mesh->addIndex(i + 2);
	}

	mesh->initBuffers();
}
