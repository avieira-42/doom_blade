# Configuration ------------------------------- #
NAME = main
VPATH = sources sources/utils sources/events sources/parse sources/physics sources/math sources/render
SRCS = main.c audio.c cleanup.c init.c rng.c actions.c rng_utils.c \
raycast.c draw_texture.c draw_utils.c draw_enemy.c draw_world.c draw_hands.c draw_radar.c draw_number.c planecast.c draw_damage.c draw_enemies.c\
time.c io_basic.c memory.c char_ascii.c transpose.c int_limits.c float_limits.c float_abs.c vec2_scalar.c vec2_vec.c \
map.c textures.c animation.c \
convert.c str_search.c \
movement.c \
bilinear_interp.c integer_scaling.c\
keyboard.c mouse.c input.c
LDLIBS = libraries/mlx/libmlx_Linux.a -lXext -lX11 -lm -lz -lSDL2 -lSDL2_mixer -lSDL2main

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
$(BIN): $(OBJS) | $(BUILD_PATH)	
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# Directory
$(OBJ_PATH):
	@mkdir -p $@
$(BUILD_PATH):
	@mkdir -p $@

# Phonies ------------------------------------- #
all: $(BIN)

clean:
	$(RM) -r $(OBJ_PATH)

fclean: clean
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
