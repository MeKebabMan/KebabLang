#include <assert.h>
#include <complex.h>
#include <inttypes.h>
#include <kebablang/lexer.h>
#include <kebablang/parser.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>

// Marcos
#define FailedParse (Abstract_Syntax_Tree){(void*)NULL, 0, 0}
#define Args_FailedParse (AST_Args){(void*)NULL, 0, 0}
#define Block_FailedParse (AST_Block){(void*)NULL, 0, 0}

#define NullFree(Memory) free((Memory)); (Memory) = NULL;

#define Error(Message, ...) fprintf(stderr, "[ERROR] Parser: (%s) " Message "\n", __func__, ##__VA_ARGS__); 
#define SyntaxErr(Message, ...) fprintf(stderr, "Syntax Error: " Message "\n", ##__VA_ARGS__);

#ifndef NDEBUG
#define Log(Message, ...) fprintf(stdout, "\n[LOG] Parser: (%s) " Message "\n", __func__, ##__VA_ARGS__);
#else
#define Log(Message, ...) ((void)0)
#endif

// Helpers Declaration (static)

// Parser functions
static AST_Node* ParseTerm(Lexer* Tokens);
static AST_Node* ParseExpression(Lexer* Tokens);
static AST_Node* ParseStatement(Lexer* Tokens);
static AST_Args ParseArgs(Lexer* Tokens);
static AST_Block ParseBlock(Lexer* Tokens);

// Memory handling functions (helpers and static)
static AST_Node* ReserveNode(AST_Node** NodeArray, size_t* Size, size_t* AllocatedNodes, int ExtendBy);
static int Resize_NodeArray(AST_Node** NodeArray, size_t OldMemory, size_t NewMemory);
static void FreeASTNodes(AST_Node* NodeArray, size_t AllocatedNodes);
static void FreeASTFields(AST_Node* Node);
static void FreeASTNode(AST_Node** Node);

// Token handling
static void NextToken(Lexer* Tokens);
static Token* PeekToken(Lexer* Tokens, size_t Offset);

// Helpers (static)

static AST_Node* ParseTerm(Lexer* Tokens) {
    if (Tokens == NULL || Tokens->Array == NULL) {
        Error("Expected NON NULL args");
        return NULL;
    }

    // Get Current Token
    Token* CurrentTok = PeekToken(Tokens, 0);
    if (CurrentTok == NULL) {
        Error("Expected Token");
        return NULL;
    }

    AST_Node* Node = calloc(1, sizeof(AST_Node));
    if (Node == NULL) {
        Error("Failed to allocate memory");
        return NULL;
    }

    // Parse
    switch (CurrentTok->Token) {

        case TOKEN_NUMBER:

            Node->ASTType = ASTNODE_NUMBER;
            Node->Number = strtoimax(CurrentTok->Data, NULL, 10);

        break;

        case TOKEN_STRING:

            Node->ASTType = ASTNODE_STRING;
            Node->String = strndup(CurrentTok->Data, CurrentTok->Length);

        break;

        case TOKEN_TRUE || TOKEN_FALSE:

            Node->ASTType = ASTNODE_BOOLEAN;
            Node->Boolean = CurrentTok->Token == TOKEN_TRUE;

        break;

        default:
        Error("Incorrect use! Attempted to parse %s which is a %s\n", CurrentTok->Data, TokenTypeString(CurrentTok->Token));

        goto Failed;
        break;

    }

    return Node;
    
    Failed:
        FreeASTNode(&Node);
        return NULL;
    
}

static AST_Node* ParseExpression(Lexer* Tokens) {
    if (Tokens == NULL || Tokens->Array == NULL) {
        Error("Expected NON NULL args");
        return NULL;
    }

    return NULL;
}

static AST_Node* ParseStatement(Lexer* Tokens) {
    if (Tokens == NULL || Tokens->Array == NULL) {
        Error("Expected NON NULL args");
        return NULL;
    }

    // Get Current Token
    Token* CurrentTok = PeekToken(Tokens, 0);
    if (CurrentTok == NULL) {
        Error("Expected Token");
        return NULL;
    }

    AST_Node* Node = calloc(1, sizeof(AST_Node));
    if (Node == NULL) {
        Error("Failed to allocate memory");
        return NULL;
    }

    // Parse
    switch (CurrentTok->Token) {

        case TOKEN_IDENTIFIER: {

            // Look ahead
            Token* AheadTok = PeekToken(Tokens, 1);
            if (AheadTok == NULL) {
                SyntaxErr("Expected token after %s\n", CurrentTok->Data);

                goto Failed;
            }

            if (AheadTok->Token == TOKEN_SINGLE_OPERATOR && AheadTok->Data[0] == '=') {
                
                Log("Parsing variable declaration");
                Node->ASTType = ASTNODE_VARIABLE_DEFINE;
                Node->VariableDefine.VariableName = strndup(CurrentTok->Data, CurrentTok->Length);

                NextToken(Tokens); // Current Token
                NextToken(Tokens); // Ahead Token

                AST_Node* Value = ParseTerm(Tokens);
                if (Value == NULL) {
                    Error("Failed to parse the variable value");

                    goto Failed;
                }

                Node->VariableDefine.VariableValue = Value;

            }

        break;
        }


        default:
        Error("Incorrect use! Attempted to parse %s which is a %s\n", CurrentTok->Data, TokenTypeString(CurrentTok->Token));

        goto Failed;
        break;
    }

    return Node;

    Failed:
        FreeASTNode(&Node);
        return NULL;
}

static AST_Args ParseArgs(Lexer* Tokens) {
    if (Tokens == NULL || Tokens->Array == NULL) {
        Error("Expected NON NULL args");
        return Args_FailedParse;
    }

    return Args_FailedParse;
}

static AST_Block ParseBlock(Lexer* Tokens) {
    if (Tokens == NULL || Tokens->Array == NULL) {
        Error("Expected NON NULL args");
        return Block_FailedParse;
    }
    
    return Block_FailedParse;
}

// TOKEN HANDLING FUNCTIONS

static void NextToken(Lexer* Tokens) {
    // Guard Rail
    if (Tokens == NULL || Tokens->Array == NULL) {
        Error("Guard Rail");
        return;
    }

    if (Tokens->CursorOffset >= Tokens->AllocatedTokens) {
        Log("Limit HIT! Not allowing to advance");
        return;
    }

    Log("Advance\nOld Offset: %zu\nNew Offset: %zu\n", Tokens->CursorOffset, Tokens->CursorOffset + 1);

    Tokens->CursorOffset++; // Next
}

static Token* PeekToken(Lexer* Tokens, size_t Offset) {
    // Guard Rail
    if (Tokens == NULL || Tokens->Array == NULL) {
        Error("Guard Rail");
        return NULL;
    }

    if ((Tokens->CursorOffset + Offset) >= Tokens->AllocatedTokens) {
        Log("No tokens");
        return NULL;
    }

    return &Tokens->Array[Tokens->CursorOffset + Offset];
}

// MEMORY HANDLING FUNCTIONS

static AST_Node* ReserveNode(AST_Node** NodeArray, size_t* Size, size_t* AllocatedNodes, int ExtendBy) {
    if (NodeArray == NULL || Size == NULL || AllocatedNodes == NULL) {
        Error("Expected NON NULL args")
        return NULL;
    }

    if (*AllocatedNodes == *Size) {
        // Resize
        size_t NewSize = *AllocatedNodes + ExtendBy;
        int result = Resize_NodeArray(NodeArray, *Size, NewSize);
        if (result == -1) {
            Error("Failed to extend memory when needed..\nSize: %zu\nAllocatedNodes: %zu\n", *Size, *AllocatedNodes);
            return NULL;
        }

        *Size = NewSize; // Keep track of size
    }

    AST_Node* Node = &(*NodeArray)[*AllocatedNodes];
    
    *AllocatedNodes = *AllocatedNodes + 1;

    return Node;
}

static void FreeASTNodes(AST_Node* NodeArray, size_t AllocatedNodes) {
    if (NodeArray == NULL) {
        Error("Expected NON NULL args");
        return;
    }

    if (AllocatedNodes == 0) {
        return; // Skip
    }

    for (size_t index = 0; index < AllocatedNodes; index++) {
        AST_Node* Node = &NodeArray[index];
        FreeASTFields(Node);
    }
}

static int Resize_NodeArray(AST_Node** Memory, size_t OldMemory, size_t NewMemory) {
    // Guard Rail
    if (Memory == NULL || *Memory == NULL) {
        Error("Expected NON NULL args");
        return -1;
    }

    if (NewMemory == 0) {
        
        // Free memory
        NullFree(*Memory);

        return 0;
    }

    // Allocate new memory
    AST_Node* NewNodeArray = calloc((size_t)NewMemory, sizeof(AST_Node)); // Prefer calloc for safety
    if (NewNodeArray == NULL) {
        Error("Failed to allocate memory\nAttempted Memory Allocation: %zu (%zu)\n", NewMemory, NewMemory * sizeof(AST_Node));
        return -1;
    }

    // Log
    Log("Reallocated %zu (%zu) bytes", NewMemory, NewMemory * sizeof(AST_Node));

    // Copy memory
    size_t CopyMemory = (size_t)OldMemory; // Safe amount of bytes to copy
    if (CopyMemory > NewMemory) {
        CopyMemory = (size_t)NewMemory;
    }

    memcpy(NewNodeArray, *Memory, CopyMemory * sizeof(AST_Node));

    // Free old memory;
    free(*Memory);

    // Replace variable pointer
    *Memory = NewNodeArray;

    return 0;
}

static void FreeASTNode(AST_Node** Node) {

    if (Node == NULL || *Node == NULL) {
        return;
    }

    FreeASTFields(*Node);

    NullFree(*Node);
}

static void FreeASTFields(AST_Node* Node) {

    if (Node == NULL) {
        return;
    }

    switch (Node->ASTType) {

        case ASTNODE_STRING:
            NullFree(Node->String);
        break;

        case ASTNODE_VARIABLE:
            NullFree(Node->Variable);
        break;

        case ASTNODE_VARIABLE_DEFINE:
            NullFree(Node->VariableDefine.VariableName);
            
            FreeASTNode(&Node->VariableDefine.VariableValue);

        break;

        case ASTNODE_BINARY_OPERATION:
            FreeASTNode(&Node->BinaryOperation.Left);

            FreeASTNode(&Node->BinaryOperation.Right);
        break;

        case ASTNODE_IF:
            FreeASTNode(&Node->IfStatement.Condition);

            FreeASTNodes(Node->IfStatement.Block.statements, Node->IfStatement.Block.AllocatedNodes);
            Node->IfStatement.Block.AllocatedNodes = 0;

            NullFree(Node->IfStatement.Block.statements);
            Node->IfStatement.Block.Size = 0;

            FreeASTNodes(Node->IfStatement.Else_block.statements, Node->IfStatement.Else_block.AllocatedNodes);
            Node->IfStatement.Block.AllocatedNodes = 0;

            NullFree(Node->IfStatement.Else_block.statements);
            Node->IfStatement.Else_block.Size = 0;

        break;

        case ASTNODE_WHILE_LOOP:
            FreeASTNode(&Node->WhileLoop.Condition);

            FreeASTNodes(Node->WhileLoop.Block.statements, Node->WhileLoop.Block.AllocatedNodes);
            Node->WhileLoop.Block.AllocatedNodes = 0;

            NullFree(Node->WhileLoop.Block.statements);
            Node->WhileLoop.Block.Size = 0;
        break;

        case ASTNODE_FUNCTION_CALL:
            NullFree(Node->FunctionCall.FunctionName);

            FreeASTNodes(Node->FunctionCall.Args.Args, Node->FunctionCall.Args.AllocatedNodes);
            Node->FunctionCall.Args.AllocatedNodes = 0;

            NullFree(Node->FunctionCall.Args.Args);
            Node->FunctionCall.Args.Size = 0;

        break;

        case ASTNODE_FUNCTION_DEFINE:
            NullFree(Node->FunctionDefine.FunctionName);

            FreeASTNodes(Node->FunctionDefine.Args.Args, Node->FunctionDefine.Args.AllocatedNodes);
            Node->FunctionDefine.Args.AllocatedNodes = 0;

            NullFree(Node->FunctionDefine.Args.Args);
            Node->FunctionDefine.Args.Size = 0;

            FreeASTNodes(Node->FunctionDefine.Block.statements, Node->FunctionDefine.Block.AllocatedNodes);
            Node->FunctionDefine.Block.AllocatedNodes = 0;

            NullFree(Node->FunctionDefine.Block.statements);
            Node->FunctionDefine.Block.Size = 0;

        break;

        default:
        // Skip
        break;

    }

}

// Main functions

Abstract_Syntax_Tree Parse(Lexer* Tokens) {
    // Guard Rail
    if (Tokens == NULL || Tokens->Array == NULL) {
        Error("Expected NON NULL args");
        return FailedParse;
    }

    // Create AST
    Abstract_Syntax_Tree AST = {
        .Array = calloc(4, sizeof(AST_Node)),
        .Size = 4,
        .AllocatedNodes = 0
    };

    if (AST.Array == NULL) {
        Error("Failed to allocate memory");
        return FailedParse;
    }

    // Parse
    Token* CurrentTok = PeekToken(Tokens, 0);
    while (CurrentTok != NULL) {
        
        AST_Node* Statement = ParseStatement(Tokens);
        if (Statement == NULL) {
            Error("Failed to parse statement");
            goto Failed;
        }

        // Insert
        AST_Node* ReservedNode = ReserveNode(&AST.Array, &AST.Size, &AST.AllocatedNodes, 4);
        if (ReservedNode == NULL) {
            Error("Failed to reserve node");
            goto Failed;
        }

        *ReservedNode = *Statement; // Shallow copy
        NullFree(Statement); // Free the struct

        NextToken(Tokens);

        CurrentTok = PeekToken(Tokens, 0);

    }

    goto Success;

    // Success (LABEL)
    Success:

        if (AST.Size > AST.AllocatedNodes) {
            size_t SafeCleanSize = AST.AllocatedNodes;
            if (SafeCleanSize == 0) {
                SafeCleanSize = 1;
            }

            int err = Resize_NodeArray(&AST.Array, AST.Size, SafeCleanSize);
            if (err == -1) {
                Error("Failed to clean AST array");

                goto Failed;
            }

            AST.Size = SafeCleanSize;
        }

        return AST;


    // Failed (LABEL)
    Failed:
        FreeParser(&AST);
        return FailedParse;

}

void FreeParser(Abstract_Syntax_Tree* AST) {
    if (AST == NULL || AST->Array == NULL) {
        Error("Expected NON NULL args");
        return;
    }

    FreeASTNodes(AST->Array, AST->AllocatedNodes);
    AST->AllocatedNodes = 0;

    NullFree(AST->Array);
    AST->Size = 0;
}