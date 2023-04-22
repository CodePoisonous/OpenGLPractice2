#include "Shader.h"

#include "Mesh.h"

using namespace std;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
	: m_vertices(vertices), m_indices(indices), m_textures(textures)
{	
	setupMesh();
}

Mesh::~Mesh()
{

}

void Mesh::Draw(const Shader& shader)
{
	// ƥ��������Ϣ
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int normalNr = 0;
	unsigned int heightNr = 0;
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		string number;						// texture ����
		string name = m_textures[i].m_type;		// texture ����

		if ("texture_diffuse" == name)
			number = to_string(++diffuseNr);
		else if ("texture_specular" == name)
			number = to_string(++diffuseNr);
		else if ("texture_normal" == name)
			number = to_string(++diffuseNr);
		else if ("texture_height" == name)
			number = to_string(++diffuseNr);

		shader.setInt(("material." + name + number).c_str(), i);
		
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}

	// ����mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, (unsigned int)m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// �������û�0λ��
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	// ���ɶ���
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
		
	glBindVertexArray(m_VAO);						// ��VAO	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);			// ��VBO
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);	// ��EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
		
	glEnableVertexAttribArray(0);	// ����λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);	// ���㷨��
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Normal));

	glEnableVertexAttribArray(2);	// ������������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_TexCoords));

	glEnableVertexAttribArray(3);	// ���� Tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Tangent));

	glEnableVertexAttribArray(4);	// ���� Bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Bitangent));

	glEnableVertexAttribArray(5);	// ���� BoneIDs
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

	glEnableVertexAttribArray(6);	// ���� Weights
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));


	glBindVertexArray(0);
}