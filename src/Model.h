#pragma once

#include <vector>
#include <string>
#include <assimp/scene.h>

struct Texture;
class Shader;
class Mesh;

class Model
{
public:
	Model(const std::string& path);
	~Model();

public:
	void Draw(const Shader& shader) const;

private:
	void loadModel(const std::string& path);
	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
	void loadMaterialTextures(std::vector<Texture>& Maps, const aiMaterial* mat, const aiTextureType& type, const std::string& typeName);
	unsigned int TextureFromFile(const std::string& subPath,  const std::string& directory, bool gamma);

private:
	std::vector<Mesh> m_meshs;		// ����
	std::string m_directory;		// �ļ�·��

private:
	std::vector<Texture> m_textures_loaded;	// ����ģ�͵������棨ȷ����ͬ�������ظ����أ�
};