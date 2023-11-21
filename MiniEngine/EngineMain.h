#pragma once

void BeginEngine(int* argc, char** argv);
void StartEngine();
void EndEngine();

static void Internal_TickEngineAction();
static void Internal_UpdateTimeAndFrameCount();