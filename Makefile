# Build file for KebabLang
# Variables
src := src
bin := bin

# Recommend to use C compilers as I am unsure if this will compile with C++ compilers..
cc := gcc

# Cflags

# posix fix (I remember posix not working when using c2x so uhh)
posix := -D_POSIX_C_SOURCE=200809L

header_files := -Iinclude
cflags_san := -fsanitize=address,undefined
cflags_development := -march=native -Wall -Wextra -g3 -O0 -fstack-protector-strong -fno-omit-frame-pointer $(cflags_san)
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

.PHONY: clean