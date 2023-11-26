#pragma once
#include <vector>

#include "ObjectLoader.h"
#include "Vector3.h"

class Mesh {
	std::vector<Vector3> _vertices;
	std::vector<Vector3> _faces;
	std::vector<Vector3> _vertexTextures;
	std::vector<Vector3> _vertexNormals;
	std::vector<Vector3> _faceNormals;

public:
	inline std::vector<Vector3>& Vertices() { return _vertices; }
	inline std::vector<Vector3>& Faces() { return _faces; }
	inline std::vector<Vector3>& VertexTextures() { return _vertexTextures; }
	inline std::vector<Vector3>& VertexNormals() { return _vertexNormals; }
	inline std::vector<Vector3>& FaceNormals() { return _faceNormals; }

	void AssignMeshDataFromLoader(const ObjectLoader& objLoader, bool bCentralized = true);
	void Centralize();
	
	void RecalculateFaceNormals();
	void RecalculateVertexNormals(); // It will call RecalculateFaceNormal func at first

	void Render();
};

