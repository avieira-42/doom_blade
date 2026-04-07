*This project has been created as part of the 42 curriculum by <login1>[, <login2>[, <login3>[...]]].*

# Cub3D — Raycasting Engine

## Description

Cub3D is a **2.5D raycasting engine** inspired by early FPS games like **Wolfenstein 3D**.

The objective of this project is to implement a full rendering pipeline from scratch using the **DDA (Digital Differential Analyzer)** algorithm. It covers:

- Map parsing and validation  
- Texture loading and sampling  
- Player movement and collision detection  
- Sprite and enemy rendering  
- Input handling (keyboard and mouse)  
- Performance-aware memory management  

The project is written in **C** using **MiniLibX**, with a focus on:

- **Determinism** — no undefined behavior or crashes  
- **Extensibility** — easy addition of new blocks, textures, and entities  
- **Clarity** — well-separated systems (parsing, rendering, input, loop)  

### Extra Features

- Texture transposition for optimized sampling  
- Sprite-based enemy rendering with animation  
- Horizontal and vertical mouse look  
- Support for custom block texture indices  
- Flexible configuration (textures or RGB colors)  

---

## Instructions

### Dependencies

- MiniLibX (42 provided)  
- C compiler (`cc` or `clang`)  
- Make  

---

### Compilation

```bash
make
```

Other commands:

```bash
make re      # full rebuild
make clean   # remove object files
make fclean  # remove objects + executable
```

---

### Execution

```bash
./cub3D path/to/map.cub
```

At launch, the program:

- Validates the `.cub` file  
- Parses textures and colors  
- Loads and processes textures  
- Validates the map layout  
- Ensures exactly one player position  
- Initializes MiniLibX and rendering buffers  

On error, the program exits with a descriptive message.

---

### Controls

**Movement**
- W / S — Forward / Backward  
- A / D — Strafe Left / Right  
- Shift — Sprint  
- ESC — Exit  

**Camera**
- Arrow keys — Rotate view  
- Mouse — Smooth horizontal rotation  
- Vertical mouse — Pitch (if enabled)  

**Interaction**
- E — Interact with doors  

---

## Map & Configuration

## Texture Parsing
Wall textures should be square and ideally be as close to 128x128 as possible, because when parsed, they will be scaled to that resolution using nearest neighbour scaling

When a wall texture isn't supplied, the game renders a pink color instead to signify missing textures.

## Map Parsing:
Block index refers to the index that will be parsed from the map;
Cardinal directions refer to how the textures will look when seen from the north/south/etc.. 

The map parsing reads textures for the default walls like so:

NO path/to/north_texture.xpm
SO path/to/south_texture.xpm
WE path/to/west_texture.xpm
EA path/to/east_texture.xpm
F path/to/floor.xpm
C path/to/ceiling.xpm

You can also optionally load colors instead of textures by using the RGB format, where a byte value is read into each color channel (R, G, B)

F 255,222,202

To load textures beyond the default mandatory ones, just use the format T(x)(y), where x is the block index, and y is the cardinal direction. 

Example:
T2N assets/sprites/xpm/tiles/castle/castle_door0.xpm
T2E assets/sprites/xpm/tiles/castle/castle_door0.xpm
T2S assets/sprites/xpm/tiles/castle/castle_door0.xpm
T2W assets/sprites/xpm/tiles/castle/castle_door0.xpm

Block index 2 is reserved for door functionality;
Spaces are parsed as 1;

Maps that are invalid:
A map that isn't surrounded by walls;
A map that has more than one player positions or none


## Map Example:
NO assets/sprites/xpm/tiles/lab0.xpm
SO assets/sprites/xpm/tiles/lab1.xpm
WE assets/sprites/xpm/tiles/lab2.xpm
EA assets/sprites/xpm/tiles/lab3.xpm
F assets/sprites/xpm/tiles/floor0.xpm
C assets/sprites/xpm/tiles/ceiling0.xpm

T2N assets/sprites/xpm/tiles/gates/gateGore0.xpm
T2E assets/sprites/xpm/tiles/gates/gateGore0.xpm
T2S assets/sprites/xpm/tiles/gates/gateGore0.xpm
T2W assets/sprites/xpm/tiles/gates/gateGore0.xpm

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
111111111011000001110000000000001
100000000011000001111111111111111
11110111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10000000000000022200000010001
10000000000000011101010000001
11000001110101011111011110N0111
11110111 1110101 101111010001
11111111 1111111 111111111111

---

## Engine Overview

### Initialization

- Argument parsing  
- File loading  
- Texture initialization  
- Memory allocation  
- MLX setup  

---

### Parsing & Validation

- Configuration extraction  
- Texture and RGB validation  
- Map normalization  
- Player detection  
- Map enclosure verification  

---

### Texture System

- XPM loading  
- Scaling to 128×128  
- Pixel buffer conversion  
- Texture transposition  

Transposition improves cache locality and rendering performance during vertical sampling.

---

### Raycasting (DDA)

For each screen column:

- Compute ray direction  
- Perform grid traversal (DDA)  
- Calculate wall distance  
- Determine texture coordinates  
- Render vertical slice  

Supports:

- Multiple block types  
- Direction-based textures  
- Depth buffering  

---

### Sprite Rendering

- Transform to camera space  
- Project onto screen  
- Depth comparison  
- Render non-transparent pixels  

Supports animated and state-based sprites.

---

### Input System

- Keyboard event handling  
- Mouse tracking  
- Frame-based updates  

Mouse is recentered each frame for continuous rotation.

---

### Game Loop

- Input processing  
- Movement and collision  
- Raycasting  
- Sprite rendering  
- Frame display  

---

## Resources

### Documentation & Tutorials

- LodeV Raycasting Tutorial  
- https://www.youtube.com/watch?v=NbSee-XM7WA  
- Freya Holmer — Math for Game Developers  
- 3Blue1Brown — Linear Algebra  

---

### AI Usage Disclosure

AI tools were used for:

- Code review and debugging assistance  
- Clarification of mathematical concepts  
- Documentation refinement  

All core systems — including raycasting (DDA), parsing, rendering, and input handling — were implemented manually.
