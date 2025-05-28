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
    NODE_NUMBER, // 123345123
    NODE_BOOLEAN, // true / false
    NODE_STRING, // "Hello, world!"
    NODE_VARIABLE, // Example: print( VARIABLE )
    NODE_ASSIGN, // Example: MyVariable = "Hi"
    NODE_BINARY_OPERATION, // Example: 3 + 4, 5 * 3, etc
    NODE_FUNCTION_CALL, // Example: print("hello, world")
    NODE_FUNCTION_DEFINE, // Example: MyFunction(MyArgs) {}
    NODE_WHILE_LOOP, // Example: while true {}
} NodeTypes;

typedef enum {
    NODE_ADD,
    NODE_MINUS,
    NODE_TIMES,
    NODE_DIVIDE,
    NODE_MODULO,
} NodeOperator;

// Structures

typedef struct AssignNode { // Assign structure for assigning variables. Example: <Name> = <Value>
    char* Name; // Heap allocation 
    struct ASTNode* Value; // Heap allocation
} AssignNode;

typedef struct WhileLoopNode { // While Loop structure. Example: while <EXPRESSION> { <BODY> }
    struct ASTNode* Expression; // Heap allocation (The while loop's condition)
    struct Body* WhileLoop; // Heap allocation (While loop body)
} WhileLoopNode;

typedef struct FunctionCallNode { // A structure for calling functions. Example: <Name>( <Args> )
    char* Name; // Heap allocation
    struct FunctionArgs* Args; // Heap allocation (Function Arguments)
} FunctionCallNode;

typedef struct FunctionDefineNode { // A structure for creating functions. Example: <Name>( <Args> ) { <FunctionBody> }
    char* Name; // Heap allocation
    struct FunctionArgs* Args; // Heap allocation (Function Arguments)
    struct Body* FunctionBody; // Heap allocation (Function Body)
} FunctionDefineNode;

typedef struct OperatorNode { // A structure for Binary Operations. Example: <Left> <Type> <Right> ( 1 + 3, 5 * 3, etc )
    NodeOperator Type; // Operator type
    struct ASTNode* Left;
    struct ASTNode* Right;
} BinaryOperationNode;

typedef struct FunctionArgs { // Structure for handling Function Arguments. Example: ( <Args> )
    struct ASTNode* Args; // Heap allocation (Array of arguments)
    size_t Size;
} FunctionArgs;

typedef struct Body { // Structure for handling Body. Example: { <Statements> }
    struct ASTNode* Statements; // Heap allocation (Array of statements)
    size_t BodySize;
} BodyNode;

typedef struct ASTNode { // A structure for handling abstract syntax tree nodes (MAIN)
    NodeTypes Type; // The Type of the node (NODE_NUMBER, NODE_STRING, NODE_VARIABLE_DEFINE, etc)
    union { // The AST Node's value
        intmax_t NodeNumber;
        char* NodeString; // Heap allocation
        char* NodeVariable; // Heap allocation
        bool NodeBoolean;
        BinaryOperationNode NodeBinaryOperation;
        AssignNode NodeAssign;
        FunctionDefineNode NodeFunctionDefine;
        FunctionCallNode NodeFunctionCall;
        WhileLoopNode NodeWhileLoop;
    };
} ASTNode;

typedef struct AbstractSyntaxTree { // A structure for handling the AST.. kind of like a arena or a pool
    ASTNode* NodeArray; // Heap allocation
    size_t NodeMemory;
    size_t AllocatedNodes;
} AbstractSyntaxTree;

// Functions

// Uses tokens to ensure correct grammar and syntax for KebabLang and creates a Abstract Syntax Tree for the interpreter
// Returns AbstractSyntaxTree on success
// Returns AbstractSyntaxTree with Node* NodeArray as NULL on failure
AbstractSyntaxTree Parse(TokenArray* Tokens);

// Frees the Abstract Syntax Tree
void FreeParser(AbstractSyntaxTree* AST);

#ifdef __cplusplus
}
#endif

#endif