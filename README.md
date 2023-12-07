# Minecraft clone
This project uses OpenGL for rendering and entt for the entity component system.
![alt text](screenshot.png)
For a 51-second long video demo on the game as of 2023-12-06, check out [https://www.youtube.com/watch?v=_cHa_oqkxno](https://www.youtube.com/watch?v=_cHa_oqkxno)
## Features
- Frustum culling
- Swept AABB Collision Detection
- Ambient Occlusion
- Terrain Generation using perlin noise
- Multithreaded terrain and chunk mesh generation using a thread pool
### TODO
- Better terrain generation: more dramatic hills
- Flowers
- Water
- proper Lighting (e.g. caves should be pitch black)
- day-night cycle
- UI for selecting another block type
- multiplayer (super long term)
## Building
Currently you can only build this project on macos using clang. I'm planning to add support for MSVC soon. Will probably
have to fix a few header issues.
## Gameplay
WASD for horizontal movement, space for jumping

Left click to remove blocks, right click to place a grass block

Once your mouse gets captured by the window, you need to press Ctrl/Cmd + Q to quit. 
## Credits
### Inspiration
- [mc-one-week](https://github.com/Hopson97/MineCraft-One-Week-Challenge)
- [CityBuilderGame](https://github.com/PhiGei2000/CityBuilderGame)

## External libraries
- Perlin Noise (`src/PerlinNoise.cpp` and `src/PerlinNoise.h`): https://github.com/sol-prog/Perlin_Noise/