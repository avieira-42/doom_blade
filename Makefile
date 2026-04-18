# Configuration ------------------------------- #
NAME = main
VPATH = sources sources/utils sources/input sources/parse \
		sources/gameplay sources/math sources/render sources/audio
SDL_LDLIBS = -lSDL2 -lSDL2_mixer -lSDL2main -lSDL2_image -lm
LDLIBS = $(SDL_LDLIBS)
# CORE ------------------------------------ #
UNIX_MAIN = main.c
WEB_MAIN = web_main.c
MAIN = $(UNIX_MAIN)
SRCS = $(MAIN) cleanup.c
# RENDER ------------------------------------ #
SRCS += raycast.c planecast.c
SRCS += draw_texture.c draw_utils.c draw_world.c draw_ascii.c
SRCS += draw_enemy.c draw_overlay.c draw_viewmodel.c
# AUDIO -------------------------------------#
SRCS += audio.c
# MATH ------------------------------------ #
SRCS += integer_scaling.c float_math.c vec2_math.c
# UTILS ------------------------------------ #
SRCS += rng.c cub_utils.c string.c io_basic.c
# PARSE ------------------------------------ #
SRCS += init.c map.c parse_textures.c read_xpm.c parse_sheet.c
# GAMEPLAY ------------------------------------ #
SRCS += movement.c player_state.c enemy_state.c game_state.c
# INPUT ------------------------------------ #
SRCS += keyboard.c mouse.c input.c

# Defaults ------------------------------------ #
RM := rm -f
BUILD_PATH = build
INC_PATH = includes
OBJ_PATH = $(BUILD_PATH)/obj
WEB = build/index.js
BIN = $(BUILD_PATH)/$(NAME)
OBJS = $(addprefix $(OBJ_PATH)/, $(SRCS:.c=.o))
OUTPUT = game.js

# Flags --------------------------------------- #
CC = clang
EMCC = emcc
COMPILER = $(CC)
CPPFLAGS = $(addprefix -I,$(INC_PATH))
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS =
DEBUG = -g -Wpedantic -Wcast-qual -Wfloat-equal -Wswitch-default -Wsign-conversion
ASAN = -fsanitize=address,undefined,leak -fno-omit-frame-pointer
TSAN = -fsanitize=thread -fno-omit-frame-pointer
FAST = -march=native -flto -O3 -ffast-math -fstrict-aliasing

# Pattern Rules: Compilation ------------------ #
$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
	$(COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Linking
$(BIN): $(OBJS) | $(BUILD_PATH)
	$(COMPILER) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) -o build/$(OUTPUT)

# Directory
$(OBJ_PATH):
	@mkdir -p $@
$(BUILD_PATH):
	@mkdir -p $@

# Phonies ------------------------------------- #
all: $(BIN)

compdb: | $(BUILD_PATH)
	$(RM) $(BUILD_PATH)/compile_commands.json
	bear --output $(BUILD_PATH)/compile_commands.json -- $(MAKE) clean asan

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

# Emcripten for WEB
web: MAIN = $(WEB_MAIN)
web: SDL_LDLIBS = 
web: LDLIBS = 
web: LDFLAGS = -s USE_SDL=2 -s USE_SDL_IMAGE=2 \
				-s SDL2_IMAGE_FORMATS='["png", "bmp"]' \
				-s ALLOW_MEMORY_GROWTH=1 \
				-s EXPORTED_RUNTIME_METHODS='["requestPointerLock"]' \
				-s USE_SDL_MIXER=2 --preload-file assets
web: SRCS := $(foreach f,$(SRCS),$(firstword $(wildcard $(addsuffix /$(f),$(VPATH)))))
web: SRCS := $(filter-out $(UNIX_MAIN) $(WEB_MAIN) sources/$(WEB_MAIN),$(SRCS))
web: SRCS := sources/$(WEB_MAIN) $(SRCS)
web:
	$(EMCC) $(CPPFLAGS) $(CFLAGS) $(SRCS) $(LDFLAGS) $(LDLIBS) -o $(WEB)

unix: COMPILER = $(CC)
unix: OUTPUT = doom_blade
unix: MAIN = $(UNIX_MAIN)
unix: $(BIN)

.PHONY: all clean fclean re debug asan tsan fast compdb web
