#include "kebablang/parser.h"
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

    Lexer Tokens = Tokenize(fd);
    if (Tokens.Array == NULL) {
        fputs("Failed to Tokenize\n", stderr);
        return -1;
    }

    // Tokens
    for (size_t index = 0; index < Tokens.AllocatedTokens; index++) {
        printf("%s -> %s\n", Tokens.Array[index].Data, TokenTypeString(Tokens.Array[index].Token));
    }

    // Lexer memory
    printf("\nLexer:\nSize: %zu\nAllocatedTokens: %zu\n\n", Tokens.Size, Tokens.AllocatedTokens);

    // Parse
    Abstract_Syntax_Tree AST = Parse(&Tokens);
    if (AST.Array == NULL) {
        fputs("Failed to parse\n", stderr);
        FreeLexer(&Tokens);
        return -1;
    }

    FreeParser(&AST);

    // Free the lexer
    FreeLexer(&Tokens);

    return 0;    
}