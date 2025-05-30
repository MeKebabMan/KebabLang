# Build file for KebabLang
# Variables
src := src
bin := bin
cc := gcc

# Cflags
header_files := -Iinclude
cflags_development := -march=native -fsanitize=address,undefined -Wall -Wextra -g3 -O0 -fstack-protector-strong -fno-omit-frame-pointer
cflags_production := -march=native -o3 -flto -fno-plt -s --as-needed -DNDEBUG

# Execute able
exe := $(bin)/KebabLang.out

# Files
srcs = $(shell find $(src) -type f -name "*.c")
objs = $(patsubst $(src)/%.c, $(bin)/%.obj, $(srcs))

# Rules

# Default Target
all: $(exe)

# Compiling Object files
$(bin)/%.obj: $(src)/%.c
	@mkdir -p $(dir $@)
	$(cc) -c $< -o $@ $(cflags_development) $(header_files)

# Linking
$(exe): $(objs)
	$(cc) $^ -o $@ $(cflags_development) $(header_files)

# Phony Rules

clean:
	rm -f $(exe) $(objs)

valgrind:
	valgrind $(exe)

.PHONY: clean valgrind