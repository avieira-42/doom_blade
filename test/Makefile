LDFLAGS = -L $(LIBFT_DIR) -lft \
          -L $(MLX_DIR) -lmlx \
          -lX11 -lbsd -lXext -lX11 -lm

CFLAGS = -Wall -Wextra -Werror -g -O3

CC = cc $(CFLAGS)

MLX_DIR = libs/minilibx-linux/
MLX = $(MLX_DIR)/libmlx.a

LIBFT_DIR = libs/libft
LIBFT = $(LIBFT_DIR)/libft.a

PARSE = code/parse.c

INPUT = code/input.c

CLEAN = code/clean.c

RAY_CAST = code/ray_cast.c

PHYSICS = code/physics.c

RENDER = code/render.c\
		 code/draw_line.c\
		 code/pixel_put.c\
		 code/draw_minimap.c

LIBFT_MATH = code/libft_math.c

MAIN = code/main.c

SRCS = $(RENDER)\
	   $(PARSE)\
	   $(INPUT)\
	   $(CLEAN)\
	   $(RAY_CAST)\
	   $(PHYSICS)\
	   $(LIBFT_MATH)\
	   $(MAIN)

OBJS = $(notdir $(SRCS:.c=.o))
OBJS := $(addprefix bin/, $(OBJS))


TOTAL := $(words $(SRCS))
BAR_LENGTH = 30
COUNTER = 0

define progress_bar
	$(eval COUNTER=$(shell echo $$(($(COUNTER)+1))))
	@percent=$$((100 * $(COUNTER) / $(TOTAL))); \
	filled=$$(( $(BAR_LENGTH) * $(COUNTER) / $(TOTAL) )); \
	empty=$$(( $(BAR_LENGTH) - $$filled )); \
	bar="$$(printf '%0.s#' $$(seq 1 $$filled))$$(printf '%0.s-' $$(seq 1 $$empty))"; \
	printf "Compiling samus_invasion objects [%s] %3d%%\r" "$$bar" $$percent;
endef

bin/%.o: code/%.c | bin
	@$(call progress_bar)
	@$(CC) -c $< -o $@

game: $(OBJS) $(MLX) $(LIBFT)
	cc $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBFT) $(MLX) -o game

bin:
	@mkdir bin

$(LIBFT):
	make -C $(LIBFT_DIR)


$(MLX):
	make -C $(MLX_DIR)

clean:
	@rm game

fclean:
	@rm -rf bin

re: fclean game
