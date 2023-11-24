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
static float targetX(0), targetY(0), targetZ(0);
static float scale(1);
static float rotateAroundX(0);
static float rotateAroundY(0);

#pragma endregion

#pragma region 鼠标

int mouseX, mouseY;

#pragma endregion

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
	glRotatef(rotateAroundX, 1, 0, 0);
	glRotatef(rotateAroundY, 0, 1, 0);
	glTranslatef(-posX, -posY, -posZ);
	glScalef(scale, scale, scale);

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

void Internal_KeyboardFunc(unsigned char key, int x, int y) {
	key = tolower(key);
	switch (key) {
		case 27: // ESC: 退出程序
			exit(0);
			break;

		case 'z':// 物体放大
			scale += 0.05f;
			break;

		case 'x':// 物体缩小
			scale -= 0.05f;
			break;

		case 'w':// 前进
			posZ += 0.05f;
			targetZ += 0.05f;
			break;

		case 's':// 后退
			posZ -= 0.05f;
			targetZ -= 0.05f;
			break;

		case 'a':// 左移
			posX -= 0.05f;
			targetX -= 0.05f;
			break;

		case 'd':// 右移
			posX += 0.05f;
			targetX += 0.05f;
			break;

		case 'r':// 上移
			posY += 0.05f;
			targetY += 0.05f;
			break;

		case 'f':// 下移
			posY -= 0.05f;
			targetY -= 0.05f;
			break;

		case 'q':// 左旋
			rotateAroundY += 2.f;
			break;

		case 'e':// 右旋
			rotateAroundY -= 2.f;
			break;

		case 't':// 上旋
			rotateAroundX += 2.f;
			if (rotateAroundX > 90.f) rotateAroundX = 90.f;
			break;

		case 'g':// 下旋
			rotateAroundX -= 2.f;
			if (rotateAroundX < -90.f) rotateAroundX = -90.f;
			break;

		case 'm':// 复位
			posX = posY = 0.0f;
			posZ = -5;
			targetX = targetY = targetZ = 0.0f;
			break;

		default:
			break;
	}
}

void Internal_SpecialFunc(int specialKey, int x, int y) { }


void Internal_MouseMotion(int x, int y) {
	int dx, dy;

	dx = x - mouseX;
	dy = y - mouseY;

	// 修改摄像机或场景
	rotateAroundY += dy;
	rotateAroundX += dx;

	// 保存鼠标当前位置，留作后用
	mouseX = x;
	mouseY = y;
}
