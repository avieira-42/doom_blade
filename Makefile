# Configuration ------------------------------- #
NAME = main
VPATH = sources sources/utils sources/parse sources/math sources/core sources/events soruces/physics sources/render
SRCS = main.c \
io_basic.c read_rgb.c memory.c char_ascii.c transpose.c \
map.c textures.c \
bilinear_scaling.c integer_scaling.c \
input.c 
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
LDFLAGS = -lXext -lX11 -lm -lz
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
	--no-leading-addr --no-leading-headers --symbolize-operands \
	--pretty-pgo-analysis-map $< > $@

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
