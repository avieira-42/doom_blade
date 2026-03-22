# Configuration ------------------------------- #
NAME = main
VPATH = sources sources/utils sources/parse sources/physics sources/math sources/events sources/render
SRCS = main.c cleanup.c init.c \
raycast.c draw_image.c render_image.c \
time.c io_basic.c read_rgb.c memory.c char_ascii.c transpose.c int_limits.c float_limits.c vec2.c \
map.c textures.c animation.c hud_init.c audio_init.c animation_update.c convert.c string.c\
movement.c \
bilinear_scaling.c integer_scaling.c \
mouse.c keyboard.c
LDLIBS = libraries/mlx/libmlx_Linux.a
ASM = $(OBJS:.o=.s)

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
CFLAGS = -Wall -Wextra -march=native -std=c11
SDLFLAGS = -lSDL2 -lSDL2_mixer -lSDL2main
LDFLAGS = -lXext -lX11 -lm -lz $(SDLFLAGS)
DEBUG = -g -Wpedantic -Wcast-qual -Wfloat-equal -Wswitch-default -Wsign-conversion
ASAN = -fsanitize=address,undefined,leak -fno-omit-frame-pointer
TSAN = -fsanitize=thread -fno-omit-frame-pointer
FAST = -march=native -O3 -ffast-math -fstrict-aliasing

# Pattern Rules: Compilation ------------------ #
$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Linking
$(BIN): $(OBJS) $(ASM) | $(BUILD_PATH)	
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# Assembly
$(OBJ_PATH)/%.s: $(OBJ_PATH)/%.o
	llvm-objdump -d --x86-asm-syntax=intel --no-show-raw-insn \
	--no-leading-addr --no-leading-headers --symbolize-operands $< > $@

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

ffast: CFLAGS += $(FAST) -Ofast
ffast: LDFLAGS += -flto
ffast: clean $(BIN)

.PHONY: all clean fclean re debug asan tsan fast ffast
