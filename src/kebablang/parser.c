#include <kebablang/parser.h>

// Marcos
#define FailedParse (AbstractSyntaxTree){(void*)NULL, 0}

// Helper functions (static)

// Functions

AbstractSyntaxTree Parse(TokenArray* Tokens) {

    if (Tokens == NULL || Tokens->Array == NULL) {
        return FailedParse;
    }

    // Ensure grammar and syntax

    // Create Abstract syntax tree

    return FailedParse;
}

void FreeParser(AbstractSyntaxTree *AST) {
    if (AST == NULL || AST->NodeArray == NULL) {
        return;
    }

    // Free the abstract syntax tree nodes

    // Free the abstract syntax tree it self

}