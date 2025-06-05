# DirectXSwapper

**DirectXSwapper** is a lightweight DirectX proxy tool for mesh extraction and debugging.

## Preview

![Preview](https://github.com/IlanVinograd/DirectXSwapper/blob/main/Img/Animation3.gif)

## Features

- Extract mesh data to `.obj` files  
- In-game overlay using ImGui  
- Debug information display

## Usage

1. Build the project
2. Copy the appropriate (`d3d9.dll`) into the game folder (next to the executable)
3. Launch the game
4. Activate export
5. Press `C` to extract the current mesh
6. Use the ImGui overlay to view debug information

## Output

- `.obj` files are saved to the `Exported/` directory

## Credits

Special thanks to **Elisha Riedlinger**, whose public work provided helpful insights and technical guidance used in parts of this project.
