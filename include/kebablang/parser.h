#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>
#ifdef __cplusplus
extern "c"
{
#endif

// Includes

#include <stddef.h>
#include <kebablang/lexer.h>
#include <stdbool.h>

// Enums

typedef enum {
    ASTNODE_NUMBER, // A number (Data type is intmax_t) (intmax_t)
    ASTNODE_STRING, // A string.. (char*)
    ASTNODE_VARIABLE, // A Variable name.. used for stuff like this print( <MyVariable> ) (char*)
    ASTNODE_BOOLEAN, // A boolean (true / false)
    ASTNODE_MATH_OPERATION, // Used for math.. <LEFT> <OPERATOR> <RIGHT> (3 * 2, etc) (struct MathOperation)
    ASTNODE_VARIABLE_DEFINE, // Used for creating variables.. MyVariable = "Hello, World!" (Struct VariableDefine)
    ASTNODE_FUNCTION_CALL, // Used for calling functions.. <Function>() (struct FunctionCall)
    ASTNODE_FUNCTION_DEFINE, // Used for creating functions.. <Function>() { <Body> } 
    ASTNODE_WHILE_LOOP, // Used for creating a loop.. while <condition> { <Body> }
} NodeTypes;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_TIMES,
    OP_POWER,
    OP_DIVIDE,
    OP_MODULO
} MathOperators;

// Structures

typedef struct AST_Args_Arena {
    struct ASTNode* Args;
    size_t  Memory;
    size_t AllocatedMemory;
} AST_Args;

typedef struct AST_Body_Arena {
        struct ASTNode** statements;
        size_t Memory;
        size_t AllocatedMemoy;
} AST_Body_Arena;

// Structures (AST)

typedef struct AST_WhileLoop {
    struct ASTNode* Condition;
    struct AST_Body_Arena* Body;
} AST_WhileLoop;

typedef struct AST_FunctionDefine {
    char* FunctionName;
    struct AST_Args_Arena* Args;
    struct AST_Body_Arena* Body;
} AST_FunctionDefine;

typedef struct AST_FunctionCall {
    char* FunctionName;
    struct AST_Args_Arena* Args;
} AST_FunctionCall;

typedef struct AST_VariableDefine {
    char* VariableName;
    struct ASTNode* VariableValue;
} AST_VariableDefine;

typedef struct AST_MathOperation {
    MathOperators Operator;
    struct ASTNode* Left;
    struct AStNode* Right;
} AST_MathOperation;


typedef struct ASTNode {
    NodeTypes ASTType;
    union {
        intmax_t Number;
        char* String;
        char* Variable;
        bool Boolean;
        AST_MathOperation Math;
        AST_VariableDefine VariableDefine;
        AST_FunctionCall FunctionCall;
        AST_FunctionDefine FunctionDefine;
        AST_WhileLoop WhileLoop;
    };
} ASTNode;

typedef struct AST_Arena {
    ASTNode* Array;
    size_t Memory;
    size_t AllocatedMemory;
} ASTArena;

// Functions

// Uses tokens to ensure correct grammar and syntax for KebabLang and creates a Abstract Syntax Tree for the interpreter
// Returns AbstractSyntaxTree on success
// Returns AbstractSyntaxTree with Node* NodeArray as NULL on failure
void Parse(TokenArray* Tokens);

// Frees the Abstract Syntax Tree
void FreeParser(void* AST);

#ifdef __cplusplus
}
#endif

#endif