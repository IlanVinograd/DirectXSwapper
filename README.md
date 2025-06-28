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
  
## Credits

Special thanks to **Elisha Riedlinger** for foundational work and architectural references used in this project.
