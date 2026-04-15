# Configuration ------------------------------- #
NAME = main
VPATH = sources sources/utils sources/input sources/parse \
		sources/gameplay sources/math sources/render sources/audio
MLX_DIR = libraries/mlx
MLX = $(MLX_DIR)/libmlx_Linux.a
SDL_LDLIBS = -lSDL2 -lSDL2_mixer -lSDL2main -lSDL2_image
MLX_LDLIBS = $(MLX) -lXext -lX11 -lm
LDLIBS = $(SDL_LDLIBS) $(MLX_LDLIBS)
# CORE ------------------------------------ #
SRCS = main.c cleanup.c
# RENDER ------------------------------------ #
SRCS += raycast.c planecast.c
SRCS += draw_texture.c draw_utils.c draw_world.c draw_ascii.c
SRCS += draw_enemy.c draw_overlay.c draw_viewmodel.c
# AUDIO -------------------------------------#
SRCS += audio.c
# MATH ------------------------------------ #
SRCS += integer_scaling.c int_limits.c int_abs.c float_math.c float_abs.c vec2_algebra.c vec2_math.c
# UTILS ------------------------------------ #
SRCS += rng.c cub_utils.c string.c io_basic.c memory.c char_ascii.c
# PARSE ------------------------------------ #
SRCS += init.c map.c parse_textures.c read_xpm.c parse_sheet.c
# GAMEPLAY ------------------------------------ #
SRCS += movement.c player_state.c enemy_state.c game_state.c
# INPUT ------------------------------------ #
SRCS += keyboard.c mouse.c input.c

# Defaults ------------------------------------ #
RM := rm -f
BUILD_PATH = build
INC_PATH = includes libraries/mlx
OBJ_PATH = $(BUILD_PATH)/obj
BIN = $(BUILD_PATH)/$(NAME)
OBJS = $(addprefix $(OBJ_PATH)/, $(SRCS:.c=.o))

# Flags --------------------------------------- #
CC = clang
CPPFLAGS = $(addprefix -I,$(INC_PATH))
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS =
DEBUG = -g -Wpedantic -Wcast-qual -Wfloat-equal -Wswitch-default -Wsign-conversion
ASAN = -fsanitize=address,undefined,leak -fno-omit-frame-pointer
TSAN = -fsanitize=thread -fno-omit-frame-pointer
FAST = -march=native -flto -O3 -ffast-math -fstrict-aliasing

# Pattern Rules: Compilation ------------------ #
$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Linking
$(BIN): $(MLX) $(OBJS) | $(BUILD_PATH)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(MLX):
	@make -C $(MLX_DIR)

mlx_fclean:
	$(RM) $(MLX_DIR)/*.a

# Directory
$(OBJ_PATH):
	@mkdir -p $@
$(BUILD_PATH):
	@mkdir -p $@

# Phonies ------------------------------------- #
all: $(BIN)

clean:
	$(RM) -r $(OBJ_PATH)

fclean: clean mlx_fclean
	$(RM) $(BIN) $(BONUS_BIN)

re: fclean all

debug: CFLAGS += $(DEBUG)
debug: clean $(BIN)

asan: CFLAGS += $(DEBUG) $(ASAN)
asan: LDFLAGS += $(ASAN)
asan: clean $(BIN)

tsan: CFLAGS += $(DEBUG) $(TSAN)
tsan: LDFLAGS += $(TSAN)
tsan: clean $(BIN)

fast: CFLAGS += $(FAST)
fast: LDFLAGS += -flto
fast: clean $(BIN)

.PHONY: all clean fclean re debug asan tsan fast
