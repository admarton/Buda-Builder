#include "Terrain.h"
#include <glm/glm.hpp>
#include <vector>

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
}

void Terrain::Draw()
{
	glBindVertexArray(vertexArrayObject);

	glDrawElements(GL_TRIANGLES, (n) * (m) * 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
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
			vertices[i * m + j].x = i;
			vertices[i * m + j].y = j;
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
}
