#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "stb_image.h"

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
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	// 所有网格三角形化|翻转y轴
	
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

	// 将当前节点中所有网格添加进m_meshs
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshs.push_back(processMesh(mesh, scene));
	}

	// 处理子节点
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

	// 顶点数据
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vx;
		// 顶点位置
		vx.m_Position.x = mesh->mVertices[i].x;
		vx.m_Position.y = mesh->mVertices[i].y;
		vx.m_Position.z = mesh->mVertices[i].z;

		// 法线
		vx.m_Normal.x = mesh->mNormals[i].x;
		vx.m_Normal.y = mesh->mNormals[i].y;
		vx.m_Normal.z = mesh->mNormals[i].z;

		// 纹理坐标
		if (mesh->mTextureCoords[0])
		{
			vx.m_TexCoords.x = mesh->mTextureCoords[0][i].x;
			vx.m_TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else 
			vx.m_TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vx);
	}

	// 网格索引
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	// 材质
	if (mesh->mMaterialIndex >= 0)
	{
		const aiMaterial* material =  scene->mMaterials[mesh->mMaterialIndex];
		
		// 漫反射贴图、镜面光贴图
		vector<Texture> diffuseMaps;
		loadMaterialTextures(diffuseMaps, material, aiTextureType_DIFFUSE, "texture_diffuse");
		vector<Texture> specularMaps;
		loadMaterialTextures(specularMaps, material, aiTextureType_SPECULAR, "texture_specular");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

void Model::loadMaterialTextures(vector<Texture>& Maps, const aiMaterial* mat, 
	const aiTextureType& type, const string& typeName)
{
	Maps.clear();
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		const auto& it = find_if(m_textures_loaded.cbegin(), m_textures_loaded.cend(), [&](const Texture& tx)
		{
			return string(str.C_Str()) == tx.m_path;
		});

		if (it == m_textures_loaded.cend())
		{
			Texture texture;
			texture.m_id = TextureFromFile(str.C_Str(), m_directory, false);
			texture.m_type = typeName;
			texture.m_path = str.C_Str();
			m_textures_loaded.push_back(texture);
			Maps.push_back(texture);
		}
		else 
			Maps.push_back(*it);
	}
}

unsigned int Model::TextureFromFile(const string& subPath, const string& directory, bool gamma)
{
	string path = directory + '/' + subPath;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;
		else;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
