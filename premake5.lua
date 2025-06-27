workspace "DirectXSwapper"
    configurations { "Debug", "Release" }
    platforms { "Win32", "Win64" }
    location "build"
    objdir ("build/obj")
    buildlog ("build/log/%{prj.name}.log")
    buildoptions { "-std:c++latest" }

project "d3d9"
    kind "SharedLib"
    language "C++"
    targetname "d3d9"
    targetextension ".dll"
    characterset ("MBCS")
    staticruntime "On"

    files {
        "d3d9/*.h",
        "d3d9/*.cpp",
        "d3d9/*.rc",
        "d3d9/*.def",
        "imgui/*.cpp",
        "imgui/backends/imgui_impl_dx9.cpp",
        "imgui/backends/imgui_impl_win32.cpp",
        "imgui/*.h",
        "imgui/backends/*.h"
    }

    removefiles {
        "imgui/imgui_demo.cpp"
    }

    includedirs {
        "d3d9/dxsdk",
        "imgui",
        "imgui/backends"
    }

    defines {
        "rsc_CompanyName=\"\"",
        "rsc_LegalCopyright=\"\"",
        "rsc_FileVersion=\"1.0.0.0\"",
        "rsc_ProductVersion=\"1.0.0.0\"",
        "rsc_InternalName=\"d3d9\"",
        "rsc_ProductName=\"d3d9\"",
        "rsc_OriginalFilename=\"d3d9.dll\"",
        "rsc_FileDescription=\"Direct3D9 Proxy DLL\"",
        "rsc_UpdateUrl=\"\""
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:Win32"
        architecture "x86"
        targetdir "data"
        libdirs { "d3d9/dxsdk/lib/x86" }

    filter "platforms:Win64"
        architecture "x64"
        targetdir "data/x64"
        libdirs { "d3d9/dxsdk/lib/x64" }