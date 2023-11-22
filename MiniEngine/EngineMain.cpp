#include "EngineMain.h"

#include <ctime>
#include <map>
#include <sstream>
#include <GL/glut.h>

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
static float posX(0), posY(0), posZ(-5);
static float scaleX(1), scaleY(1), scaleZ(1);

#pragma endregion

void BeginEngine(int* argc, char** argv) {
	// 初始化glut
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // 初始化显示模式 RGB、双缓冲
	glutInitWindowPosition(100, 100); // 初始化窗口位置
	glutInitWindowSize(windowWidth, windowHeight); // 初始化窗口大小
	glutCreateWindow(WindowTitle.c_str()); // 创建窗口
	glutDisplayFunc(&Internal_TickEngineAction);
	glutIdleFunc(&Internal_TickEngineAction);

	glEnable(GL_DEPTH_TEST);
}

void StartEngine() {
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

	// TODO matrix set
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (double)windowWidth / windowHeight, nearPlane, farPlane);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, 0, 0, 0, 0, 1, 0);
	glScalef(scaleX, scaleY, scaleZ);

	// TODO render


	glPopMatrix();
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
