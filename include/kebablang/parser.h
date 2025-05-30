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
    ASTNODE_BLOCK, // Used for { <BODY> } 
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

typedef struct AST_Args {
    struct AST_Node* Args;
    size_t Size;
    size_t AllocatedNodes;
} AST_Args;


// Structures (AST)

typedef struct AST_Block {
        struct AST_Node** statements;
        size_t Size;
        size_t AllocatedNodes;
} AST_Block;

typedef struct AST_WhileLoop {
    struct AST_Node* Condition;
    struct AST_Node* Body;
} AST_WhileLoop;

typedef struct AST_FunctionDefine {
    char* FunctionName;
    struct AST_Args* Args;
    struct AST_Node* Body;
} AST_FunctionDefine;

typedef struct AST_FunctionCall {
    char* FunctionName;
    struct AST_Args* Args;
} AST_FunctionCall;

typedef struct AST_VariableDefine {
    char* VariableName;
    struct AST_Node* VariableValue;
} AST_VariableDefine;

typedef struct AST_MathOperation {
    MathOperators Operator;
    struct AST_Node* Left;
    struct AST_Node* Right;
} AST_MathOperation;


typedef struct AST_Node {
    NodeTypes ASTType;
    union {
        intmax_t Number; // ASTNODE_NUMBER
        char* String; // ASTNODE_STRING
        char* Variable; // ASTNODE_VARIABLE
        bool Boolean; // ASTNODE_BOOLEAN
        AST_MathOperation Math; // ASTNODE_MATH_OPERATION
        AST_VariableDefine VariableDefine; // ASTNODE_VARIABLE_DEFINE
        AST_Block Block; // ASTNODE_BLOCK
        AST_FunctionCall FunctionCall; // ASTNODE_FUNCTION_CALL
        AST_FunctionDefine FunctionDefine; // ASTNODE_FUNCTION_DEFINE
        AST_WhileLoop WhileLoop; // ASTNODE_WHILE_LOOP
    };
} AST_Node;

typedef struct Abstract_Syntax_Tree{
    struct AST_Node* Array;
    size_t Size;
    size_t AllocateNodes;
} Abstract_Syntax_Tree;

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