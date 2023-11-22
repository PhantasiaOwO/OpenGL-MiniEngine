#ifndef ENGINEMODEL_H
#define ENGINEMODEL_H
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "EngineMesh.h"
#include "shader.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class EngineModel
{
public:
	explicit EngineModel(const string& path)
	{
		loadModel(path);
	}

	void Built(Shader shader)
	{
		for (auto& mesh : meshes)
		{
			mesh.DrawMesh(shader);
		}
	}
	void Destroy(EngineModel model);
private:
	//已加载纹理
	vector<Texture> textures_loaded;
	//读取网格
	vector<EngineMesh> meshes;
	//模型地址
	string directory;
	
	void loadModel(const string& path);
	void traverseNode(aiNode* node, const aiScene* scene);
	EngineMesh storeMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
	uint32_t loadTexture(const char* path);
};

#endif

