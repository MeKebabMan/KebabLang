#include <assert.h>
#include <inttypes.h>
#include <kebablang/lexer.h>
#include <kebablang/parser.h>
#include <KebabLib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// Marcos
#define AST_FailedParse (Abstract_Syntax_Tree){(void*)NULL, 0, 0}


// Helpers Declaration (static)

static AST_Node* ParseTerm(TokenArray* Tokens);
// ParseTerm(1) should parse:
// Numbers (ASTNODE_NUMBER)
// Strings (ASTNODE_STRING)
// GROUPED EXPRESSIONS (REDIRCT TO ParseExpression(1))
// Function Calls (ASTNODE_FUNCTION_CALL)
// BOOLEANS (ASTNODE_BOOLEAN)

static AST_Node* ParseExpression(TokenArray* Tokens);
// ParseExpression(1) should parse:
// 1 + 3 * 4 (ASTNODE_MATH_OPERATION)
// Toggle == true 
// (x + y) * 3 (GROUPED EXPRESSION)

static AST_Node* ParseStatement(Abstract_Syntax_Tree* AST, TokenArray* Tokens);
// ParseStatement(2) should parse:
// MyVariable = "Hello, world!" (ASTNODE_VARIABLE_DEFINE)
// REDIRECTING TO ParseTerm(1) and ParseExpression(1) WHEN NEEDED
// MyFunction(MessageArg) {} (ASTNODE_FUNCTION_DEFINE)
// While <EXPRESSION> {} (ASTNODE_WHILE_LOOP)

static void NextToken(TokenArray* Tokens);
static Token* PeekToken(TokenArray* Tokens, size_t Offset);
static int ExtendMemory(void* Memory, size_t Size, size_t ExtendBy);

// Helpers (static)

static void NexToken(TokenArray* Tokens) {
    // Guard Rail
    if (Tokens == NULL || Tokens->Array == NULL || Tokens->CursorOffset >= Tokens->AllocatedTokens) {
        return;
    }
    
    Tokens->CursorOffset++; // Next
}

static Token* PeekToken(TokenArray* Tokens, size_t Offset) {
    if (Tokens == NULL || Tokens->Array == NULL || (Tokens->CursorOffset + Offset) > Tokens->AllocatedTokens) {
        return NULL;
    }

    return &Tokens->Array[Tokens->CursorOffset + Offset];
}

static int ExtendMemory(void* Memory, size_t Size, size_t ExtendBy) {

    // Guard Rail
    if (Memory == NULL) {
        return -1;
    }

    if (ExtendBy == 0) {
        return 0;
    }

    // Allocate new memory
    void* NewAllocation = malloc(Size + ExtendBy);
    if (NewAllocation == NULL) {
        return -1;
    }

    // Copy old memory
    memcpy(NewAllocation, Memory, Size);

    // Free old memory
    free(Memory);

    // Replace pointer
    Memory = NewAllocation;

    return 0;
}

// functions

Abstract_Syntax_Tree Parse(TokenArray *Tokens) {
    // Guard Rail
    if (Tokens == NULL || Tokens->Array == NULL) {
        return AST_FailedParse;
    }

    // Create AST
    Abstract_Syntax_Tree AST = {
        .Array = malloc((sizeof(AST_Node))),
        .Size = 1,
        .AllocateNodes = 0
    };

    if (Tokens->AllocatedTokens == 0) {
        return AST; // Nothing to parse
    }

    return AST;
}

void FreeParser(Abstract_Syntax_Tree* AST) {
    // Guard Rail
    if (AST == NULL || AST->Array == NULL) {
        return;
    }

    if (AST->AllocateNodes == 0) {
        return; // Skip early (Nothing to free)
    }

    for (size_t index = 0;  index > AST->AllocateNodes; index++) {
        AST_Node Node = AST->Array[index];

        // Free nodes

    }

}