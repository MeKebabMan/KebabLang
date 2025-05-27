#ifndef _PARSER_H_
#define _PARSER_H_

#ifdef __cplusplus
extern "c"
{
#endif

// Includes

#include <stddef.h>
#include <kebablang/lexer.h>

// Enums

typedef enum {
    NODE_NUMBER,
    NODE_STRING,
    NODE_VARIABLE,
    NODE_ADD, // For math (Example: 5 * 3)
    NODE_ASSIGN,
} NodeTypes;

// Structures

typedef struct AssignNode {
    char* Name;
    struct Node* Value;
} AssignNode;

typedef struct OperatorNode {
    struct Node* Left;
    struct Node* Right;
} BinaryOperationNode;

typedef struct VariableEntry {
    char* Name;
    union {
        int number;
        char* String;
    };
} VariableEntry;

typedef struct Node {
    NodeTypes Type;
    union {
        int Number;
        char* String;
        char* Variable;
        BinaryOperationNode OperatorAdd;
        AssignNode Assign;
    };
} Node;

typedef struct AbstractSyntaxTree {
    Node* NodeArray;
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