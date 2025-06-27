#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx9.h"
#include "imgui/backends/imgui_impl_win32.h"

#include <Windows.h>
#include "d3d9.h"
#include "d3dx9.h"
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <cmath>

#include "ObjectExporter.h"
#include "logger.h"

namespace fs = std::filesystem;

extern bool g_ImGuiInitialized;
extern bool g_lastButtonState;
extern bool Button_1;
extern bool Button_2;

extern ImFont* g_FontLarge;

extern int fromFilter;
extern int toFilter;

static int selectedObjIndex = -1;
static std::vector<std::string> objFiles;
static char blenderPath[MAX_PATH] = "blender.exe";

void InitGUI(HWND& g_hFocusWindow, LPDIRECT3DDEVICE9EX& ProxyInterface);
void startFrame();
void renderFrame();
void injectMouse(HWND& g_hFocusWindow, POINT& pt);
void menuBar();
void menuTab();
void fontSize(float font_size);
bool OpenFileDialogForExe(char* outPath, DWORD outPathSize);
std::time_t FileTimeToTimeT(std::filesystem::file_time_type fileTime);
void ImGui_Spinner(const char* label, float radius = 10.0f, float thickness = 4.0f, const ImU32& color = IM_COL32(255, 255, 255, 255));

void debugLogic();
void geometryLogic();