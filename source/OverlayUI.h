#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx9.h"
#include "imgui/backends/imgui_impl_win32.h"

#include <Windows.h>
#include "d3d9.h"
#include "d3dx9.h"

extern bool g_ImGuiInitialized;
extern bool g_lastButtonState;
extern bool Button_1;
extern bool Button_2;

extern int Filter;

void InitGUI(HWND& g_hFocusWindow, LPDIRECT3DDEVICE9EX& ProxyInterface);
void startFrame();
void renderFrame();
void injectMouse(HWND& g_hFocusWindow, POINT& pt);