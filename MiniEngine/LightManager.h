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
	// EFaceRenderMode _faceRenderMode;
	// EColorRenderMode _colorRenderMode;


	LightManager();
	// set private to disable
	LightManager(const LightManager&);
	LightManager& operator=(const LightManager&);

public:
	static LightManager* GetInstance();
	void Init();
	LightUnit& AddLight();
	LightUnit& AddLightAtPosition(glm::vec4 position);

	bool GetLightingEnable() const;
	void SetLightingEnable(bool isEnabled);

	void EnableLight(int index);
	void DisableLight(int index);

	size_t LightCount() const {return _lights.size();}
	
	int GetMaxLightsNumber() const { return _maxLights; }
	
	ELightType GetLightType(int index) const;

	LightUnit& operator[](const int lightId) { return _lights[lightId]; }
public:
	


};

