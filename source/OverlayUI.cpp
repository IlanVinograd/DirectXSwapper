#include "OverlayUI.h"

bool g_lastButtonState = false;
bool g_ImGuiInitialized = false;
bool Button_1 = false;
bool Button_2 = false;

ImFont* g_FontLarge = nullptr;

int Filter = 0;

void InitGUI(HWND& g_hFocusWindow, LPDIRECT3DDEVICE9EX& ProxyInterface) {
    if (!g_ImGuiInitialized)
    {
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(g_hFocusWindow);
        ImGui_ImplDX9_Init(ProxyInterface);
        g_ImGuiInitialized = true;
    }

    fontSize(22.0f);
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

void menuBar() {
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Do stuff */ }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}


void menuTab() {
    if (ImGui::BeginTabBar("Tabs")) {
        if (ImGui::BeginTabItem("General")) {

            ImGui::Button(Button_1 ? "CAPTURE: ON (C)" : "CAPTURE: OFF (C)");

            ImGui::Separator();

            if (ImGui::Button(Button_2 ? "START EXPORT: ON" : "STOP EXPORT: OFF"))
                Button_2 = !Button_2;

            ImGui::Text("Filter");
            ImGui::SliderInt("Object Size", &Filter, 0, 1000000);

            ImGui::Separator();

            if (ImGui::Button("Reset Hashes")) {
                ObjectExporter::resetHashes();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Geometry")) {

            // Here I want to be able to see all .obj files from folder exported and be able run script to open it in blender right from the game.

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Textures")) {

            // Not ready 

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Debug")) {
            static ImGuiTextFilter filter;
            static bool autoScroll = true;

            if (ImGui::Button("Clear Log")) {
                Logger::ClearLogBuffer();
            }

            ImGui::SameLine();
            ImGui::Checkbox("Auto-scroll", &autoScroll);

            ImGui::SameLine();
            filter.Draw("Filter", 200);

            ImGui::Separator();

            ImGui::BeginChild("LogArea", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

            const auto& logLines = Logger::CopyLogBuffer();

            for (const std::string& line : logLines) {
                if (!filter.PassFilter(line.c_str()))
                    continue;

                if (line.find("[ERROR]") != std::string::npos)
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 80, 80, 255));
                else if (line.find("[WARN]") != std::string::npos)
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 200, 0, 255));
                else if (line.find("[DEBUG]") != std::string::npos)
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 150, 255, 255));
                else
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 255, 180, 255)); // INFO

                ImGui::TextUnformatted(line.c_str());
                ImGui::PopStyleColor();
            }

            if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void fontSize(float font_size) {
    ImGuiIO& io = ImGui::GetIO();
    if (!g_FontLarge) {
        g_FontLarge = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", font_size);
        io.FontDefault = g_FontLarge;
    }
}