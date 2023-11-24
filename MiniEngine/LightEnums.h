#pragma once
#include <gl/glut.h>
enum EFaceRenderMode
{
	Front = GL_FRONT,
	Back = GL_BACK,
	FrontAndBack = GL_FRONT_AND_BACK
};

enum EColorRenderMode
{
	Ambient = GL_AMBIENT,
	Diffuse = GL_DIFFUSE,
	AmbientAndDiffuse = GL_AMBIENT_AND_DIFFUSE
};

