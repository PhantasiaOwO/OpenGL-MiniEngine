#ifndef ENGINEMODEL_H
#define ENGINEMODEL_H

#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	EngineModel() = default;
	
	explicit EngineModel(const string& path,glm::vec3 pos,glm::vec3 rotation, glm::vec3 scale):pos(pos),rotation(rotation), scale(scale)
	{
		loadModel(path);
	}

	inline void BuiltModel(Shader shader)
	{
		for (auto& mesh : meshes)
		{
			mesh.DrawMesh(shader);
		}
	}
	
	inline void DestroyModel()
	{
		textures_loaded.clear();
		meshes.clear();
		directory.clear();
	}

	inline void SwitchModel(const string& path, glm::vec3 pos,glm::vec3 rotation ,glm::vec3 scale)
	{
		this->pos = pos;
		this->rotation = rotation;
		this->scale = scale;
		DestroyModel();
		loadModel(path);
	}

	inline glm::vec3 ModelPos()
	{
		return pos;
	}

	inline glm::vec3 ModelScale()
	{
		return scale;
	}

	inline GLfloat ModelRotationX()
	{
		return rotation.x;
	}

	inline GLfloat ModelRotationY()
	{
		return rotation.y;
	}

	inline GLfloat ModelRotationZ()
	{
		return rotation.z;
	}

private:
	//�Ѽ�������
	vector<Texture> textures_loaded;
	//��ȡ����
	vector<EngineMesh> meshes;
	//ģ�͵�ַ
	string directory;
	//ģ��λ��
	glm::vec3 pos;
	//���ĸ�����ת�Լ���ת����
	glm::vec3 rotation;
	//ģ�ʹ�С
	glm::vec3 scale;
	
	void loadModel(const string& path);

	void traverseNode(aiNode* node, const aiScene* scene);

	void storeMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);

	uint32_t loadTexture(const char* path);
};

#endif

