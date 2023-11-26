#include "Mesh.h"

#include <GL/glut.h>
#include "Transform.h"

void Mesh::AssignMeshDataFromLoader(const ObjectLoader& objLoader, bool bCentralized) {
	_vertices = objLoader.Vertices();
	_faces = objLoader.Faces();
	_vertexTextures = objLoader.VertexTexture();
	_vertexNormals = objLoader.VertexNormal();

	RecalculateVertexNormals();
	if (bCentralized) Centralize();
}

void Mesh::Centralize() {
	if (_vertices.empty()) return;
	Vector3 min, max;
	min = max = _vertices[0];
	for (const auto& vert : _vertices) {
		min.X() = std::min(vert.X(), min.X());
		min.Y() = std::min(vert.Y(), min.Y());
		min.Z() = std::min(vert.Z(), min.Z());
		max.X() = std::max(vert.X(), max.X());
		max.Y() = std::max(vert.Y(), max.Y());
		max.Z() = std::max(vert.Z(), max.Z());
	}
	const Vector3 center(min + (max - min) / 2);
	for (auto& vert : _vertices) {
		vert -= center;
	}
}

void Mesh::RecalculateFaceNormals() {
	_faceNormals.clear();
	_faceNormals.resize(_faces.size());
	for (size_t i = 0; i < _faces.size(); ++i) {
		const auto& face = _faces[i];
		const Vector3 edge01 = _vertices[face[1]] - _vertices[face[0]];
		const Vector3 edge02 = _vertices[face[2]] - _vertices[face[0]];
		_faceNormals[i] = Cross(edge01, edge02).Normalized();
	}
}

void Mesh::RecalculateVertexNormals() {
	RecalculateFaceNormals();

	_vertexNormals.clear();
	_vertexNormals.resize(_vertices.size());
	for (size_t i = 0; i < _faces.size(); ++i) {
		const auto& face = _faces[i];
		_vertexNormals[face[0]] += _faceNormals[i];
		_vertexNormals[face[1]] += _faceNormals[i];
		_vertexNormals[face[2]] += _faceNormals[i];
	}
	for (auto& vertexNormal : _vertexNormals) {
		vertexNormal = vertexNormal.Normalized();
	}
}

void Mesh::Render() {
	glColor3f(1, 1, 1);
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (size_t i = 0; i < _faces.size(); ++i) {
		const auto& face = _faces[i];
			Vector3 normal(_vertexNormals[face[0]]);
			Vector3 vertex(_vertices[face[0]]);
			glNormal3fv(normal.DataPtr());
			glVertex3fv(vertex.DataPtr());
			normal = Vector3(_vertexNormals[face[1]]);
			vertex = Vector3(_vertices[face[1]]);
			glNormal3fv(normal.DataPtr());
			glVertex3fv(vertex.DataPtr());
			normal = Vector3(_vertexNormals[face[2]]);
			vertex = Vector3(_vertices[face[2]]);
			glNormal3fv(normal.DataPtr());
			glVertex3fv(vertex.DataPtr());
	}

	glEnd();
	glPopMatrix();
}
