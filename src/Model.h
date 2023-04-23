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
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, const aiTextureType& type, const std::string& typeName);

private:
	std::vector<Mesh> m_meshs;
	std::string m_directory;
};