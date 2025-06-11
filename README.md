# DirectXSwapper

**DirectXSwapper** is a lightweight DirectX proxy tool for **real-time mesh and texture extraction**, overlay rendering, and debugging in Direct3D9 games.

## Preview

![https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/Animation3.gif](https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/Animation4.gif)
![https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/Portal%20gun.png](https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/img.png)

## Project Updates

-  **[06/08/2025]** Added **texture export** support for Direct3D9 – bound textures can now be saved as `.png`
-  **[06/11/2025]** Critical fix: incorrect UV extraction resolved
  
## Features

- Extract mesh geometry to `.obj` files  
- Save bound textures as `.png`  
- In-game overlay using **ImGui**  
- Live debug info and filtering  
- Minimal performance overhead  

## Usage

1. Build the project
2. Copy the proxy DLL (`d3d9.dll`) into the game folder (next to the `.exe` or in the `bin/` directory)
3. Launch the game – you should see a message box confirming that the DLL was injected
4. Use the **ImGui** overlay to activate capture
5. Press `C` to export the currently rendered mesh
6. View exported assets in the `Exported/` directory
7. View exported textures in the `Textures/` directory
   
## Output

- Geometry: `.obj` files are saved to `Exported/`
- Textures: `.png` files are saved to `Textures/`

## Planned Features

- Support for DirectX10 / 11 / 12
- Standalone **injector app** (no need to copy DLL into game folder manually)
- Export to **glTF** format
  
## Credits

Special thanks to **Elisha Riedlinger** for foundational work and architectural references used in this project.
