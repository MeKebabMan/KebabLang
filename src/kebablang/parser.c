#include <kebablang/lexer.h>
#include <kebablang/parser.h>
#include <KebabLib.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Marcos
#define FailedParse (AbstractSyntaxTree){(void*)NULL, 0}

// Helper functions (static)

static int CreateASTNode(AbstractSyntaxTree* AST, ASTNode** Output); // Declaration
static int ExtendASTArray(AbstractSyntaxTree* AST, size_t ExtendSize); // Declaration 
static Token* ViewCurrentToken(TokenArray* Array); // Declaration
static Token* NextToken(TokenArray* Array); // Declaration
static ASTNode* ParseStatement(AbstractSyntaxTree* AST, TokenArray* Array); // Declaration
static ASTNode* ParseTerm(AbstractSyntaxTree* AST, TokenArray* Array); // Declaration
static ASTNode* ParseExpression(AbstractSyntaxTree* AST, TokenArray* Array); // Declaration

static ASTNode* ParseTerm(AbstractSyntaxTree* AST, TokenArray* Array) {
    if (AST == NULL || AST->NodeArray == NULL || AST == NULL || AST->NodeArray == NULL) { // (Guard Rail)
        return NULL;
    }


    return NULL;
}

static ASTNode* ParseExpression(AbstractSyntaxTree* AST, TokenArray* Array) {
    if (Array == NULL || Array->Array == NULL || AST == NULL || AST->NodeArray == NULL) { // (Guard Rail)
        return NULL;
    }

    return NULL;
}

static ASTNode* ParseStatement(AbstractSyntaxTree* AST, TokenArray* Array) {
    if (Array == NULL || Array->Array == NULL || AST == NULL || AST->NodeArray == NULL) { // (Guard Rail)
        return NULL;
    }



    return NULL;
}

static Token* ViewCurrentToken(TokenArray* Array) {
    // Guard Rail
    if (Array == NULL || Array->Array == NULL || Array->CursorOffset > Array->AllocatedTokens) {
        return NULL;
    }

    // Return Token
    return &Array->Array[Array->CursorOffset];
}

static Token* NextToken(TokenArray* Array) {
    // Guard Rail
    if (Array == NULL || Array->Array == NULL || Array->CursorOffset >= Array->AllocatedTokens) {
        return NULL;
    }

    // Increment
    Array->CursorOffset++;

    // Return Token
    return &Array->Array[Array->CursorOffset++];
}

static int CreateASTNode(AbstractSyntaxTree* AST, ASTNode** Output) {
    if (AST == NULL || AST->NodeArray == NULL) { // (Guard Rail)
        return -1;
    }

    // Ensure we have space for inserting a node
    if (AST->AllocatedNodes >= AST->NodeMemory) {
        // Extend Array
        if (ExtendASTArray(AST, 4) == -1) {
            return -1;
        }
    }

    // Create the node
    ASTNode* NewNode = &AST->NodeArray[AST->AllocatedNodes];

    // Set the default values
    *NewNode = (ASTNode){0};

    // Output
    if (Output != NULL) {
        *Output = NewNode;
    }

    // Keep track
    AST->AllocatedNodes++;

    return 0;
}

static int ExtendASTArray(AbstractSyntaxTree* AST, size_t ExtendSize) {
    if (AST == NULL || AST->NodeArray == NULL) { // (Guard Rail)
        return -1;
    }

    // Allocate new memory
    ASTNode* NewAllocation = calloc(AST->NodeMemory + ExtendSize, sizeof(ASTNode));
    if (NewAllocation == NULL) {
        return -1;
    }

    // Copy old memory
    memcpy(NewAllocation, AST->NodeArray, AST->NodeMemory * sizeof(ASTNode));

    // Free old memory allocation
    free(AST->NodeArray);

    // Replace variable pointer
    AST->NodeArray = NewAllocation;

    // Keep track of allocated memory
    AST->NodeMemory = AST->NodeMemory + ExtendSize;
    
    return 0;
}

// Functions

AbstractSyntaxTree Parse(TokenArray* Tokens); // Declaration
void FreeParser(AbstractSyntaxTree *AST); // Declaration

AbstractSyntaxTree Parse(TokenArray* Tokens) {

    if (Tokens == NULL || Tokens->Array == NULL) { // (Guard Rail)
        return FailedParse;
    }

    // Create the abstract syntax tree
    AbstractSyntaxTree AST = {
        .NodeArray = calloc(1, sizeof(ASTNode)),
        .NodeMemory = 1,
        .AllocatedNodes = 0
    };

    if (AST.NodeArray == NULL) {
        return FailedParse;
    }

    // Ensure grammar and syntax and create AST Nodes
    while (ViewCurrentToken(Tokens) != NULL) {

        // Parse statement

        // Insert statement
    }

    return AST;
}

void FreeParser(AbstractSyntaxTree *AST) {
    if (AST == NULL || AST->NodeArray == NULL) { // (Guard Rail)
        return;
    }

}