#include <assert.h>
#include <inttypes.h>
#include <kebablang/lexer.h>
#include <kebablang/parser.h>
#include <KebabLib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// Marcos
#define AST_FailedParse (Abstract_Syntax_Tree){(void*)NULL, 0, 0}

// Parser functions declaration (static)

static Token* ViewCurrentToken(TokenArray Tokens); // returns current token (NULL ON FAILURE)
static Token* NextToken(TokenArray* Tokens); // Moves cursor offset by 1 and returns token (NULL ON FAILURE)

// Helper functions declaration (static)

static AST_Node* Create_AST_Node(Abstract_Syntax_Tree* AST); // WARNING! DO NOT USE THIS FOR STRUCT FIELDS!!
static int Extend_Array(void* Array, size_t Memory, size_t ExtendBy, size_t Size);

// Parser functions (static)

static Token* ViewCurrentToken(TokenArray Tokens) {

    assert(Tokens.CursorOffset > Tokens.AllocatedTokens);

    if (Tokens.Array == NULL || Tokens.CursorOffset > Tokens.AllocatedTokens) { // (Guard Rail)
        return NULL;
    }

    return &Tokens.Array[Tokens.CursorOffset];
}

static Token* NextToken(TokenArray* Tokens) {
    
    assert(Tokens->CursorOffset > Tokens->AllocatedTokens);
    
    if (Tokens == NULL || Tokens->Array == NULL || Tokens->CursorOffset >= Tokens->AllocatedTokens) { // (Guard Rail)
        return NULL;
    }

    Tokens->CursorOffset++; // Increment cursor
    return &Tokens->Array[Tokens->CursorOffset];
    
}

// Helper functions (static)

static AST_Node* Create_AST_Node(Abstract_Syntax_Tree* AST) {

    if (AST == NULL || AST->Array == NULL) {
        return NULL;
    }

    assert(AST->AllocateNodes > AST->Size);

    if (AST->AllocateNodes == AST->Size) {
        // Extend
        size_t ExtendBy = 4;
        if (Extend_Array(AST->Array, AST->Size, ExtendBy, sizeof(AST_Node)) == -1) {
            return NULL;
        } 
        AST->Size += ExtendBy;
    }

    // Create node
    AST_Node* Node = &AST->Array[AST->AllocateNodes];

    *Node = (AST_Node){0}; // Default

    AST->AllocateNodes++; // Increment count

    return Node;
}

static int Extend_Array(void* Array, size_t Memory, size_t ExtendBy, size_t Size) {
    if (Array == NULL) { // (Guard Rail)
        return -1;
    }

    if (ExtendBy == 0) {
        return 0;
    }

    // Allocate new memory
    void* NewAllocation = malloc((Memory + ExtendBy) * Size);
    if (NewAllocation == NULL) {
        return -1;
    }

    // Copy old memory
    if (Memory != 0) {
        memcpy(NewAllocation, Array, Memory);
    }

    // Free old memory
    free(Array);

    // Replace pointer
    Array = NewAllocation;

    return 0;

}