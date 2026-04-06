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
 # define ENEMY_WALK "assets/sprites/xpm/characters/zombie/enemy_walking_front"
 # define ENEMY_ATK "assets/sprites/xpm/characters/zombie/enemy_shooting"
 # define ENEMY_HIT "assets/sprites/xpm/characters/zombie/enemy_shot"
 # define ENEMY_DEATH "assets/sprites/xpm/characters/zombie/enemy_exploding"
 # define ENEMY_WALK_COUNT 4
 # define ENEMY_ATK_COUNT 2
 # define ENEMY_HIT_COUNT 1
 # define ENEMY_DEATH_COUNT 12
 # define ENEMY_WALK_SPEED 3 * ANIM_TIME
 # define ENEMY_ATK_SPEED 3 * ANIM_TIME
 # define ENEMY_HIT_SPEED 3 * ANIM_TIME
 # define ENEMY_DEATH_SPEED 1 * ANIM_TIME
# endif

# ifdef MAP_CASTLE
 # define ENEMY_WALK "assets/sprites/xpm/characters/gentleman/gentleman_running"
 # define ENEMY_ATK "assets/sprites/xpm/characters/gentleman/gentleman_shooting"
 # define ENEMY_HIT "assets/sprites/xpm/characters/gentleman/gentleman_hurt"
 # define ENEMY_DEATH "assets/sprites/xpm/characters/gentleman/gentleman_dying"
 # define ENEMY_WALK_COUNT 4
 # define ENEMY_ATK_COUNT 3
 # define ENEMY_HIT_COUNT 1
 # define ENEMY_DEATH_COUNT 4
 # define ENEMY_WALK_SPEED 3 * ANIM_TIME
 # define ENEMY_ATK_SPEED 4 * ANIM_TIME
 # define ENEMY_HIT_SPEED 3 * ANIM_TIME
 # define ENEMY_DEATH_SPEED 2 * ANIM_TIME
# endif

#endif
