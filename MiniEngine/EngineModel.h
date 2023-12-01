#ifndef ENGINEMODEL_H
#define ENGINEMODEL_H

#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <vector>
#include "EngineMesh.h"
using namespace std;

class EngineModel
{
private:
	string name;
	//读取网格
	vector<EngineMesh> meshes;
	vector<Texture> textures_loaded;
	//模型位置
	glm::vec3 pos;
	//绕哪个轴旋转以及旋转度数
	glm::vec3 rotation;
	//模型大小
	glm::vec3 scale;

	string directory;

	void loadModel(const string& path);

	void traverseNode(aiNode* node, const aiScene* scene);

	void storeMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);

	uint32_t loadTexture(const char* path);

public:
	EngineModel() :pos(glm::vec3(0.0f,0.0f,0.0f)), rotation(glm::vec3(0.0f,0.0f,0.0f)), scale(glm::vec3(0.0f,0.0f,0.0f)){}
	
	explicit EngineModel(string name,const string& path,glm::vec3 pos,glm::vec3 rotation, glm::vec3 scale):name(name), pos(pos), rotation(rotation), scale(scale)
	{
		loadModel(path);
	}

	inline void BuiltModel()
	{
		glPushMatrix();
		glScalef(scale.x,scale.y,scale.z);
		glRotatef(rotation.x,1, 0, 0);
		glRotatef(rotation.y,0, 1, 0);
		glRotatef(rotation.z,0, 0, 1);
		glTranslatef(pos.x,pos.y,pos.z);
		for (auto& mesh : meshes)
		{
			mesh.DrawMesh();
		}
		glPopMatrix();
		glFlush();
	}
	
	inline void DestroyModel()
	{
		meshes.clear();
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
	
	inline const float ModelRotationX() 
	{
		return rotation.x;
	}

	inline const float ModelRotationY() 
	{
		return rotation.y;
	}

	inline const float ModelRotationZ() 
	{
		return rotation.z;
	}

};

#endif

