#include "Terrain.h"
#include <vector>
#include "Perlin.hpp"

Terrain::Terrain(GLuint n, GLuint m) : n{ n }, m{ m }
{
	InitSurface();
	InitShaders();
	InitTextures();
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);

	glDeleteTextures(1, &heightTexture);
}

void Terrain::Draw()
{
	program.SetUniform("n", (float)n);
	program.SetUniform("m", (float)m);
	program.SetUniform("minHeight", minHeight);
	program.SetUniform("maxHeight", maxHeight);
	program.SetTexture("heightMap", 0, heightTexture);
	program.SetTexture("patchMap", 1, patchTexture);
	program.SetTexture("grass1", 2, grass1);
	program.SetTexture("grass2", 3, grass2);
	program.SetTexture("grass3", 4, grass3);

	glBindVertexArray(vertexArrayObject);

	glDrawElements(GL_TRIANGLES, (n) * (m) * 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Terrain::FillHeightMap(float offsetX, float offsetY, float increment)
{
	Perlin p{ offsetX, offsetY, increment };
	heightMapData = p.GetPerlin2D(n, m);

	for (unsigned i = 0; i < n; i++) {
		for (unsigned j = 0; j < m; j++) {
			heightMapData[i * m + j] *= 1.f - 2*glm::distance(glm::vec2{ (float)i/(float)n,(float)j/(float)m}, glm::vec2{0.5f, 0.5f});
		}
	}
}

void Terrain::ChangeHeightMap(float offsetX, float offsetY, float increment)
{
	FillHeightMap(offsetX, offsetY, increment);

	glBindTexture(GL_TEXTURE_2D, heightTexture);
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0,
		0,
		n,
		m,
		GL_RED,
		GL_FLOAT,
		(void*)&heightMapData[0]);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::FillPatchMap(float offsetX, float offsetY, float increment)
{
	auto noise = Perlin{ offsetX, offsetY, increment }.GetPerlin2D(n,m);
	patchMapData.resize(n * m);
	for (size_t i = 0; i < n*m; i++)
	{
		if (noise[i] < 0.33f) patchMapData[i] = glm::vec4(1.f,0.f,0.f,0.f);
		else if (noise[i] < 0.44f) patchMapData[i] = glm::vec4(0.5f, 0.5f, 0.f, 0.f);
		else if (noise[i] < 0.55f) patchMapData[i] = glm::vec4(0.f, 1.f, 0.f, 0.f);
		else if (noise[i] < 0.66f) patchMapData[i] = glm::vec4(0.f, 0.5f, 0.5f, 0.f);
		else patchMapData[i] = glm::vec4(0.f, 0.f, 1.f, 0.f);
	}
}

void Terrain::ChangePatchMap(float offsetX, float offsetY, float increment)
{
	FillPatchMap(offsetX, offsetY, increment);

	glBindTexture(GL_TEXTURE_2D, patchTexture);
	glTexSubImage2D(
		GL_TEXTURE_2D,
		0,
		0,
		0,
		n,
		m,
		GL_RGBA,
		GL_FLOAT,
		(void*)&patchMapData[0]);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::InitSurface()
{
	const GLuint vertCount = n * m;
	const GLuint indCount = (n) * (m) * 6;
	std::vector<glm::vec2> vertices;
	std::vector<GLuint> indices;
	vertices.resize(vertCount);
	indices.resize(indCount);

	for (GLuint i = 0; i < n; i++)
		for (GLuint j = 0; j < m; j++) {
			vertices[i * m + j].x = (float)i/(float)n;
			vertices[i * m + j].y = (float)j/(float)m;
		}

	for (GLuint i = 0; i < n - 1; i++)
		for (GLuint j = 0; j < m - 1; j++) {
			int squareInd = (i * m + j) * 6;
			indices[squareInd + 0] = (i + 0) * m + (j + 0);
			indices[squareInd + 1] = (i + 0) * m + (j + 1);
			indices[squareInd + 2] = (i + 1) * m + (j + 0);

			indices[squareInd + 3] = (i + 1) * m + (j + 0);
			indices[squareInd + 4] = (i + 0) * m + (j + 1);
			indices[squareInd + 5] = (i + 1) * m + (j + 1);
		}

	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), (void*)&vertices[0], GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), (void*)&indices[0], GL_STREAM_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::InitShaders()
{
	program.AttachShaders({
		{ GL_VERTEX_SHADER, "terrain.vert" },
		{ GL_FRAGMENT_SHADER, "terrain.frag" }
	});

	program.BindAttribLocation(0, "vs_in_tex");

	program.LinkProgram();
}

void Terrain::InitTextures()
{
	std::srand(std::time(nullptr));
	float offsetX = (float)std::rand() / (float)std::rand();
	float offsetY = (float)std::rand() / (float)std::rand();
	float increment = (float)(std::rand()%1000)/10000.f;
	FillHeightMap(offsetX,offsetY,increment);

	glGenTextures(1, &heightTexture);
	glBindTexture(GL_TEXTURE_2D, heightTexture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,				
		GL_R16F,		
		n,	
		m,	
		0,				
		GL_RED,	
		GL_FLOAT,
		(void*)&heightMapData[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	offsetX = (float)std::rand() / (float)std::rand();
	offsetY = (float)std::rand() / (float)std::rand();
	increment = (float)(std::rand() % 1000) / 10000.f;
	FillPatchMap(offsetX, offsetY, increment);

	glGenTextures(1, &patchTexture);
	glBindTexture(GL_TEXTURE_2D, patchTexture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		n,
		m,
		0,
		GL_RGBA,
		GL_FLOAT,
		(void*)&patchMapData[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	grass1.FromFile("assets/Grass001_1K_Color.png");
	grass2.FromFile("assets/Grass003_1K_Color.png");
	grass3.FromFile("assets/Grass004_1K_Color.png");
}
