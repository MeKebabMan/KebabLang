#include <KebabLib.h>
#include <fcntl.h>
#include <kebablang/lexer.h>
#include <stdio.h>

int main() {

    int fd = open("main.kebab", O_RDONLY);
    if (fd == -1) {
        fputs("Failed to open file\n\n", stderr);
        return -1;
    }

    TokenArray Tokens = Tokenize(fd);
    if (Tokens.Array == NULL) {
        fputs("Failed to Tokenize\n", stderr);
        return -1;
    }

    for (size_t index = 0; index < Tokens.AllocatedTokens; index++) {
        printf("Token: %s -> %d\n", Tokens.Array[index].Data, Tokens.Array[index].Token);
    }

    // Free the lexer
    FreeLexer(&Tokens);

    return 0;    
}