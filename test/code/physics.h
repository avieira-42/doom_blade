#ifndef PHYSICS_H
#define	PHYSICS_H

# include "types.h"

void		time_delta_get(t_game *game);
uint64_t 	time_get();
void		character_move(t_game * game, t_player *player, float dt);
void		camera_move(t_vecf32 player_pos, t_cam *cam);
t_vecf32	update_player_dir(t_vecf32 player, t_vecf32 cam);

#endif
