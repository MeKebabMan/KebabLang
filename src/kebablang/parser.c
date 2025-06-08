#include <kebablang/lexer.h>
#include <kebablang/parser.h>

// Marcos
#define FailedParse (Abstract_Syntax_Tree){(void*)NULL, 0, 0}
#define Args_FailedParse (AST_Args){(void*)NULL, 0, 0}
#define Block_FailedParse (AST_Block){(void*)NULL, 0, 0}
#define Error(Function, Message) fprintf(stderr, "Parser: (%s) %s\n", Function, Message);

// Helpers Declaration (static)

// Parser functions
static AST_Node* ParseTerm(Lexer* Tokens);
static AST_Node* ParseExpression(Lexer* Tokens);
static AST_Node* ParseStatement(Lexer* Tokens);
static AST_Args ParseArgs(Lexer* Tokens);
static AST_Block ParseBlock(Lexer* Tokens);

// Memory handling functions (helpers and static)
static AST_Node* ReserveNode(AST_Node* NodeArray, size_t* Size, size_t* AllocatedNodes);
static int Resize_NodeArray(AST_Node** NodeArray, size_t* Size);
static void FreeASTNode(AST_Node* Node);

// Token handling
static void NextToken(Lexer* Tokens);
static Token* PeekToken(Lexer* Tokens, size_t Offset);

// Helpers (static)

static AST_Node* ParseTerm(Lexer* Tokens) {

    if (Tokens == NULL || Tokens->Array == NULL) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return NULL;
    }

    return NULL;
}

static AST_Node* ParseExpression(Lexer* Tokens) {

    if (Tokens == NULL || Tokens->Array == NULL) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return NULL;
    }

    return NULL;
}

static AST_Node* ParseStatement(Lexer* Tokens) {

    if (Tokens == NULL || Tokens->Array == NULL) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return NULL;
    }

    
    return NULL;
}

static AST_Args ParseArgs(Lexer* Tokens) {
    if (Tokens == NULL || Tokens->Array == NULL) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return Args_FailedParse;
    }

    return Args_FailedParse;
}

static AST_Block ParseBlock(Lexer* Tokens) {
    if (Tokens == NULL || Tokens->Array == NULL) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return Block_FailedParse;
    }
    
    return Block_FailedParse;
}

// TOKEN HANDLING FUNCTIONS

static void NextToken(Lexer* Tokens) {
    // Guard Rail
    if (Tokens == NULL || Tokens->Array == NULL || Tokens->CursorOffset >= Tokens->AllocatedTokens) {
        fprintf(stderr, "Expected NON NULL args.. got null args\n");
        return;
    }
    
    Tokens->CursorOffset++; // Next
}

static Token* PeekToken(Lexer* Tokens, size_t Offset) {
    // Guard Rail
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

Abstract_Syntax_Tree Parse(Lexer* Tokens) {
    // Guard Rail
    if (Tokens == NULL || Tokens->Array == NULL) {
        return FailedParse;
    }

    return FailedParse;
}

void FreeParser(Abstract_Syntax_Tree* AST) {

}