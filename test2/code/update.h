#ifndef UPDATE_H
#define	UPDATE_H

# include "types.h"

void		time_delta_get(t_game *game);
void		player_move(t_player *player, t_cam cam, float dt);
void		camera_move(t_player player, t_cam *cam);
uint64_t 	time_get();
t_vecf32	update_player_dir(t_vecf32 player, t_vecf32 cam);

#endif
