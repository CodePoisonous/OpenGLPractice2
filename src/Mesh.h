#pragma once

#include <vector>
#include <glm/glm.hpp>

class Shader;

// ��������
struct Vertex
{
	glm::vec3 m_Position;	// λ������
	glm::vec3 m_Normal;		// ������
	glm::vec2 m_TexCoords;	// ������������

	glm::vec3 m_Tangent;
	glm::vec3 m_Bitangent;

	int m_BoneIDs[4];		// ��������
	int m_Weights[4];		// ����Ȩ��
};

// ��������
struct Texture
{
	unsigned int m_id;
	std::string m_type;		// ����
	std::string m_path;		// ·��
};

// ��������
class Mesh 
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	~Mesh();

public:
	void Draw(const Shader& shader) const;

private:
	void setupMesh();

public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;
};