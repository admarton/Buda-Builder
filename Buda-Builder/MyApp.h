#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

#include "Terrain.h"
#include "Building.h"
#include "FrameBuffer.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

	void PlaceBuilding(int x, int y);

protected:
	// shader variables
	ProgramObject		m_programSkybox;	// skybox shader

	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		

	gCamera				m_camera;

	TextureCubeMap		m_skyboxTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// terrain data
	float m_offsetX_h = 0.f;
	float m_offsetY_h = 0.f;
	float m_increment_h = 0.01f;
	float m_offsetX_p = 0.f;
	float m_offsetY_p = 0.f;
	float m_increment_p = 0.01f;
	Terrain m_terrain;

	// buildings
	BuildingContainer m_buildings;
	BuildingType m_buildingType = BuildingType::StudioFlat;

	// frame buffer
	FrameBuffer m_fbo;

	void InitShaders();
	void InitSkyBox();

	// cursor
	float m_cursor[2]{ 0.f,0.f };
	ProgramObject m_cursorProgram;
	Building m_cursorBuildings[5] = {
		{0,0,BuildingType::StudioFlat},
		{0,0,BuildingType::House},
		{0,0,BuildingType::FamiliyHouse},
		{0,0,BuildingType::Tower},
		{0,0,BuildingType::BlockHouse}
	};
	glm::vec4 m_cursorColor;
};

