#pragma once
#include <gl/glut.h>
enum EFaceRenderMode
{
	Front = GL_FRONT,
	Back = GL_BACK,
	FrontAndBack = GL_FRONT_AND_BACK
};

class LightManager
{
private:
	LightManager();
public:
	void Init();

public:
	FaceRenderMode;


};

