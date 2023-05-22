#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "includes/ProgramObject.h"

class FrameBuffer
{
public:
	FrameBuffer();

	void CreateFrameBuffer(int width, int height);
	
	GLuint GetDepthBuffer() const { return m_depthBuffer; }
	GLuint GetColorBuffer() const { return m_colorBuffer; }
	GLuint GetFrameBuffer() const { return m_frameBuffer; }

	glm::vec3 GetValueFromLocation(int x, int y);

	ProgramObject m_terrainProgram;

protected:
	bool m_frameBufferCreated{ false };
	GLuint m_depthBuffer;				
	GLuint m_colorBuffer;				
	GLuint m_frameBuffer;				
	glm::vec3 m_pixelData;
	int m_w, m_h;

	void InitShaders();
};

