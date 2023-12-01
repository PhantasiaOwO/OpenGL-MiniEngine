#ifndef MODELLOADER
#define MODELLOADER
#include<map>
#include<algorithm>
#include<unordered_map>
#include<string>
#include<fstream>
#include<sstream>
#include "EngineModel.h"
using namespace std;
class ModelLoader{
private:
	unordered_map<string, EngineModel> models;
	vector<string> modelNames;
	vector<string> modelPaths;
	vector<glm::vec3> pos;
	vector<glm::vec3> rotation;
	vector<glm::vec3> scale;

	inline void stringSplit(vector<glm::vec3>& vec3,string str)
	{
		replace(str.begin(), str.end(), '(', ' ');
		replace(str.begin(), str.end(), ')', ' ');
		int num = 0;
		char seps[] = " ";
		char* token = nullptr;
		char* ptr = nullptr;
		vector<float> vecf;
		token = strtok_s(const_cast<char*>(str.c_str()), seps, &ptr);
		while (token != nullptr) {
			vecf.emplace_back(atof(token));
			++num;
			if (num == 3)
			{
				vec3.emplace_back(glm::vec3 (vecf[0], vecf[1], vecf[2]));
				vecf.clear();
				num = 0;
			}
			token = strtok_s(nullptr, seps, &ptr);
		}
	}

public:
	explicit ModelLoader()
	{
	}
	/// <summary>
	/// 加载模型路径配置文件
	/// </summary>
	inline void LoadModelFormFile()
	{
		ifstream file;
		stringstream ss;
		file.open("../Setting.csv", ios::in);
		if (!file.is_open())
		{
			std::cerr << "Fail to open Setting.csv";
		}

		vector<string> rows;
		string row;
		while (getline(file, row))
		{
			rows.emplace_back(row);
		}

		for (auto it = rows.begin()+1; it != rows.end(); ++it)
		{
			string str;
			istringstream istr(*it);

			getline(istr, str, ',');
			modelNames.emplace_back(str);

			getline(istr, str, ',');
			modelPaths.emplace_back(str);

			getline(istr, str, ',');
			stringSplit(pos,str);
			
			getline(istr, str, ',');
			stringSplit(rotation, str);

			getline(istr, str, ',');
			stringSplit(scale, str);
		}

		int index = 0;
		for (vector<string>::iterator it = modelPaths.begin(); it != modelPaths.end(); ++it, ++index)
		{
			models.emplace(make_pair(modelNames[index], EngineModel(modelNames[index], *it, pos[index], rotation[index], scale[index])));
		}
	}

	inline void BuiltScene()
	{
		for (auto& model : models)
		{
			model.second.BuiltModel();
		}
	}
};
#endif // !MODELLOADER
