#pragma once

void BeginEngine(int* argc, char** argv);
void StartEngine();
void EndEngine();

static void Internal_TickEngineAction();
static void Internal_UpdateTimeAndFrameCount();
static void Internal_KeyboardFunc(unsigned char key, int x, int y);
static void Internal_SpecialFunc(int specialKey, int x, int y);
static void Internal_MousePassiveMotion(int x, int y);
static void Internal_MouseMotion(int x, int y);
