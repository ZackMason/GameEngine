#pragma once

#include "include/glm/glm.hpp"
#include "include/GLEW/GL/glew.h"
#include "obj_loader.h"

#include <iostream>

class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(0,0,0))
	{
		this->pos      = pos;
		this->texCoord = texCoord;
		this->normal   = normal;
	}

	inline glm::vec3 *GetPos()      { return      &pos; }
	inline glm::vec2 *GetTexCoord() { return &texCoord; }
	inline glm::vec3 *GetNormal()   { return   &normal; }

protected:
private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

class Mesh
{
public:
	Mesh();
	Mesh(Vertex* vertices, unsigned int numVertices);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(const std::string& fileName);

	void Draw();
	void V_Draw();

	virtual ~Mesh();

private:

	Mesh(const Mesh& other) : Mesh(other.m_filename) {}


	Mesh& operator=(const Mesh& other)
	{
		std::cout << "assign" << std::endl;

		if (this == &other)
			return *this;

		m_drawCount = other.m_drawCount;
		m_vertexArrayObject = other.m_vertexArrayObject;
		//m_vertexArrayBuffers[NUM_BUFFERS];

		glCopyBufferSubData(*other.m_vertexArrayBuffers, *m_vertexArrayBuffers, 0, 0, sizeof(other.m_vertexArrayBuffers) * NUM_BUFFERS);

		return *this;
	}

	void InitMesh(const IndexedModel& model);

	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		INDEX_VB   ,
		NORMAL_VB  ,
		NUM_BUFFERS
	};

	std::string	 m_filename;
	GLuint       m_vertexArrayObject;
	GLuint       m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
};

