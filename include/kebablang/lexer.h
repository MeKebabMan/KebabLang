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
    TOKEN_UNKNOWN, // Useless garbage
} TokenType;

// structures
typedef struct Token { // For tokens
    TokenType Token;
    char* Data;
    size_t Length;
    size_t Line;
} Token;

typedef struct {
    Token* Array;
    size_t Size; // Allocated Memory for the array
    size_t AllocatedTokens; // How many tokens are allocated
    size_t CursorOffset; // Cursor for the Token array.. (Mainly used by the parser)
} Lexer;

// Functions

// Reads the file using the provided file descripter (int fd) and dumps into a buffer
// Reads that buffer byte by byte tokenizing and inserting into the TokenArray (Token* Array)
// Returns TokenArray
// WARNING: check if TokenArray.Array == NULL because if it is NULL that means it failed to tokenize.. if not then it was successfully tokenized
Lexer Tokenize(int fd);

// Frees every token and array and memory allocated by the lexer
void FreeLexer(Lexer* Array);

static inline char* TokenTypeString(TokenType Type) {
    switch (Type) {
        case TOKEN_PARENTHESES1:
            return "TOKEN_PARENTHESES1";
        break;

        case TOKEN_PARENTHESES2:
            return "TOKEN_PARENTHESES2";
        break;

        case TOKEN_SQUARE_BRACKETS1:
            return "TOKEN_SQAURE_BRACKETS1";
        break;

        case TOKEN_SQUARE_BRACKETS2:
            return "TOKEN_SQAURE_BRACKETS2";
        break;

        case TOKEN_BRACKETS1:
            return "TOKEN_BRACKETS1";
        break;

        case TOKEN_BRACKETS2:
            return "TOKEN_BRACKETS2";
        break;

        case TOKEN_IDENTIFIER:
            return "TOKEN_INDENTIFER";
        break;

        case TOKEN_SINGLE_OPERATOR:
            return "TOKEN_SINGLE_OPERATOR";
        break;

        case TOKEN_MULTI_OPERATOR:
            return "TOKEN_MULTI_OPERATOR";
        break;

        case TOKEN_STRING:
            return "TOKEN_STRING";
        break;

        case TOKEN_NUMBER:
            return "TOKEN_NUMBER";
        break;

        case TOKEN_IF:
            return "TOKEN_IF";
        break;

        case TOKEN_ELSE:
            return "TOKEN_ELSE";
        break;

        case TOKEN_WHILE:
            return "TOKEN_WHILE";
        break;

        case TOKEN_TRUE:
            return "TOKEN_TRUE";
        break;

        case TOKEN_FALSE:
            return "TOKEN_FALSE";
        break;

        case TOKEN_COMMA:
            return "TOKEN_COMMA";
        break;

        case TOKEN_FUNCTION:
            return "TOKEN_FUNCTION";
        break;

        default:
            return "TOKEN_UNKNOWN";
        break;
    }
}

#ifdef __cplusplus
}
#endif

#endif