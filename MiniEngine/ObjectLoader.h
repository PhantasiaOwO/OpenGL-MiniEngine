#pragma once
#include <string>
#include <vector>

#include "Vector3.h"

class ObjectLoader {
	bool _bLoaded;
	std::vector<Vector3> _vertices;
	std::vector<Vector3> _faces;
	std::vector<Vector3> _vertexTexture;
	std::vector<Vector3> _vertexNormal;

public:
	inline explicit ObjectLoader(const std::string& path) : _bLoaded(false) {
		_bLoaded = LoadObject(path);
	}

	bool LoadObject(const std::string& path);

	inline bool IsLoaded() const { return _bLoaded; }
	inline const std::vector<Vector3>& Vertices() const { return _vertices; }
	inline const std::vector<Vector3>& Faces() const { return _faces; }
	inline const std::vector<Vector3>& VertexTexture() const { return _vertexTexture; }
	inline const std::vector<Vector3>& VertexNormal() const { return _vertexNormal; }
};

