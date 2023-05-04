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
	unsigned int m_type;	// 类型:0diffuse,1specular,2normal,3height
	std::string m_path;		// 路径（用于与其他纹理进行比较）
};

// 网格数据
class Mesh 
{
public:
	Mesh(const std::vector<Vertex>& vertices, 
		 const std::vector<unsigned int>& indices, 
		 const std::vector<Texture>& textures);
	~Mesh();

public:
	void Draw(const Shader& shader) const;

private:
	void setupMesh();

public:
	std::vector<Vertex> m_vertices;			// 顶点
	std::vector<unsigned int> m_indices;	// 索引
	std::vector<Texture> m_textures;		// 纹理

private:
	unsigned int m_VAO;	// vertex array object
	unsigned int m_VBO;	// vertex buffer object
	unsigned int m_EBO;	// element buffer object
};