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

void BeginEngine(int* argc, char** argv) {
	// 初始化glut
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // 初始化显示模式 RGB、双缓冲
	glutInitWindowPosition(100, 100); // 初始化窗口位置
	glutInitWindowSize(800, 600); // 初始化窗口大小
	glutCreateWindow(WindowTitle.c_str()); // 创建窗口
	glutDisplayFunc(&Internal_TickEngineAction);
	glutIdleFunc(&Internal_TickEngineAction);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, 800.f / 600, 0.01f, 1000.f);
}

void StartEngine() {
	glutMainLoop();
}


void EndEngine() {}


void Internal_TickEngineAction() {
	Internal_UpdateTimeAndFrameCount();

	// render
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO matrix set


	// TODO render tick

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
