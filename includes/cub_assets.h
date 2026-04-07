#ifndef CUB_ASSETS_H
# define CUB_ASSETS_H

# include "cub_defines.h"

# define MAP_CASTLE

# define PLAYER_ATK "assets/sprites/xpm/hud/hands/hands_shooting"
# define PLAYER_WALK "assets/sprites/xpm/hud/hands/hands_walking"
# define PLAYER_RELOAD "assets/sprites/xpm/hud/hands/hands_reloading"
# define PLAYER_RADAR "assets/sprites/xpm/hud/hands/hands_radar"
# define PLAYER_RADAR0 "assets/sprites/xpm/hud/hands/map/layer0_"
# define PLAYER_RADAR1 "assets/sprites/xpm/hud/hands/map/layer1_"
# define PLAYER_BLOOD "assets/sprites/xpm/hud/damage/damage"
# define PLAYER_IDLE "assets/sprites/xpm/hud/hands/hands_idle"

# ifdef MAP_HELL
 # define EWALK "assets/sprites/xpm/characters/zombie/EWALKing_front"
 # define EATK "assets/sprites/xpm/characters/zombie/enemy_shooting"
 # define EHIT "assets/sprites/xpm/characters/zombie/enemy_shot"
 # define EDEATH "assets/sprites/xpm/characters/zombie/enemy_exploding"
 # define EWALK_COUNT 4
 # define EATK_COUNT 2
 # define EHIT_COUNT 1
 # define EDEATH_COUNT 12
 # define EWALK_SPEED 3 * ANIM_TIME
 # define EATK_SPEED 3 * ANIM_TIME
 # define EHIT_SPEED 3 * ANIM_TIME
 # define EDEATH_SPEED 1 * ANIM_TIME
# endif

# ifdef MAP_CASTLE
 # define EWALK "assets/sprites/xpm/characters/gentleman/gentleman_running"
 # define EATK "assets/sprites/xpm/characters/gentleman/gentleman_shooting"
 # define EHIT "assets/sprites/xpm/characters/gentleman/gentleman_hurt"
 # define EDEATH "assets/sprites/xpm/characters/gentleman/gentleman_dying"
 # define EWALK_COUNT 4
 # define EATK_COUNT 3
 # define EHIT_COUNT 1
 # define EDEATH_COUNT 4
 # define EWALK_SPEED 3 * ANIM_TIME
 # define EATK_SPEED 4 * ANIM_TIME
 # define EHIT_SPEED 3 * ANIM_TIME
 # define EDEATH_SPEED 2 * ANIM_TIME
# endif

#endif
