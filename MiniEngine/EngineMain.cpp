﻿#include "EngineMain.h"

#include <ctime>
#include <map>
#include <sstream>

#include <GL/glut.h>

#include "ModelLoader.h"
#include "LightManager.h"
#include "Mesh.h"
#include "Transform.h"
using namespace std;

#pragma region 帧数和时间

static const string WindowTitle = "Engine";
static time_t now;
static time_t lastTickTime;
static float timeElapsed = 0;

static int fpsCount = 0;
static int fpsCalInterval = 1;
static time_t lastFpsUpdateTime;

#pragma endregion

#pragma region 视窗设置

static int windowWidth = 800;
static int windowHeight = 600;

#pragma endregion

#pragma region 相机设置

static float fovy(60);
static float nearPlane(0.01);
static float farPlane(100);
static Transform cameraTransform;
static bool isChangeRotate;
static Vector3 rotateAngle;
static Vector3 initPos;
static Quaternion initRot;

static float linearStep = 0.05f;
static float angleStep = 2.f;
static float mouseSensitive = 0.05f;

#pragma endregion

#pragma region 鼠标

Vector3 mouseLast;

#pragma endregion

#pragma region 模型加载器

ModelLoader modelLoader;

#pragma endregion

#pragma region 着色器

#pragma endregion

Mesh mesh;

void BeginEngine(int* argc, char** argv) {
	// 初始化glut
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // 初始化显示模式 RGB、双缓冲
	glutInitWindowPosition(100, 100); // 初始化窗口位置
	glutInitWindowSize(windowWidth, windowHeight); // 初始化窗口大小
	glutCreateWindow(WindowTitle.c_str()); // 创建窗口
	glutDisplayFunc(&Internal_TickEngineAction);
	glutIdleFunc(&Internal_TickEngineAction);
	glutKeyboardFunc(&Internal_KeyboardFunc);
	glutSpecialFunc(&Internal_SpecialFunc);
	glutMotionFunc(&Internal_MouseMotion);
	glutPassiveMotionFunc(&Internal_MousePassiveMotion);
	glEnable(GL_DEPTH_TEST);
}

void StartEngine() {
	// Lighting Enable
	LightManager& lightMgr = *LightManager::GetInstance();
	lightMgr[0].SetAmbientColor({0.2f, 0.2f, 0.2f, 1.0f});
	lightMgr[0].SetDiffuseColor({0.7f, 0.7f, 0.7f, 1.0f});
	lightMgr[0].SetSpecularColor({1.0f, 1.0f, 1.0f, 1.0f});

	// Mesh Load
	modelLoader.LoadModelFormFile();
	// Camera
	cameraTransform.position = {0, 0, 5};

	glutMainLoop();
}


void EndEngine() {}

void Internal_Reshape(int w, int h) {
	if (h == 0) h = 1;
	windowWidth = w;
	windowHeight = h;
	glViewport(0, 0, windowWidth, windowHeight);
}

void Internal_TickEngineAction() {
	Internal_UpdateTimeAndFrameCount();

	// render
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// matrix set
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (double)windowWidth / windowHeight, nearPlane, farPlane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vector3 cameraPosition = cameraTransform.position;
	Quaternion cameraRot = cameraTransform.rotation;
	glMultMatrixf(Matrix4x4ForGL(cameraRot.RotateMatrix().Transpose())); // 正交矩阵转置和逆一致
	glTranslatef(-cameraPosition.X(), -cameraPosition.Y(), -cameraPosition.Z());

	// worldAxes
	{
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0.5, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0.5, 0);
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 0.5);
		glEnd();
	}
	//mesh.Render();
	glPopMatrix();

	modelLoader.BuiltScene();

	glutSwapBuffers();
}

void Internal_UpdateTimeAndFrameCount() {
	now = clock();
	timeElapsed = static_cast<float>(now - lastTickTime) / CLOCKS_PER_SEC;
	lastTickTime = now;

	fpsCount += 1;
	if (now - lastFpsUpdateTime > CLOCKS_PER_SEC * fpsCalInterval) {
		stringstream newTitle;
		newTitle << WindowTitle << " - " << to_string(fpsCount / fpsCalInterval) << "fps";
		glutSetWindowTitle(newTitle.str().c_str());
		lastFpsUpdateTime = now;
		fpsCount = 0;
	}
}

void Internal_KeyboardFunc(unsigned char key, int x, int y) {
	key = tolower(key);
	switch (key) {
		case 27: // ESC: 退出程序
			exit(0);
			break;

		case 'w':// 前进
			cameraTransform.position += cameraTransform.rotation.ZAxis() * -linearStep;
			break;

		case 's':// 后退
			cameraTransform.position += cameraTransform.rotation.ZAxis() * linearStep;
			break;

		case 'a':// 左移
			cameraTransform.position += cameraTransform.rotation.XAxis() * -linearStep;
			break;

		case 'd':// 右移
			cameraTransform.position += cameraTransform.rotation.XAxis() * linearStep;
			break;

		case 'r':// 上移
			cameraTransform.position += cameraTransform.rotation.YAxis() * linearStep;
			break;

		case 'f':// 下移
			cameraTransform.position += cameraTransform.rotation.YAxis() * -linearStep;
			break;

		case 'q':// 左旋
			isChangeRotate = true;
			rotateAngle.Y() = rotateAngle.Y() + angleStep;
			break;

		case 'e':// 右旋
			isChangeRotate = true;
			rotateAngle.Y() = rotateAngle.Y() - angleStep;
			break;

		case 't':// 上旋
			isChangeRotate = true;
			rotateAngle.X() = rotateAngle.X() + angleStep;
			break;

		case 'g':// 下旋
			isChangeRotate = true;
			rotateAngle.X() = rotateAngle.X() - angleStep;
			break;

		case 'm':// 复位
			cameraTransform.position = initPos;
			cameraTransform.rotation = initRot;
			rotateAngle = {0, 0, 0};
			break;

		default:
			break;
	}

	if (isChangeRotate) {
		isChangeRotate = false;

		Quaternion newRotate;
		newRotate.SetFromEulerAngleZYX(rotateAngle);
		cameraTransform.rotation = newRotate;
	}
}

void Internal_SpecialFunc(int specialKey, int x, int y) { }


void Internal_MousePassiveMotion(int x, int y) {
	mouseLast = Vector3(x, y, 0);
}

void Internal_MouseMotion(int x, int y) {
	Vector3 delta(Vector3(x, y, 0) - mouseLast);
	mouseLast = Vector3(x, y, 0);
	delta *= mouseSensitive;
	isChangeRotate = true;
	rotateAngle.X() = rotateAngle.X() - delta.Y();
	rotateAngle.Y() = rotateAngle.Y() - delta.X();

	if (isChangeRotate) {
		isChangeRotate = false;
		rotateAngle.X() = std::max(-90.f, std::min(90.f, rotateAngle.X()));
		if (rotateAngle.Y() > 180.f) {
			rotateAngle.Y() = -180.f + fmod(rotateAngle.Y() - 180.f, 360.f);
		} else if (rotateAngle.Y() <= -180.f) {
			rotateAngle.Y() = 180.f - fmod(fabs(rotateAngle.Y()) - 180.f, 360.f);
		}
		Quaternion newRotate;
		newRotate.SetFromEulerAngleZYX(rotateAngle);
		cameraTransform.rotation = newRotate;
	}
}
