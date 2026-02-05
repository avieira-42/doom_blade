MLX_DIR = libs/minilibx-linux/
MLX = $(MLX_DIR)/libmlx.a

LIBFT_DIR = libs/libft
LIBFT = $(LIBFT_DIR)/libft.a

LDFLAGS = -L $(LIBFT_DIR) -lft \
          -L $(MLX_DIR) -lmlx \
          -lX11 -lbsd -lXext -lX11 -lm

CFLAGS = -g

$(LIBFT):
	make -C $(LIBFT_DIR)


$(MLX):
	make -C $(MLX_DIR)

game: $(MLX) $(LIBFT)
	cc $(CFLAGS) code/teste.c code/libft_math.c $(LDFLAGS) $(LIBFT) $(MLX)
