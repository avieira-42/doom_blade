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