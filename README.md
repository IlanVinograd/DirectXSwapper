# DirectXSwapper

**DirectXSwapper** is a lightweight DirectX proxy tool for **real-time mesh and texture extraction**, overlay rendering, and debugging in Direct3D9 games.  
It now also includes **experimental support for Direct3D12** (64-bit only) with mesh export triggered via the `N` key (overlay not yet supported).


> [!Caution]
>  This tool is intended solely for research and educational purposes.  
>  It allows exploration of level geometry not normally visible in-game.  
>  It is not designed for piracy or unauthorized asset distribution.  


## Preview

![https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/Animation3.gif](https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/Animation4.gif)
![https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/Portal%20gun.png](https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/img.png)

## Project Updates

-  **[06/08/2025]** Added **texture export** support for Direct3D9 – bound textures can now be saved as `.png`
-  **[06/11/2025]** Critical fix: incorrect UV extraction resolved
-  **[06/11/2025]** Added support for `D3DPT_TRIANGLESTRIP` primitive type
-  **[06/27/2025]** Added **experimental Direct3D12 proxy support** for **64-bit games** – enabling early mesh extraction and resource logging for D3D12 titles
   > ⚠️ Overlay is **not available yet** for D3D12.  
   > Press **`N`** during gameplay to export the currently rendered mesh.
  
## Features

- Extract mesh geometry to `.obj` files  
- Save bound textures as `.png`  
- In-game overlay using **ImGui** (D3D9 only)  
- Live debug info and mesh filtering  
- Minimal performance overhead  
- Experimental support for Direct3D12 (64-bit games)

## Usage

### Direct3D9 Games

1. Build the project
2. Copy the proxy DLL (`d3d9.dll`) into the game folder (next to the `.exe` or inside the `bin/` directory)
3. Launch the game – a message box should appear confirming injection
4. Use the **ImGui** overlay to control capture
5. Press `C` to export the currently rendered mesh
6. View exported assets in the `Exported/` directory
7. View exported textures in the `Textures/` directory

### Direct3D12 Games (Experimental)

1. Build the project
2. Copy the proxy DLL (`d3d12.dll`) into the game folder (next to the `.exe`)
3. Launch the game – there is currently **no overlay**
4. Press `N` while in-game to export the currently rendered mesh
5. View exported assets in the `Exported/` directory

## Output

- Geometry: `.obj` files are saved to `Exported/`
- Textures: `.png` files are saved to `Textures/`

## Planned Features

- Full support for DirectX10 / 11
- Standalone injector application (no manual DLL copying)
- Export to `.glTF` format
- Overlay support for D3D12

## Build Instructions

This project is designed to be built using **Microsoft Visual Studio 2022** not with `g++` or command-line compilers.

This repository contains two projects:

- **`d3d9`**: Full-featured Direct3D9 proxy (supports overlay, texture export, mesh export)  
- **`d3d12`**: Experimental Direct3D12 proxy (64-bit only, mesh export only)  

You can build both projects using **Visual Studio 2022**.

---

### 1. Clone the Repository

```bash
git clone https://github.com/IlanVinograd/DirectXSwapper.git
cd DirectXSwapper
```

### 2. Open the Project in Visual Studio

1. Open **Visual Studio 2022**  
2. Go to **File → Open → Project/Solution...**  
3. Select the solution file:  
   ```
   DirectXSwapper/DirectXSwapper.sln
   ```
4. Visual Studio will load the two subprojects:
   - `d3d9`
   - `d3d12`


---

### 3. Configure the Build

At the top of Visual Studio:

- **Configuration**: Choose `Release` (recommended) not `Debug`
- **Platform**:
  - For `d3d9`: choose either `Win32` or `Win64`
  - For `d3d12`: choose only `x64` (32-bit is **not supported**)

---

### 4. Build the Project

Use the menu:

```
Build → Build Solution
```

---

### 5. Locate the Output DLL

After building, the compiled DLLs will be found in the `data` folder inside the repository:

- **32-bit (`x32`) build**:
  ```
  DirectXSwapper\data\d3d9.dll
  ```

- **64-bit (`x64`) build**:
  ```
  DirectXSwapper\data\x64\d3d9.dll
  DirectXSwapper\data\x64\d3d12.dll
  ```

> **Note:**
> - `d3d9` supports both **32-bit** and **64-bit** builds  
> - `d3d12` supports **64-bit only**

---

### 6. Use the DLL

Copy the compiled DLL (`d3d9.dll` or `d3d12.dll`) into the game folder next to the game’s `.exe` file:

```text
C:\Games\SomeGame\ → paste the DLL here
```

### Rebuild with Premake (Optional)

This project also includes support for **Premake5**, which can be used to regenerate the Visual Studio solution and project files from `premake5.lua`.

#### To rebuild the solution manually:

1. Make sure you have `premake5.exe` in the project root.
2. Run the following in Command Prompt:

```bash
premake5.exe vs2022
```

This will generate the Visual Studio 2022 `.sln` and `.vcxproj` files under the `build/` directory.

You can now open `build/DirectXSwapper.sln` and build it as described above.

> ⚠️ **You do not need to run Premake if you're using the provided `.sln`** in the root folder it's already generated.

Premake is useful only if you're modifying the project structure or adding new files and want to regenerate project files automatically.

## Credits

Special thanks to **Elisha Riedlinger** for foundational work and architectural references used in this project.
