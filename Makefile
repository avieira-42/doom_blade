# Configuration ------------------------------- #
NAME = cub3D
VPATH = sources sources/utils sources/input sources/parse \
		sources/gameplay sources/math sources/render sources/audio
# CORE ---------------------------------------- #
UNIX_SRCS = main.c
WEB_SRCS = web_main.c
# RENDER -------------------------------------- #
SRCS += raycast.c planecast.c
SRCS += draw_texture.c draw_utils.c draw_world.c draw_ascii.c
SRCS += draw_enemy.c draw_overlay.c draw_viewmodel.c
# AUDIO --------------------------------------- #
SRCS += audio.c
# MATH ---------------------------------------- #
SRCS += integer_scaling.c float_math.c vec2_math.c
# UTILS --------------------------------------- #
SRCS += rng.c cub_utils.c string.c io_basic.c cleanup.c
# PARSE --------------------------------------- #
SRCS += init.c map.c parse_textures.c read_xpm.c parse_sheet.c
# GAMEPLAY ------------------------------------ #
SRCS += movement.c player_state.c enemy_state.c game_state.c
# INPUT --------------------------------------- #
SRCS += keyboard.c mouse.c input.c

# Defaults ------------------------------------ #
RM := rm -f
BUILD_PATH = build
INC_PATH = includes
OBJ_PATH = $(BUILD_PATH)/obj/native
OBJS = $(addprefix $(OBJ_PATH)/, $(SRCS:.c=.o) $(UNIX_SRCS:.c=.o))
BIN = $(BUILD_PATH)/$(NAME)
WEB_OBJ_PATH = $(BUILD_PATH)/obj/web
WEB_BIN = $(BUILD_PATH)/index.js
WEB_OBJS = $(addprefix $(WEB_OBJ_PATH)/, $(SRCS:.c=.o) $(WEB_SRCS:.c=.o))

# Flags --------------------------------------- #
CC = clang
EMCC = emcc
CPPFLAGS = $(addprefix -I,$(INC_PATH))
CFLAGS = -Wall -Wextra -std=c11 -march=native -O1
LDLIBS = -lSDL2 -lSDL2_mixer -lSDL2main -lSDL2_image -lm
LDFLAGS =
WEB_LDFLAGS = -s USE_SDL=2 -s USE_SDL_IMAGE=2 \
				-s SDL2_IMAGE_FORMATS='["png", "bmp"]' \
				-s ALLOW_MEMORY_GROWTH=1 \
				-s EXPORTED_RUNTIME_METHODS='["requestPointerLock"]' \
				-s USE_SDL_MIXER=2 --preload-file assets
DEBUG = -g -Wpedantic -Wcast-qual -Wfloat-equal -Wswitch-default -Wsign-conversion
ASAN = -fsanitize=address,undefined,leak -fno-omit-frame-pointer
TSAN = -fsanitize=thread -fno-omit-frame-pointer
FAST = -march=native -flto -O3 -ffast-math -fstrict-aliasing

# Pattern Rules: Compilation ------------------ #
$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(WEB_OBJ_PATH)/%.o: %.c | $(WEB_OBJ_PATH)
	$(EMCC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Linking ------------------------------------- #
$(BIN): $(OBJS) | $(BUILD_PATH)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

$(WEB_BIN): $(WEB_OBJS) | $(BUILD_PATH)
	$(EMCC) $(WEB_OBJS) $(WEB_LDFLAGS) -o $@

# Directory ----------------------------------- #
$(OBJ_PATH):
	@mkdir -p $@
$(WEB_OBJ_PATH):
	@mkdir -p $@
$(BUILD_PATH):
	@mkdir -p $@

# Phonies ------------------------------------- #
all: $(BIN)
web: $(WEB_BIN)

compdb: | $(BUILD_PATH)
	$(RM) $(BUILD_PATH)/compile_commands.json
	bear --output $(BUILD_PATH)/compile_commands.json -- $(MAKE) clean debug

clean:
	$(RM) -r $(OBJ_PATH) $(WEB_OBJ_PATH)

fclean: clean
	$(RM) $(BIN) $(WEB_BIN) $(BUILD_PATH)/compile_commands.json

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

.PHONY: all clean fclean re debug asan tsan fast compdb web