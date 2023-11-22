#ifndef ENGINEMESH
#define ENGINEMESH
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include "Shader.h"
#include <vector>
using namespace std;

//������Ϣ
struct Vertex {
	glm::vec3 Position;

	glm::vec3 Normal;
	//����
	glm::vec2 TexCoords;
	//����
	glm::vec3 Tangent;
};

struct Texture {
	//���������
	uint32_t id;
	TextureType type;
	string path;
};

enum TextureType{
	DiffuseMap,
	SpecularMap,
	NormalMap,
	HeightMap
};

class EngineMesh {

private:
	///����
	vector<Vertex> vertices;
	///��������
	vector<unsigned int> indices;
	///����
	vector<Texture> textures;
	///�����
	GLuint VAO, VBO, EBO;
	void setupMesh();
public:
	explicit EngineMesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) :vertices(vertices), indices(indices), textures(textures)
	{
		setupMesh();
	}
	/// <summary>
	/// �������
	/// ��ҪShader��
	/// </summary>
	/// <param name="shader"></param>
	void DrawMesh(const Shader& shader);
};
#endif

