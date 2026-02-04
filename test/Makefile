MLX_DIR = libs/minilibx-linux/
MLX = $(MLX_DIR)/libmlx.a

LIBFT_DIR = libs/libft
LIBFT = $(LIBFT_DIR)/libft.a

LDFLAGS = -L $(LIBFT_DIR) -lft \
          -L $(MLX_DIR) -lmlx \
          -lX11 -lbsd -lXext -lX11 -lm

$(LIBFT):
	make -C $(LIBFT_DIR)


$(MLX):
	make -C $(MLX_DIR)

game: $(MLX) $(LIBFT)
	cc code/teste.c $(LDFLAGS) $(LIBFT) $(MLX)
