#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game_defines.h"
#include "game_prototypes.h"
#include "game_types.h"

static
void	stt_load_texture(t_game *game, t_str path, size_t count, t_mat32 *dst)
{
	static const char	append_str[5] = ".png";
	size_t				number_length;
	char				*append_ptr;

	append_ptr = path.ptr + path.length;
	number_length = ft_itoa_r((int64_t)count, append_ptr);
	append_ptr += number_length;
	memcpy(append_ptr, append_str, sizeof(append_str));
	cub_read_texture(game, dst, path.ptr, NULL);
}

// Loads a width by height image, and with already allocated memory, 
// saves an image there transposing it. Path is different as well
// Saves images sequentially in memory, in a row x col x depth matrix
t_sheet	cub_readsheet(t_game *game, const char *base_path, size_t count, long frame_time)
{
	size_t			i;
	t_sheet			sheet;
	t_mat32			tex;
	char			buffer[256];
	const t_str		path = {buffer, strlen(base_path)};

	if (path.length + 32 > sizeof(buffer))
		exit_log(2, "Texture path is too long", 1);
	memcpy(buffer, base_path, path.length + 1);
	tex = (t_mat32){NULL, 0, 0, count, 0};
	stt_load_texture(game, path, 0, &tex);
	sheet = (t_sheet){tex, frame_time, 0, tex.width * tex.height, 0, count};
	i = 1;
	tex.ptr += sheet.frame_size;
	while (i < count)
	{
		stt_load_texture(game, path, i, &tex);
		tex.ptr += sheet.frame_size;
		i++;
	}
	return (sheet);
}
