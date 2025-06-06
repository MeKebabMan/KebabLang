#ifndef _LEXER_H_
#define _LEXER_H_

#ifdef __cplusplus
extern "C"
{
#endif 

// Includes

#include <stdio.h>

// Enums

typedef enum {
    TOKEN_PARENTHESES1, // (
    TOKEN_PARENTHESES2, // )
    TOKEN_SQUARE_BRACKETS1, // [
    TOKEN_SQUARE_BRACKETS2, // ]
    TOKEN_BRACKETS1, // {
    TOKEN_BRACKETS2, // }
    TOKEN_IDENTIFIER, // MyVariable
    TOKEN_SINGLE_OPERATOR, // +
    TOKEN_MULTI_OPERATOR, // ==
    TOKEN_STRING, // "Hello, world!"
    TOKEN_NUMBER, // 123
    TOKEN_IF, // if
    TOKEN_ELSE, // else
    TOKEN_WHILE, // while
    TOKEN_FUNCTION, // function
    TOKEN_FALSE, // false (BOOLEAN)
    TOKEN_TRUE, //  true (BOOLEAN)
    TOKEN_COMMA, // ,
} TokenType;

// structures
typedef struct Token { // For tokens
    TokenType Token;
    char* Data;
    size_t Length;
} Token;

typedef struct {
    Token* Array;
    size_t Size; // Allocated Memory for the array
    size_t AllocatedTokens; // How many tokens are allocated
    size_t CursorOffset; // Cursor for the Token array.. (Mainly used by the parser)
} TokenArray;

// Functions

// Reads the file using the provided file descripter (int fd) and dumps into a buffer
// Reads that buffer byte by byte tokenizing and inserting into the TokenArray (Token* Array)
// Returns TokenArray
// WARNING: check if TokenArray.Array == NULL because if it is NULL that means it failed to tokenize.. if not then it was successfully tokenized
TokenArray Tokenize(int fd);

// Frees every token and array and memory allocated by the lexer
void FreeLexer(TokenArray* Array);

#ifdef __cplusplus
}
#endif

#endif