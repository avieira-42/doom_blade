#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static
int stt_mlx_init(t_game *game)
{
    t_win_list *window;

    mlx_new_window(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "doom_blade");
    if (game->mlx->win_list == NULL)
        return (-1);
    game->img = mlx_int_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, ZPixmap);
    if (game->img == NULL)
        return (-1);
    window = game->mlx->win_list;
    mlx_hook(window, KeyPress, KeyPressMask, cmlx_keydown, game);
    mlx_hook(window, KeyRelease, KeyReleaseMask, cmlx_keyup, game);
    mlx_hook(window, ButtonPress, ButtonPressMask, cmlx_mousedown, game);
    mlx_hook(window, ButtonRelease, ButtonReleaseMask, cmlx_mouseup, game);
    mlx_mouse_hide(game->mlx, window);
    mlx_loop_hook(game->mlx, cmlx_loop, game);
    return (0);
}

static
t_cfg   stt_config(void)
{
    t_cfg   cfg;

    cfg.sens = 1.0f / 512.0f;
    return (cfg);
}

static
void    stt_params_init(t_game *game, t_memory *memory)
{
    const t_mat32   empty = {memory->empty_line, 1, RENDER_HEIGHT, 1, 0};

    //game->vd = ft_qsqrt(game->map.cols * game->map.cols + game->map.rows * game->map.rows);
    game->player.cam.dir = (t_vec2){.x.f = 0.0f, .y.f = 1.0f};
    game->player.cam.plane = (t_vec2){
        .x.f = -game->player.cam.dir.y.f * 0.66f,
        .y.f =  game->player.cam.dir.x.f * 0.66f
    };
    game->display_frame.ptr = (uint32_t*)game->img->data;
    game->display_frame.rows = game->img->height;
    game->display_frame.cols = game->img->width;
    game->render_frame.ptr = (uint32_t*)memory->render_frame;
    game->render_frame.rows = RENDER_WIDTH;
    game->render_frame.cols = RENDER_HEIGHT;
    ft_memset(memory->empty_line, 0, sizeof(memory->empty_line));
    game->blocks[0].east = empty;
    game->blocks[0].west = empty;
    game->player.move.speed.x.f = 0.0f;
    game->player.move.speed.y.f = 0.0f;
    game->cfg = stt_config();

	// TMP >>>>>>>
    game->time.prev = get_time();        // ADDED OLD
    time_delta_get(game);                // ADDED OLD
	// <<<<<<<< TMP
}

int cub_init(const char *filename, t_game *game, t_memory *memory)
{
    size_t      file_size;
    const char  *file = ft_read_all(filename, &file_size);
    const char  *str = file;

    ft_memset(game, 0, sizeof(*game));
    if (file == NULL)
        return (-1);    // TODO: PRINT ERROR
    game->mlx = mlx_init();
    if (game->mlx == NULL)
        return (-1);    // TODO: Print Error
    if (cub_read_textures(game->mlx, str, &str, game->blocks) == -1)    // Double check: sending by value wont affect the struct
        return (free(file), -1);
    if (cub_read_map(str, &game->map, &game->player) == -1)
        return (free(file), -1);
    free(file);
    if (stt_mlx_init(game) == -1)
        return (-1);
    // if (cub_is_map_enclosed(game->map, game->player.cam.pos) == -1)
    //  return (-1);
    stt_params_init(game, memory);

    //  ALEX SOEIRO INITS >>>
    hud_init(game);                       // ADDED OLD
    audio_init(game);                     // ADDED OLD
    game->frame.image = mlx_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);   // ADDED OLD
    game->frame.data = mlx_get_data_addr(                                     // ADDED OLD
            game->frame.image, &game->frame.bpp, &game->frame.size_line,
                &game->frame_endian);                                         // ADDED OLD
    game->w = false;                      // ADDED OLD
    game->a = false;                      // ADDED OLD
    game->s = false;                      // ADDED OLD
    game->d = false;                      // ADDED OLD
    game->r = false;                      // ADDED OLD
    game->p = false;                      // ADDED OLD
    game->shift = false;                  // ADDED OLD
    game->mouse_l = false;                // ADDED OLD
    game->pause = false;                  // ADDED OLD
    game->hud.gun.ammo = 8;               // ADDED OLD
    game->hud.gun.max_ammo = 8;           // ADDED OLD
    game->hud.gun.first_iterator = -1;    // ADDED OLD
    //  <<< ALEX SOEIRO INITS

    get_time();
    return (0);
}
