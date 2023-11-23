#include "LightManager.h"

LightManager::LightManager()
{
	glGetIntegerv(GL_MAX_LIGHTS, &_maxLights);

	_lights.emplace_back(0);
}

LightManager* LightManager::GetInstance()
{
	if (Instance == nullptr) Instance = new LightManager();
	return Instance;
}

void LightManager::Init()
{
	// 设置光照和材质 /////////////////////////////////////
	glColorMaterial(_faceRenderMode, _colorRenderMode);	// 设置颜色材质参数
	glEnable(GL_COLOR_MATERIAL);											// 使用颜色材质
	glColor3f(1.0f, 1.0f, 1.0f);								// 设置默认的物体颜色
	glEnable(GL_LIGHTING);
	//GL_LIGHT0// 使用光照
	//GL_MAX_LIGHTS
}

LightUnit& LightManager::AddLight()
{
	_lights.emplace_back(_lights.size());
	return _lights.back();
}

LightUnit& LightManager::AddLightAtPosition(GLfloat position[])
{
	AddLight().
}


