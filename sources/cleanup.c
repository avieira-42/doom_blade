#include <stdlib.h>
#include "cmlx_base.h"
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

void	stt_error(const char *msg)
{
	char			buffer[1024];
	const char		prefix[8] = "Error\n";
	const size_t	msg_length = ft_strlen(msg) % 512;

	ft_memcpy(buffer, prefix, 6);
	ft_memcpy(buffer + 6, msg, msg_length);
	write(STDERR_FILENO, buffer, msg_length + 6);
}

static
void	stt_free_assets(t_assets *assets, t_block *blocks, t_player *player)
{
	size_t	i;

	free(player->hands.reload.texture.ptr);
	free(player->hands.shoot.texture.ptr);
	free(player->hands.walk.texture.ptr);
	// free(assets->city.texture.ptr);
	free(player->hands.radar.texture.ptr);
	free(player->hands.radar_l0.texture.ptr);
	free(player->hands.radar_l1.texture.ptr);
}

static
void	stt_free_enemies(t_game *game)
{
	free(game->assets.hud_blood.texture.ptr);
}

int	cub_cleanup(t_game *game, const char *msg)
{
	const int	rvalue = msg == NULL;

	if (msg != NULL && *msg != 0)
		stt_error(msg);
	free(game->file);
	free(game->map.tex_index);
	if (game->mlx != NULL && game->mlx->win_list != NULL)
		mlx_destroy_window(game->mlx, game->mlx->win_list);
	if (game->mlx != NULL && game->frame.img != NULL)
		mlx_destroy_image(game->mlx, game->frame.img);
	if (game->mlx != NULL)
		mlx_destroy_display(game->mlx);
	stt_free_assets(&game->assets, game->blocks, &game->player);
	stt_free_enemies(game);
	exit(rvalue);
}
