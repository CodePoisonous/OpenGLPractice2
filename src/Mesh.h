#pragma once

#include <vector>
#include <glm/glm.hpp>

class Shader;

// 顶点数据
struct Vertex
{
	glm::vec3 m_Position;	// 位置向量
	glm::vec3 m_Normal;		// 法向量
	glm::vec2 m_TexCoords;	// 纹理坐标向量

	glm::vec3 m_Tangent;
	glm::vec3 m_Bitangent;

	int m_BoneIDs[4];		// 骨骼索引
	int m_Weights[4];		// 骨骼权重
};

// 纹理数据
struct Texture
{
	unsigned int m_id;
	std::string m_type;		// 类型
	std::string m_path;		// 路径
};

// 网格数据
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