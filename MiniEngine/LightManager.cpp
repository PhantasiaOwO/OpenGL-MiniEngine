#include "LightManager.h"

void LightManager::Init()
{
	// ���ù��պͲ��� /////////////////////////////////////
	glColorMaterial(FaceRenderMode, GL_AMBIENT_AND_DIFFUSE);	// ������ɫ���ʲ���
	glEnable(GL_COLOR_MATERIAL);											// ʹ����ɫ����
	glColor3f(1.0f, 1.0f, 1.0f);								// ����Ĭ�ϵ�������ɫ
	glEnable(GL_LIGHTING);												// ʹ�ù���
}
