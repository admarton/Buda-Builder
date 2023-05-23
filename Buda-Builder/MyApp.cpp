#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(130, 20, 130), glm::vec3(125, 0, 125), glm::vec3(0, 1, 0));
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// back face
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// front face
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// back face
			0, 1, 2,
			2, 3, 0,
			// front face
			4, 6, 5,
			6, 4, 7,
			// left
			0, 3, 4,
			4, 3, 7,
			// right
			1, 5, 2,
			5, 6, 2,
			// bottom
			1, 0, 4,
			1, 4, 5,
			// top
			3, 2, 6,
			3, 6, 7,
	}
	);

	// Register geometry in VAO
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_skyboxTexture.AttachFromFile("assets/xpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	m_skyboxTexture.AttachFromFile("assets/xneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_skyboxTexture.AttachFromFile("assets/ypos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/yneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/zpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_skyboxTexture.AttachFromFile("assets/zneg.png", true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CMyApp::InitShaders()
{
	// Cursor
	m_cursorProgram.Init(
		{
			{ GL_VERTEX_SHADER, "building.vert" },
			{ GL_FRAGMENT_SHADER, "cursor.frag" }
		},
		{
			{ 0, "vs_in_pos" },
			{ 1, "vs_in_norm" },
			{ 2, "vs_in_tex" },
		}
	);

	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },
		}
	);
}

bool CMyApp::Init()
{
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); 
	glEnable(GL_DEPTH_TEST);

	InitShaders();
	InitSkyBox();

	// kamera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);

	m_fbo.CreateFrameBuffer(1080,720);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	glm::mat4 viewProj = m_camera.GetViewProj();

	// Draw to the FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo.GetFrameBuffer());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Terrain
	glm::mat4 terrainWorld = glm::mat4(1.f);
	m_fbo.m_terrainProgram.Use();
	m_fbo.m_terrainProgram.SetUniform("MVP", viewProj * terrainWorld);
	m_fbo.m_terrainProgram.SetUniform("world", terrainWorld);
	m_fbo.m_terrainProgram.SetUniform("worldIT", glm::inverse(glm::transpose(terrainWorld)));
	m_fbo.m_terrainProgram.SetUniform("n", (float)m_terrain.n);
	m_fbo.m_terrainProgram.SetUniform("m", (float)m_terrain.m);
	m_fbo.m_terrainProgram.SetUniform("minHeight", m_terrain.minHeight);
	m_fbo.m_terrainProgram.SetUniform("maxHeight", m_terrain.maxHeight);
	m_fbo.m_terrainProgram.SetTexture("heightMap", 0, m_terrain.GetHeightTexture());
	m_terrain.Draw();
	m_fbo.m_terrainProgram.Unuse();

	// Draw to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Terrain
	m_terrain.program.Use();
	m_terrain.program.SetUniform("MVP", viewProj * terrainWorld);
	m_terrain.program.SetUniform("world", terrainWorld);
	m_terrain.program.SetUniform("worldIT", glm::inverse(glm::transpose(terrainWorld)));
	m_terrain.SetUniforms();
	m_terrain.Draw();
	m_terrain.program.Unuse();

	//Buildings
	if (0 < m_buildings.size()) {
		m_buildings.program.Use();
		m_buildings.program.SetUniform("n", (float)m_terrain.n);
		m_buildings.program.SetUniform("m", (float)m_terrain.m);
		m_buildings.program.SetUniform("minHeight", m_terrain.minHeight);
		m_buildings.program.SetUniform("maxHeight", m_terrain.maxHeight);
		m_buildings.program.SetTexture("heightMap", 0, m_terrain.GetHeightTexture());
		m_buildings.Draw(viewProj);
		m_buildings.program.Unuse();
	}

	//Cursor
	glm::mat4 buildingWorld = glm::translate( glm::vec3{ m_cursor[0],0.f,m_cursor[1] }) * glm::mat4(1.f);
	m_cursorProgram.Use();
	m_cursorProgram.SetUniform("n", (float)m_terrain.n);
	m_cursorProgram.SetUniform("m", (float)m_terrain.m);
	m_cursorProgram.SetUniform("minHeight", m_terrain.minHeight);
	m_cursorProgram.SetUniform("maxHeight", m_terrain.maxHeight);
	m_cursorProgram.SetTexture("heightMap", 0, m_terrain.GetHeightTexture());
	m_cursorProgram.SetUniform("color", m_cursorColor);
	m_cursorProgram.SetUniform("MVP", viewProj * buildingWorld);
	m_cursorProgram.SetUniform("world", buildingWorld);
	m_cursorProgram.SetUniform("worldIT", glm::inverse(glm::transpose(buildingWorld)));
	m_cursorProgram.SetUniform("x", m_cursor[0]);
	m_cursorProgram.SetUniform("z", m_cursor[1]);
	m_cursorBuildings[(int)m_buildingType].Draw();
	m_cursorProgram.Unuse();

	//Skybox
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate( m_camera.GetEye()) );
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	glDepthFunc(prevDepthFnc);

	{
		ImGui::Begin("Camera controls");
		ImGui::Text("Move:");
		ImGui::BulletText("Forward - W");
		ImGui::BulletText("Backward - S");
		ImGui::BulletText("Left - A");
		ImGui::BulletText("Right - D");
		ImGui::BulletText("Up - E");
		ImGui::BulletText("Down - Q");
		ImGui::Text("Zoom:");
		ImGui::BulletText("Mouse wheel scroll");
		ImGui::Text("Rotate:");
		ImGui::BulletText("Mouse move around a point");
		ImGui::BulletText("Hold right mouse button");
		ImGui::End();
	}
	{
		ImGui::Begin("Terrain");
		{
			ImGui::SliderFloat("Min height", &(m_terrain.minHeight), -50.f, 0.f);
			ImGui::SliderFloat("Max height", &(m_terrain.maxHeight), 0.f, 100.f);
			ImGui::SliderFloat("Sand height", &(m_terrain.sandHeight), 0.f, 10.f);
			ImGui::SliderFloat("Snow height", &(m_terrain.snowHeight), 0.f, 100.f);
		}
		{
			ImGui::SliderFloat("Height Offset X", &m_offsetX_h, 0.f, 100.f);
			ImGui::SliderFloat("Height Offset Y", &m_offsetY_h, 0.f, 100.f);
			ImGui::SliderFloat("Height Increment", &m_increment_h, 0.0001f, 0.1f);
			if (ImGui::Button("Height Regenerate")) m_terrain.ChangeHeightMap(m_offsetX_h, m_offsetY_h, m_increment_h);
		}
		{
			ImGui::SliderFloat("Color Offset X", &m_offsetX_p, 0.f, 100.f);
			ImGui::SliderFloat("Color Offset Y", &m_offsetY_p, 0.f, 100.f);
			ImGui::SliderFloat("Color Increment", &m_increment_p, 0.0001f, 0.1f);
			if (ImGui::Button("Color Regenerate")) m_terrain.ChangePatchMap(m_offsetX_p, m_offsetY_p, m_increment_p);
		}
		ImGui::End();
	}
	{
		ImGui::Begin("FPS");
		ImGui::Text("%.3f", ImGui::GetIO().Framerate);
		ImGui::End();
	}
	{
		ImGui::Begin("Building Type");
		ImGui::RadioButton("Studio Flat", (int*) & m_buildingType, (int)BuildingType::StudioFlat);
		ImGui::RadioButton("House", (int*) & m_buildingType, (int)BuildingType::House);
		ImGui::RadioButton("Family House", (int*) & m_buildingType, (int)BuildingType::FamiliyHouse);
		ImGui::RadioButton("Tower", (int*) & m_buildingType, (int)BuildingType::Tower);
		ImGui::RadioButton("Block House", (int*) & m_buildingType, (int)BuildingType::BlockHouse);
		ImGui::End();
	}
	{
		ImGui::Begin("FrameBuffer");
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)m_fbo.GetColorBuffer(), wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	{
		ImGui::Begin("HeightMap");
		ImGui::Image((ImTextureID)m_terrain.GetHeightTexture(), ImVec2{(float)m_terrain.n, (float)m_terrain.m}, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);

	auto data = m_fbo.GetValueFromLocation(mouse.x, mouse.y);
	m_cursor[0] = data.x * m_terrain.n;
	m_cursor[1] = data.y * m_terrain.m;
	Rect area = Building::GetBoundingArea(m_cursor[0], m_cursor[1], m_buildingType);
	m_cursorColor = m_buildings.CanBuildingBeAdded(area) ? glm::vec4{ 1.f,1.f,1.f,1.f } : glm::vec4{ 1.f,0.f,0.f,1.f };
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
		PlaceBuilding(mouse.x, mouse.y);
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
	m_camera.MouseWheel(wheel);
}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );
	m_camera.Resize(_w, _h);
	m_fbo.CreateFrameBuffer(_w, _h);
}

void CMyApp::PlaceBuilding(int x, int y)
{
	auto data = m_fbo.GetValueFromLocation(x, y);

	float xPos = data.x * m_terrain.n;
	float zPos = data.y * m_terrain.m;

	Rect area = Building::GetBoundingArea(xPos, zPos, m_buildingType);
	Rect found = Building::GetFoundationArea(xPos, zPos, m_buildingType);

	if (m_buildings.CanBuildingBeAdded(area)) {
		m_terrain.AddFoundation(found);
		m_buildings.AddBuilding(xPos, zPos, m_buildingType);
	}
}
