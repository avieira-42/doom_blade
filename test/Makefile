MLX_DIR = libs/minilibx-linux/

MLX = $(MLX_DIR)/libmlx.a

LIBFT_DIR = libs/libft

LIBFT = $(LIBFT_DIR)/libft.a

LDFLAGS = -L $(LIBFT_DIR) -lft \
          -L $(MLX_DIR) -lmlx \
          -lX11 -lbsd -lXext -lX11 -lm


minilibx: $(MINILIBX_DIR)
	make -C $(MINILIBX_DIR)

game:
	cc code/teste.c $(LDFLAGS) $(LIBFT) $(MINILIBX)
