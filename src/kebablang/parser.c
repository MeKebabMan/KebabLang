#include <assert.h>
#include <inttypes.h>
#include <kebablang/lexer.h>
#include <kebablang/parser.h>
#include <KebabLib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// Marcos
#define AST_FailedParse (Abstract_Syntax_Tree){(void*)NULL, 0, 0}
#define ASTArgs_FailedParse (AST_Args){(void*)NULL, 0, 0}
#define ASTBlock_FailedParse (AST_Block){(void*)NULL, 0, 0}

// Helpers Declaration (static)

// Parser functions
static AST_Node* ParseTerm(TokenArray* Tokens);
static AST_Node* ParseExpression(TokenArray* Tokens);
static AST_Node* ParseStatement(TokenArray* Tokens);
static AST_Args ParseArgs(TokenArray* Tokens);
static AST_Block ParseBlock(TokenArray* Tokens);

// Memory handling functions (helpers and static)
static AST_Node* ReserveNode(AST_Node* NodeArray, size_t* Size, size_t* AllocatedNodes);
static int Resize_NodeArray(AST_Node** NodeArray, size_t* Size);
static void FreeASTNode(AST_Node* Node);

// Token handling
static void NextToken(TokenArray* Tokens);
static Token* PeekToken(TokenArray* Tokens, size_t Offset);

// Helpers (static)

static AST_Node* ParseTerm(TokenArray* Tokens) {

    return NULL;
}

static AST_Node* ParseExpression(TokenArray* Tokens) {

    if (Tokens == NULL || Tokens->Array == NULL) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return NULL;
    }

    return NULL;
}

static AST_Node* ParseStatement(TokenArray* Tokens) {

    if (Tokens == NULL || Tokens->Array == NULL) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return NULL;
    }

    
    return NULL;
}

static AST_Args ParseArgs(TokenArray* Tokens) {

    return ASTArgs_FailedParse;
}

static AST_Block ParseBlock(TokenArray* Tokens) {
    
    return ASTBlock_FailedParse;
}

// TOKEN HANDLING FUNCTIONS

static void NextToken(TokenArray* Tokens) {
    // Guard Rail
    if (Tokens == NULL || Tokens->Array == NULL || Tokens->CursorOffset >= Tokens->AllocatedTokens) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return;
    }
    
    Tokens->CursorOffset++; // Next
}

static Token* PeekToken(TokenArray* Tokens, size_t Offset) {
    if (Tokens == NULL || Tokens->Array == NULL || (Tokens->CursorOffset + Offset) > Tokens->AllocatedTokens) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return NULL;
    }

    return &Tokens->Array[Tokens->CursorOffset + Offset];
}

// MEMORY HANDLING FUNCTIONS

static AST_Node* ReserveNode(AST_Node* NodeArray, size_t* Size, size_t* AllocatedNodes) {
    return NULL;
}

static void FreeASTNodes(AST_Node** NodeArray, size_t AllocatedNodes) {

}

static int Resize_NodeArray(AST_Node** Memory, size_t* Size) {

    return 0;
}

static void FreeASTNode(AST_Node* Node) {

}

// Main functions

Abstract_Syntax_Tree Parse(TokenArray *Tokens) {

    return AST_FailedParse;
}

void FreeParser(Abstract_Syntax_Tree* AST) {

}