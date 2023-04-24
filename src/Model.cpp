#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

#include "Model.h"


using namespace std;

Model::Model(const string& path)
{
	loadModel(path);
}

Model::~Model()
{

}

void Model::Draw(const Shader& shader) const
{
	for (const auto& mesh : m_meshs) mesh.Draw(shader);
}

void Model::loadModel(const string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	// �������������λ�|��תy��
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}

	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode* node, const aiScene* scene)
{
	if (!node || !scene) return;

	// ����ǰ�ڵ�������������ӽ�m_meshs
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshs.push_back(processMesh(mesh, scene));
	}

	// �����ӽڵ�
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// ��������
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vx;
		// ����λ��
		vx.m_Position.x = mesh->mVertices[i].x;
		vx.m_Position.y = mesh->mVertices[i].y;
		vx.m_Position.z = mesh->mVertices[i].z;

		// ����
		vx.m_Normal.x = mesh->mNormals[i].x;
		vx.m_Normal.y = mesh->mNormals[i].y;
		vx.m_Normal.z = mesh->mNormals[i].z;

		// ��������
		if (mesh->mTextureCoords[0])
		{
			vx.m_TexCoords.x = mesh->mTextureCoords[0][i].x;
			vx.m_TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else vx.m_TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vx);
	}

	// ��������
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	// ����
	if (mesh->mMaterialIndex >= 0)
	{

	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, const aiTextureType& type, const string& typeName)
{
	return vector<Texture>();
}
