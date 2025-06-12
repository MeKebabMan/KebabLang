#include <assert.h>
#include <complex.h>
#include <kebablang/lexer.h>
#include <KebabLib.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

// Marcos
#define FailedTokenize (Lexer){(Token*)NULL, 0, 0, 0}

#define Is_Start(c) ((c) != ' ' && (c) != '\n' && (c) != '\t')

#define Is_Empty(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')

#define Is_SingleCharacter(c) ((c) == '(' || (c) == ')' || (c) == '[' || (c) == ']' || (c) == '{' || (c) == '}' || \
    (c) == '#' || (c) == ',' || (c) == '@' || (c) == '$' || (c) == '|' || (c) == '\\' || (c) == '`' || (c) == '~' || (c) == '?')

#define Is_Operator(c) ((c) == '+' || (c) == '-' || (c) == '*' || (c) == '/' || (c) == '^' || (c) == '%' || (c) == '<' || (c) == '>' || (c) == '!' || (c) == '=')

#define Invalid_Index ((size_t)-1)
#define Error(Messaage) fprintf(stderr, "Lexer: (%s) %s\n", __func__, Messaage);

// Structures

typedef struct {
    const char* Word;
    TokenType Token;
} Keyword;

// Arrays

static const Keyword KeywordEntries[] = {
    {"if", TOKEN_IF},
    {"while", TOKEN_WHILE},
    {"else", TOKEN_ELSE},
    {"false", TOKEN_FALSE},
    {"true", TOKEN_TRUE},
    {"function", TOKEN_FUNCTION}
};

// Enums

typedef enum {
    LEXER_NORMAL,
    LEXER_STRING,
    LEXER_EMPTY,
    LEXER_COMMENT,
    LEXER_SINGLE_CHARACTER,
} LexerMode;

// Helper functions (static)

static Token* ReserveToken(Lexer* Array); // Declaration 
static int ExtendLexer(Lexer* Array, size_t ExtendSize); // Declaration
static int CleanLexer(Lexer* Array); // Declaration

static Token* ReserveToken(Lexer* Array) {

    if (Array == NULL || Array->Array == NULL) {
        Error("Expected NON NULL args")
        return NULL;
    }

    // Check if we have enough space to insert
    if (Array->AllocatedTokens == Array->Size) {
        // Extend the Array
        if (ExtendLexer(Array, 4) == -1) { // Extend by 4 so we can easily insert
            Error("Failed to extend memory");
            return NULL;
        }
    }

    // Create the Token
    Token* NewToken = &Array->Array[Array->AllocatedTokens];
    
    *NewToken = (Token){
        .Token = TOKEN_IDENTIFIER,
        .Data = NULL,
        .Length = 0,
        .Line = 0
    };

    // Increment Allocated Tokens
    Array->AllocatedTokens++;

    return NewToken;
}

static int ExtendLexer(Lexer* Array, size_t ExtendSize) {

    if (Array == NULL || Array->Array == NULL) {
        Error("Expected NON NULL args");
        return -1;
    }

    if (ExtendSize == 0) {
        return 0; // Skip
    }

    // New allocation
    Token* NewAllocation = calloc(Array->Size + ExtendSize, sizeof(Token));
    if (NewAllocation == NULL) {
        Error("Failed to allocate memory");
        return -1;
    }

    // Copy old memory allocation
    memcpy(NewAllocation, Array->Array, Array->Size * sizeof(Token));

    // Free the old allocation
    free(Array->Array);

    // Replace old variable pointer
    Array->Array = NewAllocation;

    // Update the size
    Array->Size = Array->Size + ExtendSize;

    return 0;
}

static int CleanLexer(Lexer* Array) { // Cleans up all the unused space within the Array (Based on size_t AllocatedTokens)
    
    if (Array == NULL || Array->Array == NULL) {
        Error("Expected NON NULL args");
        return -1;
    }

    if (Array->Size == Array->AllocatedTokens) {
        return 0; // Skip
    }
    
    // Allocate new memory
    Token* NewAllocation = malloc(Array->AllocatedTokens * sizeof(Token));
    if (NewAllocation == NULL) {
        Error("Failed to allocate memory");
        return -1;
    }

    // Copy old memory
    memcpy(NewAllocation, Array->Array, Array->AllocatedTokens * sizeof(Token));

    // Free the old allocation
    free(Array->Array);

    // Replace variable pointer
    Array->Array = NewAllocation;
    
    // Update the size
    Array->Size = Array->AllocatedTokens;

    return 0;
}

// Functions

Lexer Tokenize(int fd); // Declaration
void FreeLexer(Lexer* Array); // Declaration

Lexer Tokenize(int fd) {

    // Get the file MetaData
    // By the way this also is pretty much a check to ensure the file descripter is valid
    struct stat metadata; 
    if (fstat(fd, &metadata) == -1) {
        Error("Failed to get file meta data");
        return FailedTokenize;
    }

    // Ensure the file is a normal file
    if (!S_ISREG(metadata.st_mode)) {
        Error("Expected regular file");
        return FailedTokenize;
    }

    // Get the File Size
    size_t FileSize = (size_t)metadata.st_size;

    // Allocate the Token Array
    Lexer Array = {
        .Array = malloc(1 * sizeof(Token)),
        .Size = 1,
        .AllocatedTokens = 0,
        .CursorOffset = 0
    };

    if (Array.Array == NULL) {
        Error("Failed to allocate memory");
        return FailedTokenize;
    }

    // Read the Buffer
    size_t TotalBytesRead = 0;
    char* Buffer = calloc(FileSize + 1, sizeof(char)); // +1 for the NULL Terminator
    if (Buffer == NULL) {
        Error("Failed to allocate memory for reading");
        FreeLexer(&Array);
        return FailedTokenize;
    }

    while (TotalBytesRead < FileSize) {
        ssize_t BytesRead = read(fd, &Buffer[TotalBytesRead], FileSize - TotalBytesRead);

        if (BytesRead == -1) {
            Error("Read failed");
            perror("read");
            FreeLexer(&Array);
            return FailedTokenize;
        }

        TotalBytesRead += BytesRead;

    }

    // Declare variables for tracking
    size_t Start = Invalid_Index;
    size_t CurrentLine = 1;
    LexerMode Mode = LEXER_EMPTY;

    // Read the file byte by byte and tokenize every word

    for (size_t index = 0; index < FileSize; index++) {

        // ONLY PROCESS LINE IF IT IS NOT EMPTY
        if (Start == Invalid_Index && Mode == LEXER_EMPTY && !Is_Empty(Buffer[index])) {
            Mode = LEXER_NORMAL; // Set to normal because it isnt empty
        }

        // Handle new lines
        if (Buffer[index] == '\n' || Buffer[index] == '\r') {

            // Build Tokens that are incomplete
            if (Start != Invalid_Index) {

                size_t Length = index - Start;

                Token* ReservedToken = ReserveToken(&Array);
                if (ReservedToken == NULL) {
                    // Failed to Create Token
                    Error("(1) (NEWLINE) Failed to Tokenize\n");
                    FreeLexer(&Array);
                    free(Buffer);
                    return FailedTokenize;
                }

                ReservedToken->Data = calloc(Length + 1, sizeof(char)); // +1 For the NULL Terminator
                if (ReservedToken->Data == NULL) {
                    // Failed to Create Token
                    Error("(2) (NEWLINE) Failed to Tokenize\n");
                    FreeLexer(&Array);
                    free(Buffer);
                    return FailedTokenize;
                }

                memcpy(ReservedToken->Data, &Buffer[Start], Length);

                ReservedToken->Length = Length;

                // Ensure Null Terminator
                ReservedToken->Data[Length] = '\0'; // In case of feature modifications..

                // Type the token
                
                // Defaults to TOKEN_INDENTIFER

                // Keywords
                if (ReservedToken->Token == TOKEN_IDENTIFIER) {

                    size_t Keywords = sizeof(KeywordEntries)/sizeof(KeywordEntries[0]);
                    for (size_t index = 0; index < Keywords; index++) {
                        if (strcmp(ReservedToken->Data, KeywordEntries[index].Word) == 0) { // In this case strcmp is safe because both strings are NULL terminated
                            ReservedToken->Token = KeywordEntries[index].Token;
                        }
                    }

                }
                
                // Number
                if (IsNumberOnly(ReservedToken->Data, ReservedToken->Length) == 0 && ReservedToken->Token == TOKEN_IDENTIFIER) {
                    ReservedToken->Token = TOKEN_NUMBER;
                }

            }

            // Reset tracking variables
            Start = Invalid_Index;
            Mode = LEXER_EMPTY; // Reset Processing Mode
            CurrentLine++;
        }

        if (Mode == LEXER_COMMENT || Mode == LEXER_EMPTY) { // Skip processing 
            continue; // Skip
        }

        // Handle Single tokens that are connected to other Tokens..

        if (Mode == LEXER_NORMAL && (Is_SingleCharacter(Buffer[index]) || Is_Operator(Buffer[index]))) {
            Mode = LEXER_SINGLE_CHARACTER;
        }

        // Get the start of a token
        if (Start == Invalid_Index && Mode == LEXER_NORMAL && Is_Start(Buffer[index])) {
            Start = index;
        }

        // Handle tokens

        // Normal
        // Normal is the default.. it gets set when the line is not empty

        // String
        if (Start != Invalid_Index && Mode == LEXER_NORMAL && Buffer[Start] == '"') {
            Mode = LEXER_STRING;
            continue; // Skip
        }
        

        // Lexer Modes
        switch (Mode) {
            case LEXER_NORMAL: // Normally tokenize by well.. looking for ' ' or '\t'
                if (Start != Invalid_Index && (Buffer[index] == ' ' || Buffer[index] == '\t')) {

                    size_t Length = index - Start;

                    Token* ReservedToken = ReserveToken(&Array);
                    if (ReservedToken == NULL) {
                        // Failed to Create Token
                        Error("(1) Failed to Tokenize\n");
                        FreeLexer(&Array);
                        free(Buffer);
                        return FailedTokenize;
                    }

                    ReservedToken->Data = calloc(Length + 1, sizeof(char)); // +1 For the NULL Terminator
                    if (ReservedToken->Data == NULL) {
                        // Failed to Create Token
                        Error("(2) Failed to Tokenize\n");
                        FreeLexer(&Array);
                        free(Buffer);
                        return FailedTokenize;
                    }

                    memcpy(ReservedToken->Data, &Buffer[Start], Length);

                    ReservedToken->Length = Length;
                    ReservedToken->Line = CurrentLine;

                    // Ensure Null Terminator
                    ReservedToken->Data[Length] = '\0'; // In case of feature modifications..

                    // Type the token
                    
                    // Defaults to TOKEN_INDENTIFER

                    // Keywords
                    if (ReservedToken->Token == TOKEN_IDENTIFIER) {

                        size_t Keywords = sizeof(KeywordEntries)/sizeof(KeywordEntries[0]);
                        for (size_t index = 0; index < Keywords; index++) {
                            if (strcmp(ReservedToken->Data, KeywordEntries[index].Word) == 0) { // In this case strcmp is safe because both strings are NULL terminated
                                ReservedToken->Token = KeywordEntries[index].Token;
                            }
                        }

                    }
                    
                    // Number
                    if (IsNumberOnly(ReservedToken->Data, ReservedToken->Length) == 0 && ReservedToken->Token == TOKEN_IDENTIFIER) {
                        ReservedToken->Token = TOKEN_NUMBER;
                    }

                    Start = Invalid_Index;
                    Mode = LEXER_NORMAL;
                }
            break;
            case LEXER_STRING: // Keep looking for a END '"' until you reach FileSize..
                    if (Start != Invalid_Index && Buffer[index] == '"') {

                        size_t Length = (index + 1) - Start;

                        Token* ReservedToken = ReserveToken(&Array);
                        if (ReservedToken == NULL) {
                            // Failed to Create Token
                            Error("(1) (STRING) Failed to Tokenize\n");
                            FreeLexer(&Array);
                            free(Buffer);
                            return FailedTokenize;
                        }

                        ReservedToken->Data = calloc(Length + 1, sizeof(char)); // +1 For the NULL Terminator
                        if (ReservedToken->Data == NULL) {
                            // Failed to Create Token
                            Error("(2) (STRING) Failed to Tokenize\n");
                            FreeLexer(&Array);
                            free(Buffer);
                            return FailedTokenize;
                        }

                        ReservedToken->Token = TOKEN_STRING;

                        memcpy(ReservedToken->Data, &Buffer[Start], Length);

                        ReservedToken->Length = Length;
                        ReservedToken->Line = CurrentLine;

                        // Ensure Null Terminator
                        ReservedToken->Data[Length] = '\0'; // In case of feature modifications..

                        Start = Invalid_Index;
                        Mode = LEXER_NORMAL;
                    }
            break;
            case LEXER_SINGLE_CHARACTER: { // Handle Single characters such as '(' or ')' or '[' or ']'

                    char SingleCharacter = Buffer[index];

                    // Finish the token that didnt get tokenized.. 
                    if (Start != Invalid_Index) {

                        size_t Length = index - Start;
                        
                        Token* ReservedToken = ReserveToken(&Array);
                        if (ReservedToken == NULL) {
                            // Failed to Create Token
                            Error("(1) (COMMENT) Failed to Tokenize\n");
                            FreeLexer(&Array);
                            free(Buffer);
                            return FailedTokenize;
                        }

                        ReservedToken->Data = calloc(Length + 1, sizeof(char)); // +1 For the NULL Terminator
                        if (ReservedToken->Data == NULL) {
                            // Failed to Create Token
                            Error("(2) (COMMENT) Failed to Tokenize\n");
                            FreeLexer(&Array);
                            free(Buffer);
                            return FailedTokenize;
                        }

                        memcpy(ReservedToken->Data, &Buffer[Start], Length);

                        ReservedToken->Length = Length;
                        ReservedToken->Line = CurrentLine;

                        // Ensure Null Terminator
                        ReservedToken->Data[Length] = '\0'; // In case of feature modifications..

                        // Type the token
                        
                        // Defaults to TOKEN_INDENTIFER

                        // Keywords
                        if (ReservedToken->Token == TOKEN_IDENTIFIER) {

                            size_t Keywords = sizeof(KeywordEntries)/sizeof(KeywordEntries[0]);
                            for (size_t index = 0; index < Keywords; index++) {
                                if (strcmp(ReservedToken->Data, KeywordEntries[index].Word) == 0) { // In this case strcmp is safe because both strings are NULL terminated
                                    ReservedToken->Token = KeywordEntries[index].Token;
                                }
                            }

                        }
                        
                        // Number
                        if (IsNumberOnly(ReservedToken->Data, ReservedToken->Length) == 0 && ReservedToken->Token == TOKEN_IDENTIFIER) {
                            ReservedToken->Token = TOKEN_NUMBER;
                        }

                        Start = Invalid_Index;
                        Mode = LEXER_NORMAL;
                    }

                    if (Is_SingleCharacter(SingleCharacter) && SingleCharacter != '#') {

                        // Tokenize
                        size_t Length = 1;
                        
                        Token* ReservedToken = ReserveToken(&Array);
                        if (ReservedToken == NULL) {
                            // Failed to Create Token
                            Error("(1) (SINGLE CHARACTER) Failed to Tokenize\n");
                            FreeLexer(&Array);
                            free(Buffer);
                            return FailedTokenize;
                        }

                        ReservedToken->Data = calloc(Length + 1, sizeof(char)); // +1 For the NULL Terminator
                        if (ReservedToken->Data == NULL) {
                            // Failed to Create Token
                            Error("(2) (SINGLE CHARACTER) Failed to Tokenize\n");
                            FreeLexer(&Array);
                            free(Buffer);
                            return FailedTokenize;
                        }

                        memcpy(ReservedToken->Data, &Buffer[index], Length);

                        // Type the token
                        if (SingleCharacter == '(') {
                            ReservedToken->Token = TOKEN_PARENTHESES1;
                        } else if (SingleCharacter == ')') {
                            ReservedToken->Token = TOKEN_PARENTHESES2;
                        } else if (SingleCharacter == '[') {
                            ReservedToken->Token = TOKEN_SQUARE_BRACKETS1;
                        } else if (SingleCharacter == ']') {
                            ReservedToken->Token = TOKEN_SQUARE_BRACKETS2;
                        } else if (SingleCharacter == '{') {
                            ReservedToken->Token = TOKEN_BRACKETS1;
                        } else if (SingleCharacter == '}') {
                            ReservedToken->Token = TOKEN_BRACKETS2;
                        } else if (SingleCharacter == ',') {
                            ReservedToken->Token = TOKEN_COMMA;
                        } else {
                            ReservedToken->Token = TOKEN_UNKNOWN;
                        }

                        ReservedToken->Length = Length;
                        ReservedToken->Line = CurrentLine;

                        // Ensure Null Terminator
                        ReservedToken->Data[Length] = '\0'; // In case of feature modifications..
                        
                        Start = Invalid_Index;
                        Mode = LEXER_NORMAL;
                        continue; // Skip
                    }

                    if (Is_Operator(SingleCharacter)) {

                        size_t Length = 1;

                        if ((index + 1) < FileSize && Buffer[index + 1] == '=') {
                            // Multi Operator
                            Length++;
                        }
                       
                        Token* ReservedToken = ReserveToken(&Array);
                        if (ReservedToken == NULL) {
                            // Failed to Create Token
                            Error("(1) (OPERATOR) Failed to Tokenize\n");
                            FreeLexer(&Array);
                            free(Buffer);
                            return FailedTokenize;
                        }

                        ReservedToken->Data = calloc(Length + 1, sizeof(char)); // +1 For the NULL Terminator
                        if (ReservedToken->Data == NULL) {
                            // Failed to Create Token
                            Error("(2) (OPERATOR) Failed to Tokenize\n");
                            FreeLexer(&Array);
                            free(Buffer);
                            return FailedTokenize;
                        }

                        memcpy(ReservedToken->Data, &Buffer[index], Length);

                        // Type the token
                        if (Length == 2) {
                            ReservedToken->Token = TOKEN_MULTI_OPERATOR;
                        } else if (Length == 1) {
                            ReservedToken->Token = TOKEN_SINGLE_OPERATOR;
                        }

                        ReservedToken->Length = Length;
                        ReservedToken->Line = CurrentLine;

                        // Ensure Null Terminator
                        ReservedToken->Data[Length] = '\0'; // In case of feature modifications..

                        if (Length == 2) {
                            index++;
                        }

                        Start = Invalid_Index;
                        Mode = LEXER_NORMAL;
                        continue; // Skip
                    }

                    // Handle Comments
                    if (SingleCharacter == '#') {

                        // Set Mode to comment and Start to Invalid_Index
                        Mode = LEXER_COMMENT;
                        Start = Invalid_Index;

                        continue; // Skip
                    }

                break;
            }
            default:
                Error("What? Why did the switch statement enter default mode??\n");
                FreeLexer(&Array);
                free(Buffer);
                return FailedTokenize;
            break;
        }

    }

    // Handle last line.. (If statement should only be Start != Invalid_Index.. in case someone forgot to add a " to end there string)
    if (Start != Invalid_Index) {

        // Length
        size_t Length = FileSize - Start;

        // Tokenize
        Token* ReservedToken = ReserveToken(&Array); // Defaults to TOKEN_UNKNOWN
        if (ReservedToken == NULL) {
            // Failed to Create Token
            Error("(1) (END) Failed to Tokenize\n");
            FreeLexer(&Array);
            free(Buffer);
            return FailedTokenize;
        }

        ReservedToken->Data = calloc(Length + 1, sizeof(char)); // +1 For the NULL Terminator
        if (ReservedToken->Data == NULL) {
            // Failed to Create Token
            Error("(2) (END) Failed to Tokenize\n");
            FreeLexer(&Array);
            free(Buffer);
            return FailedTokenize;
        }

        memcpy(ReservedToken->Data, &Buffer[Start], Length);

        ReservedToken->Length = Length;
        ReservedToken->Length = CurrentLine;

        // Ensure Null Terminator
        ReservedToken->Data[Length] = '\0'; // In case of feature modifications..

        // Type the token
        
        // Defaults to TOKEN_INDENTIFER

        // Keywords
        if (ReservedToken->Token == TOKEN_IDENTIFIER) {

            size_t Keywords = sizeof(KeywordEntries)/sizeof(KeywordEntries[0]);
            for (size_t index = 0; index < Keywords; index++) {
                if (strcmp(ReservedToken->Data, KeywordEntries[index].Word) == 0) { // In this case strcmp is safe because both strings are NULL terminated
                    ReservedToken->Token = KeywordEntries[index].Token;
                }
            }

        }
        
        // Number
        if (IsNumberOnly(ReservedToken->Data, ReservedToken->Length) == 0 && ReservedToken->Token == TOKEN_IDENTIFIER) {
            ReservedToken->Token = TOKEN_NUMBER;
        }

        // Reset tracking variables in case of feature modifications
        Start = Invalid_Index;
        Mode = LEXER_EMPTY;
    }

    // Clean up
    free(Buffer);

    // Clean Array
    if (CleanLexer(&Array) == -1) {
        Error("Failed to clean token array\n");
        FreeLexer(&Array);
        return FailedTokenize;
    }

    return Array;
}

void FreeLexer(Lexer* Array) {
    
    if (Array == NULL || Array->Array == NULL) {
        Error("Expected NON NULL args");
        return; // Ensure we are not freeing a NULL array (Token* Array)
    }
    
    // Loop through the tokens and free there data (char* Data)
    for (size_t index = 0; index < Array->AllocatedTokens; index++) {

        if (Array->Array[index].Data != NULL) { // (Guard Rail)
            // Free the Data (char* Data)
            free(Array->Array[index].Data);
            Array->Array[index].Data = NULL; // Ensure no undefined behaviour
        }

    }

    // Free the Array (Token* Array)
    free(Array->Array);
    Array->Array = NULL; // Ensure no undefined behaviour

    // Reset fields within TokenArray (In case of feature modifications)
    Array->Size = 0;
    Array->AllocatedTokens = 0;

}