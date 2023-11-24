#ifndef ENGINEMODEL_H
#define ENGINEMODEL_H

#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
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

public:
	EngineModel() :pos(glm::vec3(0.0f,0.0f,0.0f)), rotation(glm::vec3(0.0f,0.0f,0.0f)), scale(glm::vec3(0.0f,0.0f,0.0f)){}
	
	explicit EngineModel(const string& path,glm::vec3 pos,glm::vec3 rotation, glm::vec3 scale):pos(pos),rotation(rotation), scale(scale)
	{
		loadModel(path);
	}

	inline void BuiltModel(Shader shader)
	{
		glm::mat4 my_transform = glm::mat4(1.0f);

		my_transform = glm::translate(my_transform, ModelPos());
		my_transform = glm::scale(my_transform, ModelScale());
		my_transform = glm::rotate(my_transform, glm::radians(ModelRotationX()), glm::vec3(1.0f, 0.0f, 0.0f));
		my_transform = glm::rotate(my_transform, glm::radians(ModelRotationY()), glm::vec3(0.0f, 1.0f, 0.0f));
		my_transform = glm::rotate(my_transform, glm::radians(ModelRotationZ()), glm::vec3(0.0f, 0.0f, 1.0f));

		shader.setMat4("transform", my_transform);
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

	inline const glm::vec3 ModelPos() 
	{
		return pos;
	}

	inline const glm::vec3 ModelScale() 
	{
		return scale;
	}
	
	inline const GLfloat ModelRotationX() 
	{
		return rotation.x;
	}

	inline const GLfloat ModelRotationY() 
	{
		return rotation.y;
	}

	inline const GLfloat ModelRotationZ() 
	{
		return rotation.z;
	}

};

#endif

