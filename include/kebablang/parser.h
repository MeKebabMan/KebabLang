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
    ASTNODE_BINARY_OPERATION, // Used for math and expressions.. <LEFT> <OPERATOR> <RIGHT> (3 * 2, etc) (struct AST_BinaryOperation)
    ASTNODE_VARIABLE_DEFINE, // Used for creating variables.. MyVariable = "Hello, World!" (Struct VariableDefine)
    ASTNODE_FUNCTION_CALL, // Used for calling functions.. <Function>() (struct FunctionCall)
    ASTNODE_FUNCTION_DEFINE, // Used for creating functions.. <Function>() { <Body> }
    ASTNODE_WHILE_LOOP, // Used for creating a loop.. while <condition> { <Body> }
    ASTNODE_IF, // If statement.. if <condition> { <Body> }
} NodeTypes;

typedef enum {

    // Comparison
    OP_COMPARE, // ==
    OP_BIGGER_THAN, // >
    OP_LESS_THAN, // <
    OP_BIGGER_THAN_COMPARE, // >=
    OP_LESS_THAN_COMPARE, // <=

    // MATH
    OP_ADD, // +
    OP_SUB, // -
    OP_TIMES, // *
    OP_POWER, // ^
    OP_DIVIDE, // /
    OP_MODULO // %
} Operators;

// Structures

typedef struct AST_Args {
    struct AST_Node* Args;
    size_t Size;
    size_t AllocatedNodes;
} AST_Args;

typedef struct AST_Block {
        struct AST_Node* statements;
        size_t Size;
        size_t AllocatedNodes;
} AST_Block;

// Structures (AST)

typedef struct AST_WhileLoop {
    struct AST_Node* Condition;
    AST_Block Block; // ASTNODE_BLOCK
} AST_WhileLoop;

typedef struct AST_If {
    struct AST_Node* Condition;
    AST_Block Block;
    AST_Block Else_block;
} AST_If;

typedef struct AST_FunctionDefine {
    char* FunctionName;
    struct AST_Args Args;
    AST_Block Block; // ASTNODE_BLOCK
} AST_FunctionDefine;

typedef struct AST_FunctionCall {
    char* FunctionName;
    struct AST_Args Args;
} AST_FunctionCall;

typedef struct AST_VariableDefine {
    char* VariableName;
    struct AST_Node* VariableValue;
} AST_VariableDefine;

typedef struct AST_BinaryOperation {
    Operators Operator;
    struct AST_Node* Left;
    struct AST_Node* Right;
} AST_BinaryOperation;


typedef struct AST_Node {
    NodeTypes ASTType;
    union {
        intmax_t Number; // ASTNODE_NUMBER
        char* String; // ASTNODE_STRING
        char* Variable; // ASTNODE_VARIABLE
        bool Boolean; // ASTNODE_BOOLEAN
        AST_BinaryOperation BinaryOperation; // ASTNODE_BINARY_OPERATION
        AST_VariableDefine VariableDefine; // ASTNODE_VARIABLE_DEFINE
        AST_FunctionCall FunctionCall; // ASTNODE_FUNCTION_CALL
        AST_FunctionDefine FunctionDefine; // ASTNODE_FUNCTION_DEFINE
        AST_WhileLoop WhileLoop; // ASTNODE_WHILE_LOOP
        AST_If IfStatement; // ASTNODE_IF && ASTNODE_IF_ELSE
    };
} AST_Node;

typedef struct Abstract_Syntax_Tree{
    struct AST_Node* Array;
    size_t Size;
    size_t AllocatedNodes;
} Abstract_Syntax_Tree;

// Functions

// Uses tokens to ensure correct grammar and syntax for KebabLang and creates a Abstract Syntax Tree for the interpreter
// Returns AbstractSyntaxTree on success
// Returns AbstractSyntaxTree with Node* NodeArray as NULL on failure
Abstract_Syntax_Tree Parse(Lexer* Tokens);

// Frees the Abstract Syntax Tree
void FreeParser(Abstract_Syntax_Tree* AST);

#ifdef __cplusplus
}
#endif

#endif