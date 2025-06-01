workspace "DirectXSwapper"
   configurations { "Release", "Debug" }
   platforms { "Win32", "Win64" }
   location "build"
   objdir ("build/obj")
   buildlog ("build/log/%{prj.name}.log")
   buildoptions {"-std:c++latest"}

project "DirectXSwapper"
   kind "SharedLib"
   language "C++"
   targetname "d3d9"
   targetextension ".dll"
   characterset ("MBCS")
   staticruntime "On"

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

   files { "source/*.h", "source/*.cpp" }
   files { "source/*.def" }
   files { "source/*.rc" }
   includedirs { "source/dxsdk" }

   filter "configurations:Debug"
      defines "DEBUG"
      symbols "On"

   filter "configurations:Release"
      defines "NDEBUG"
      optimize "On"

   filter "platforms:Win32"
      architecture "x32"
      targetdir "data"
      libdirs { "source/dxsdk/lib/x86" }

   filter "platforms:Win64"
      architecture "x64"
      targetdir "data/x64"
      libdirs { "source/dxsdk/lib/x64" }