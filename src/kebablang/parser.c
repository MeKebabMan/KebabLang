#include <inttypes.h>
#include <kebablang/lexer.h>
#include <kebablang/parser.h>
#include <KebabLib.h>
#include <stddef.h>
#include <unistd.h>

// Marcos
#define AST_FailedParse (Abstract_Syntax_Tree){(void*)NULL, 0, 0}

// Parser functions (static)

// Helper functions (static)

static struct AST_Node* Create_AST_Node();
