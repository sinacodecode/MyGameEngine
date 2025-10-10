#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_s.h"

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4


struct Vertex {
	glm::vec3 Position {};
	glm::vec3 Normal {};

	glm::vec2 TexCoords {};

	glm::vec3 Tangent {};
	glm::vec3 Bitangent {};

	int m_BoneIDs[MAX_BONE_INFLUENCE] {};
	float m_Weights[MAX_BONE_INFLUENCE] {};
};

struct Texture {
	unsigned int id {};

	std::string type {};
	std::string path {};
};

class Mesh {
public:
	std::vector<Vertex> m_vertices{};

	std::vector<unsigned int> m_indices{};

	std::vector<Texture> m_textures{};


	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		:m_vertices{vertices}, m_indices{indices}, m_textures{textures}
	{
		setupMesh();
	}

	void Draw(Shader& shader);
	unsigned int getVAO()
	{
		return VAO;
	}
private:

	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

#endif