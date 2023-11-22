#pragma once
#include "LightEnums.h"
#include <gl/glut.h>


class LightManager
{
private:
	static LightManager* Instance;

	LightManager();
	LightManager(const LightManager&);
	LightManager& operator=(const LightManager&);


public:
	LightManager* GetInstance()
	{
		if (Instance == nullptr) Instance = new LightManager();
		return Instance;
	}

	void Init();

public:
	EFaceRenderMode FaceRenderMode;
	EColorRenderMode ColorRenderMode;


};

