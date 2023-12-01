#ifndef ENGINEMESH
#define ENGINEMESH
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "Vector3.h"
using namespace std;

//顶点信息
struct Vertex {
	Vector3 Position;

	Vector3 Normal;

	glm::vec2 TexCoords;
};
//贴图类型
enum TextureType {
	DiffuseMap,
	SpecularMap,
	NormalMap,
	HeightMap
};

struct Texture {
	//用于纹理绑定
	uint32_t id;
	TextureType type;
	string path;
};

class EngineMesh {

private:

	///链接索引
	vector<uint32_t> indices;
	///纹理
	vector<Texture> textures;

public:
	///顶点
	vector<Vertex> vertices;
	explicit EngineMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) :vertices(vertices), indices(indices),textures(textures)
	{
		
	}
	/// <summary>
	/// 网格绘制
	/// 需要Shader类
	/// </summary>
	/// <param name="shader"></param>
	void DrawMesh();
};
#endif

