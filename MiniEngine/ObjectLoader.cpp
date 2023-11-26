#include "ObjectLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

bool ObjectLoader::LoadObject(const std::string& path) {
	ifstream file;
	file.open(path, ios::in);
	if (!file.is_open()) {
		stringstream ss;
		ss << "Could not find object file. Path: \"" << path << "\"";
		std::cout << ss.str() << endl;
		return _bLoaded = false;
	}
	while (!file.eof()) {
		string line;
		getline(file, line);
		if ("vt" == line.substr(0, 2)) {
			int x, y;
			istringstream iss(line.substr(3));
			iss >> x >> y;
			_vertexTexture.emplace_back(x, y, 0);
		} else if ("vn" == line.substr(0, 2)) {
			float x, y, z;
			istringstream iss(line.substr(3));
			iss >> x >> y >> z;
			_vertexNormal.emplace_back(x, y, z);
		} else if ("v" == line.substr(0, 1)) {
			float x, y, z;
			istringstream iss(line.substr(2));
			iss >> x >> y >> z;
			_vertices.emplace_back(x, y, z);
		} else if ("f" == line.substr(0, 1)) {
			int x, y, z;
			istringstream iss(line.substr(2));
			iss >> x >> y >> z;
			_faces.emplace_back(x - 1, y - 1, z - 1);
		}
	}
	file.close();
	return _bLoaded = true;
}
