#ifndef ENGINEMESH
#define ENGINEMESH
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include <vector>
#include"ModelShader.h"
using namespace std;

//顶点信息
struct Vertex {
	glm::vec3 Position;

	glm::vec3 Normal;
	//纹理
	glm::vec2 TexCoords;
	//切线
	glm::vec3 Tangent;
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
	///顶点
	vector<Vertex> vertices;
	///链接索引
	vector<uint32_t> indices;
	///纹理
	vector<Texture> textures;
	///顶点绑定
	GLuint VAO, VBO, EBO;
	void setupMesh();
public:
	explicit EngineMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) :vertices(vertices), indices(indices), textures(textures)
	{
		setupMesh();
	}
	/// <summary>
	/// 网格绘制
	/// 需要Shader类
	/// </summary>
	/// <param name="shader"></param>
	void DrawMesh(const Shader& shader);
};
#endif

