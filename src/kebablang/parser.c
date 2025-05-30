#include <inttypes.h>
#include <kebablang/lexer.h>
#include <kebablang/parser.h>
#include <KebabLib.h>
#include <stddef.h>
#include <unistd.h>

// Marcos
#define FailedParse (AbstractSyntaxTree){(void*)NULL, 0}