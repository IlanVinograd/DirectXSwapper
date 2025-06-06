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
        if (ImGui::BeginTabItem("Geometry")) {
            geometryLogic();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Textures")) {
            // Not ready
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Debug")) {
            debugLogic();
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

bool OpenFileDialogForExe(char* outPath, DWORD outPathSize) {
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "Blender Executable\0*.exe\0All Files\0*.*\0";
    ofn.lpstrFile = outPath;
    ofn.nMaxFile = outPathSize;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrTitle = "Select Blender Executable";

    return GetOpenFileNameA(&ofn);
}

void debugLogic() {
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
}

void geometryLogic() {
    try {
        std::error_code ec;
        if (!std::filesystem::exists("Exported")) {
            std::filesystem::create_directory("Exported", ec);
            if (ec) {
                Logger::LogInfo() << "[geometryLogic] Failed to create Exported/: " << ec.message();
            }
        }

        ImGui::Button(Button_1 ? "CAPTURE: ON  (C)" : "CAPTURE: OFF (C)");

        if (ObjectExporter::running) {
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
            ImGui_Spinner("ExportSpinner", 8.0f, 2.0f, IM_COL32(100, 255, 255, 255));
        }

        ImGui::Separator();

        if (ImGui::Button(Button_2 ? "START EXPORT: ON" : "STOP EXPORT: OFF"))
            Button_2 = !Button_2;

        ImGui::Text("Filter");
        ImGui::SliderInt("Object Size", &Filter, 0, 1000000);

        ImGui::Separator();

        if (ImGui::Button("Reset Hashes")) {
            ObjectExporter::resetHashes();
        }

        ImGui::Separator();

        enum SortMode { ByName, BySize, ByDate };
        static SortMode sortMode = ByName;
        ImGui::Combo("Sort by", (int*)&sortMode, "Name\0Size\0Date\0");

        struct FileInfo {
            std::string name;
            uintmax_t size;
            std::filesystem::file_time_type time;
        };

        std::vector<FileInfo> fileInfos;

        if (std::filesystem::exists("Exported")) {
            for (const auto& entry : std::filesystem::directory_iterator("Exported")) {
                if (entry.path().extension() == ".obj") {
                    FileInfo info;
                    info.name = entry.path().filename().string();
                    info.size = std::filesystem::file_size(entry.path(), ec);
                    info.time = std::filesystem::last_write_time(entry.path(), ec);
                    fileInfos.push_back(info);
                }
            }
        }

        std::sort(fileInfos.begin(), fileInfos.end(), [&](const FileInfo& a, const FileInfo& b) {
            if (sortMode == ByName)
                return a.name < b.name;
            if (sortMode == BySize)
                return a.size > b.size;
            if (sortMode == ByDate)
                return a.time > b.time;
            return false;
            });

        ImGui::Text("Exported .obj files:");
        if (ImGui::BeginListBox("##obj_listbox", ImVec2(-FLT_MIN, 200))) {
            for (size_t i = 0; i < fileInfos.size(); ++i) {
                const bool is_selected = (selectedObjIndex == static_cast<int>(i));

                std::time_t t = FileTimeToTimeT(fileInfos[i].time);
                char timeBuf[32];
                ctime_s(timeBuf, sizeof(timeBuf), &t);
                timeBuf[strcspn(timeBuf, "\n")] = 0;

                std::string label = fileInfos[i].name +
                    " | " + std::to_string(fileInfos[i].size / 1024) + " KB" +
                    " | " + timeBuf;

                if (ImGui::Selectable(label.c_str(), is_selected)) {
                    selectedObjIndex = static_cast<int>(i);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        if (selectedObjIndex >= 0 && static_cast<size_t>(selectedObjIndex) < fileInfos.size()) {
            const std::string& selectedName = fileInfos[selectedObjIndex].name;
            std::string fullPath = std::filesystem::absolute("Exported/" + selectedName).string();

            if (ImGui::Button("Open in Explorer")) {
                std::string explorerArg = "/select,\"" + fullPath + "\"";
                ShellExecuteA(nullptr, "open", "explorer.exe", explorerArg.c_str(), nullptr, SW_SHOWNORMAL);
            }

            ImGui::SameLine();

            if (ImGui::Button("Open in 3DViewer.net")) {
                ShellExecuteA(nullptr, "open", "https://3dviewer.net", nullptr, nullptr, SW_SHOWNORMAL);
            }

            ImGui::SameLine();

            if (ImGui::Button("Delete Selected")) {
                std::filesystem::remove("Exported/" + selectedName, ec);
                selectedObjIndex = -1;
            }
        }

        if (ImGui::Button("Delete All")) {
            for (const auto& info : fileInfos) {
                std::filesystem::remove("Exported/" + info.name, ec);
            }
            selectedObjIndex = -1;
        }

    }
    catch (const std::exception& e) {
        Logger::LogInfo() << "[geometryLogic] Exception: " << e.what();
    }
    catch (...) {
        Logger::LogInfo() << "[geometryLogic] Unknown exception occurred.";
    }
}

std::time_t FileTimeToTimeT(std::filesystem::file_time_type fileTime) {
    using namespace std::chrono;

    auto sctp = time_point_cast<system_clock::duration>(
        fileTime - std::filesystem::file_time_type::clock::now()
        + system_clock::now()
    );

    return system_clock::to_time_t(sctp);
}

void ImGui_Spinner(const char* label, float radius, float thickness, const ImU32& color) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float time = (float)ImGui::GetTime();

    float start_angle = time * 8.0f;
    float end_angle = start_angle + 3.14159265f * 1.5f;

    ImVec2 center = ImVec2(pos.x + radius, pos.y + radius);
    draw_list->PathArcTo(center, radius, start_angle, end_angle, 32);
    draw_list->PathStroke(color, false, thickness);
    ImGui::Dummy(ImVec2((radius + thickness) * 2, (radius + thickness) * 2));
}