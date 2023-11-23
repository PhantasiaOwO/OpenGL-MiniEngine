#pragma once
#include <unordered_map>
#include <vector>

#include "LightEnums.h"
#include "LightUnit.h"


class LightManager
{
private:
	static LightManager* Instance;

	int _maxLights;
	std::vector<LightUnit> _lights;

	bool _isLightingEnable = false;
	EFaceRenderMode _faceRenderMode;
	EColorRenderMode _colorRenderMode;


	LightManager();
	// set private to disable
	LightManager(const LightManager&);
	LightManager& operator=(const LightManager&);

public:
	static LightManager* GetInstance();
	void Init();
	LightUnit& AddLight();
	LightUnit& AddLightAtPosition(GLfloat position[]);

	bool GetLightingEnable();
	bool SetLightingEnable();

	void EnableLight(int index);
	void DisableLight(int index);

	int LightCount() const {return _lights.size();}
	

	int GetMaxLightsNumber() const { return _maxLights; }
	//void GetLightType(int index);

	LightUnit& operator[](const int lightId) { return _lights[lightId]; }
public:
	


};

