#include "mesh.h"

Mesh::Mesh(bool includeNormals) {
	m_hasNormals = includeNormals;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	if (m_hasNormals) {
		glGenBuffers(1, &NBO);
	}
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	if (m_hasNormals) {
		glDeleteBuffers(1, &NBO);
	}
}

void Mesh::addQuad(glm::vec3 topLeftPos, glm::vec3 topRightPos, glm::vec3 bottomLeftPos, glm::vec3 bottomRightPos) {
	int vertexBase = m_vertices.size();
	
	m_vertices.push_back(topLeftPos);
	m_vertices.push_back(topRightPos);
	m_vertices.push_back(bottomLeftPos);
	m_vertices.push_back(bottomRightPos);

	if (m_hasNormals) {
		glm::vec3 normal = glm::normalize(glm::cross(topRightPos - topLeftPos, bottomLeftPos - topRightPos));
		m_normals.push_back(normal);
		m_normals.push_back(normal);
		m_normals.push_back(normal);
		m_normals.push_back(normal);
	}

	m_faces.push_back(glm::uvec3(vertexBase + 0, vertexBase + 2, vertexBase + 1));
	m_faces.push_back(glm::uvec3(vertexBase + 1, vertexBase + 2, vertexBase + 3));
}

void Mesh::updateGlBuffers() {
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, // This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
			3, // How many components there are per attribute value
			GL_FLOAT, // What type these components are
			GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't.
			3 * sizeof(GLfloat), //sizeof(glm::vec3), // Offset between consecutive indices
			(GLvoid*)0); // Offset of the first component. In our case it's 0 since we don't pad the input data array with anything.

	if (m_hasNormals) {
		glBindBuffer(GL_ARRAY_BUFFER, NBO);
		glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, // This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
			3, // How many components there are per attribute value
			GL_FLOAT, // What type these components are
			GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't.
			3 * sizeof(GLfloat), //sizeof(glm::vec3), // Offset between consecutive indices
			(GLvoid*)0); // Offset of the first component. In our case it's 0 since we don't pad the input data array with anything.
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_faces.size() * sizeof(glm::uvec3), &m_faces[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw() {
	glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, m_faces.size() * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}