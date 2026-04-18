#include <stdlib.h>
#include <string.h>
#include "game_types.h"
#include "game_types.h"
#include "game_prototypes.h"

static
void	stt_error(const char *msg)
{
	char			buffer[1024];
	const char		prefix[8] = "Error\n";
	const size_t	msg_length = MAX(1000, strlen(msg));

	memcpy(buffer, prefix, 6);
	memcpy(buffer + 6, msg, msg_length);
	write(STDERR_FILENO, buffer, msg_length + 6);
}

static
void	stt_free_assets(t_assets *assets, t_player *player, t_enemy *enemy)
{
	free(player->hands.reload.texture.ptr);
	free(player->hands.shoot.texture.ptr);
	free(player->hands.walk.texture.ptr);
	free(player->hands.radar.texture.ptr);
	free(player->hands.radar_l0.texture.ptr);
	free(player->hands.radar_l1.texture.ptr);
	free(assets->hud_blood.texture.ptr);
	free(enemy->running.texture.ptr);
	free(enemy->shooting.texture.ptr);
	free(enemy->dying.texture.ptr);
	free(enemy->shot.texture.ptr);
	free(player->hands.idle.texture.ptr);
}

int	cub_cleanup(t_game *game, const char *msg)
{
	const int	rvalue = msg == NULL;

	if (msg != NULL && *msg != 0)
		stt_error(msg);
	free(game->file);
	free(game->map.tex_index);
	stt_free_assets(&game->assets, &game->player, game->enemies);
	exit(rvalue);
}
