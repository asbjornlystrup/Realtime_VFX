#pragma once

#include "config.h"

class Mesh {
public:
	Mesh(bool includeNormals);
	~Mesh();
	void draw();
	void updateGlBuffers();
	void addQuad(glm::vec3 topLeftPos, glm::vec3 topRightPos, glm::vec3 bottomLeftPos, glm::vec3 bottomRightPos);

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::uvec3> m_faces;
	std::vector<glm::vec3> m_normals;
	bool m_hasNormals;
	GLuint VAO, VBO, EBO, NBO;
};