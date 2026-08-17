#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_s.h"
#include <utility>
#include <string>
#include <vector>
#include <array>

enum skeleton
{
	maxBoneInfluence = 4
};


struct Vertex {
	glm::vec3 Position {};
	glm::vec3 Normal {};

	glm::vec2 TexCoords {};

	glm::vec3 Tangent {};
	glm::vec3 Bitangent {};

	std::array<int, maxBoneInfluence> m_BoneIDs{ -1, -1, -1, -1 };
	std::array<float, maxBoneInfluence> m_Weights{};
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
		:m_vertices{ std::move(vertices) }, m_indices{ std::move(indices) }, m_textures{ std::move(textures) }
	{
		setupMesh();
	}

	void Draw(Shader& shader) const ;
	unsigned int const getVAO()
	{
		return VAO;
	}
private:

	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

#endif