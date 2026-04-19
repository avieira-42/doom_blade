#ifndef GAME_ASSETS_H
# define GAME_ASSETS_H

# include "game_defines.h"
# define MAP_CASTLE

# define PLAYER_ATK "assets/sprites/hud/hands/hands_shooting"
# define PLAYER_WALK "assets/sprites/hud/hands/hands_walking"
# define PLAYER_RELOAD "assets/sprites/hud/hands/hands_reloading"
# define PLAYER_RADAR "assets/sprites/hud/hands/hands_radar"
# define PLAYER_RADAR0 "assets/sprites/hud/hands/map/layer0_"
# define PLAYER_RADAR1 "assets/sprites/hud/hands/map/layer1_"
# define PLAYER_BLOOD "assets/sprites/hud/damage/damage"
# define PLAYER_IDLE "assets/sprites/hud/hands/hands_idle"

# ifdef MAP_HELL
#  define EWALK "assets/sprites/characters/zombie/EWALKing_front" // TODO: FIX
#  define EATK "assets/sprites/characters/zombie/enemy_shooting"
#  define EHIT "assets/sprites/characters/zombie/enemy_shot"
#  define EDEATH "assets/sprites/characters/zombie/enemy_exploding"
#  define EWALK_COUNT 4
#  define EATK_COUNT 2
#  define EHIT_COUNT 1
#  define EDEATH_COUNT 12
#  define EWALK_SPEED VSLOW_ANIM_TIME
#  define EATK_SPEED VSLOW_ANIM_TIME
#  define EHIT_SPEED VSLOW_ANIM_TIME
#  define EDEATH_SPEED ANIM_TIME
# endif

# ifdef MAP_CASTLE
#  define EWALK "assets/sprites/characters/gentleman/gentleman_running"
#  define EATK "assets/sprites/characters/gentleman/gentleman_shooting"
#  define EHIT "assets/sprites/characters/gentleman/gentleman_hurt"
#  define EDEATH "assets/sprites/characters/gentleman/gentleman_dying"
#  define EWALK_COUNT 4
#  define EATK_COUNT 3
#  define EHIT_COUNT 1
#  define EDEATH_COUNT 4
#  define EWALK_SPEED VSLOW_ANIM_TIME
#  define EATK_SPEED VSLOW_ANIM_TIME
#  define EHIT_SPEED VSLOW_ANIM_TIME
#  define EDEATH_SPEED SLOW_ANIM_TIME
# endif

#endif
