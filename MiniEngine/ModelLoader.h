#ifndef MODELLOADER
#define MODELLOADER
#include<map>
#include<unordered_map>
#include<string>
#include "EngineModel.h"
using namespace std;
class ModelLoader{
private:
	unordered_map<string, EngineModel> models;

public:
	ModelLoader()
	{
		LoadModelFormFile();
	}
	/// <summary>
	/// 加载模型路径配置文件
	/// </summary>
	void LoadModelFormFile()
	{
		std::ifstream file;
		stringstream ss;
		file.open("../Setting.txt", std::ios::in);
		if (!file.is_open())
		{
			std::cerr << "Fail to open Setting.txt";
		}

		string line;
		while (getline(file, line))
		{
			ss.str(line);
			string single;
			int i = 0;
			while (getline(ss, single, ' ')) {
				
			}
		}

		//EngineModel("../Obj/Bottle/Bottle.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f));
		//models.emplace(make_pair("Bottle",EngineModel("../Obj/Bottle/Bottle.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f))));
		//cout << models["Bottle"].ModelPos().x;
		//models.emplace(make_pair("Nanosuit", EngineModel("../Obj/Nanosuit/Nanosuit.obj", glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f))));
	}

	void BuiltScene(Shader shader)
	{
		for (auto& model : models)
		{
			model.second.BuiltModel(shader);
		}
	}
};
#endif // !MODELLOADER
