#include "OverlayUI.h"

bool g_ImGuiInitialized = false;
bool Button_1 = false;

void InitGUI(HWND& g_hFocusWindow, LPDIRECT3DDEVICE9EX& ProxyInterface) {
    if (!g_ImGuiInitialized)
    {
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(g_hFocusWindow);
        ImGui_ImplDX9_Init(ProxyInterface);
        g_ImGuiInitialized = true;
    }
}

void startFrame() {
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void renderFrame() {
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void injectMouse(HWND& g_hFocusWindow, POINT& pt) {
    GetCursorPos(&pt);
    ScreenToClient(g_hFocusWindow, &pt);

    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)pt.x, (float)pt.y);
    io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}