#include "LightManager.h"

void LightManager::Init()
{
	// 设置光照和材质 /////////////////////////////////////
	glColorMaterial(FaceRenderMode, GL_AMBIENT_AND_DIFFUSE);	// 设置颜色材质参数
	glEnable(GL_COLOR_MATERIAL);											// 使用颜色材质
	glColor3f(1.0f, 1.0f, 1.0f);								// 设置默认的物体颜色
	glEnable(GL_LIGHTING);												// 使用光照
}
