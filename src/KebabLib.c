#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>

// Declare functions

void KebabPrintf(FILE* __restrict__ FileStream,  char* __restrict__ format, ...);
void KebabPrint(FILE* __restrict__ FileStream, char* __restrict__ content);

char* TrimBuffer(const char* buffer, size_t end);
int IsNumberOnly(const char* buffer, size_t end); 
int IsOperator(char character);

int KebabWriteFD(int fd, char* buffer, size_t bytes);
int KebabWriteFile(const char* __restrict__ file, char* buffer, size_t bytes);

char* KebabReadFile(const char* __restrict__ file);

char* ParseIMAX(intmax_t number);
char* ParseUMAX(uintmax_t number);

// Functions 

void KebabPrintf(FILE* __restrict__ FileStream,  char* __restrict__ format, ...) {

    if (FileStream == NULL) {
        FileStream = stdout;
    }

    if (format == NULL) {
        format = "NULL";

        fputs(format, FileStream);

        return;

    }

    // Lock file stream
    flockfile(FileStream);

    va_list args;
    va_start(args, format);

    while(*format != '\0') {

        if (*format == '%') {

            format++; // Move ahead
            
            if (*format == '%') {
                putc('%', FileStream);
                format++;
                continue; // Skip
            }

            if (*format == 's') {

                char* s = va_arg(args, char*);

                fputs(s, FileStream);

            }

            if (*format == 'c') {

                char c = va_arg(args, int);

                fputc(c, FileStream);

            }

            if (*format == 'd') {

                int d = va_arg(args, int);

                char* ParsedInt = ParseIMAX((int)d);
                if (ParsedInt == NULL) {
                    fputs("Failed", FileStream);
                    format++;
                    continue; // Skip
                }

                fputs(ParsedInt, FileStream);

                free(ParsedInt);

            }

            if (*format == 'z') {
                format++;
                if (*format == 'u') {

                    size_t zu = va_arg(args, size_t);

                    char* ParsedInt = ParseUMAX((size_t)zu);
                    if (ParsedInt == NULL) {
                        fputs("Failed", FileStream);
                        format++;
                        continue; // Skip
                    }

                    fputs(ParsedInt, FileStream);

                    free(ParsedInt);

                }
            }


        } else {
            fputc(*format, FileStream);
        }

        format++;
    }

    va_end(args);

    // Flush the file stream
    fflush(FileStream);

    // Unlock the file stream
    funlockfile(FileStream);

}

int KebabWriteFD(int fd, char* buffer, size_t bytes) {

    if (bytes == 0) {
        return 0;
    }

    if (fd <= -1 || buffer == NULL) {
        return -1;
    }

    // Write

    size_t TotalBytesWritten = 0;

    while (TotalBytesWritten < bytes) {

        ssize_t BytesWritten = write(fd, &buffer[TotalBytesWritten], bytes - TotalBytesWritten);

        if (BytesWritten == -1) {
            return -1;
        }

        TotalBytesWritten += BytesWritten;

    }

    // Flush
    fsync(fd);

    return 0;
}

int KebabWriteFile(const char* __restrict__ file, char* buffer, size_t bytes) {

    if (bytes == 0) {
        return 0; // Nothing to write
    }

    if (file == NULL || buffer == NULL) {
        return -1;
    }

    // Open the file
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        return -1;
    }

    // Write

    size_t TotalBytesWritten = 0;

    while (TotalBytesWritten < bytes) {

        ssize_t BytesWritten = write(fd, &buffer[TotalBytesWritten], bytes - TotalBytesWritten);

        if (BytesWritten == -1) {
            close(fd);
            return -1;
        }

        TotalBytesWritten += BytesWritten;
        
    }

    // fsync to ensure all the bytes were written
    fsync(fd);

    close(fd);

    return 0;
}

char* KebabReadFile(const char* __restrict__ file) {

    if (file == NULL) {
        return NULL;
    }

    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        return NULL;
    }

    // Get the file MetaData
    struct stat metadata;
    if (fstat(fd, &metadata) == -1) {
        close(fd);
        return NULL;
    }

    // Check if is REG
    if (!S_ISREG(metadata.st_mode)) {
        close(fd);
        return NULL;
    }

    size_t size = (size_t)metadata.st_size;

    char* buffer = calloc(size + 1, sizeof(char)); // +1 for the NULL terminator
    if (buffer == NULL) {
        close(fd);
        return NULL;
    }

    ssize_t readReturn = read(fd, buffer, size);
    if (readReturn == -1) {
        free(buffer);
        close(fd);
        return NULL;
    }

    close(fd);

    return buffer;
}

char* TrimBuffer(const char* buffer, size_t end) {

    if (buffer == NULL) { // (Guard Rail)
        return NULL;
    }

    // Ignore Excess white space at start
    size_t start = 0;
    while (start < end && (buffer[start] == ' ' || buffer[start] == '\t')) {
        start++;
    }

    // Check if the buffer is all white space / tab
    if (start == end) {
        return strdup("\0"); // Return Empty buffer
    }

    // Find the END
    size_t TrimmedEnd = end;
    while (TrimmedEnd > start && (buffer[TrimmedEnd] != ' ' && buffer[TrimmedEnd] != '\t')) {
        TrimmedEnd--;
    }

    // Allocate memory 
    char* Memory = calloc(TrimmedEnd + 1, sizeof(char)); // +1 for the Null Terminater
    if (Memory == NULL) {
        return NULL;
    }

    // Copy Memory into the allocated memory
    memcpy(Memory, &buffer[start], TrimmedEnd + start);

    // Null Terminate the buffer in case of feature modifications to ensure no bugs
    Memory[TrimmedEnd + 1] = '\0';

    // Return the allocated memory
    return Memory;
}

void KebabPrint(FILE* __restrict__ FileStream, char* __restrict__ content) {

    if (FileStream == NULL) {
        FileStream = stdout;
    }

    if (content == NULL) {
        content = "NULL";
    }

    // Lock the File Stream
    flockfile(FileStream);

    // Log
    fputs(content, FileStream);

    // Flush the File Stream
    fflush(FileStream);

    // Unlock the File Stream
    funlockfile(FileStream);

}

int IsNumberOnly(const char* buffer, size_t end) {

    if (buffer == NULL) {
        return -1;
    }

    for (size_t index = 0; index < end; index++) {
        if (buffer[index] > 48 && buffer[index] < 58) {
            continue;
        }

        return -2;
    }    

    return 0;
}


int IsOperator(char character) {

    if (character == '+') {
        return 0;
    }
    
    if (character == '-') {
        return 0;
    }
    
    if (character == '/') {
        return 0;
    }
    
    if (character == '%') {
        return 0;
    }
    
    if (character == '=') {
        return 0;
    }

    if (character == '*') {
        return 0;
    }

    if (character == '&') {
        return 0;
    }

    if (character == '^') {
        return 0;
    }

    return -1;
}

char* ParseIMAX(intmax_t number) {

    // Declare variables for tracking (Declare integer variables as unsigned.. so they can never be negative.. helps prevent UB)
    bool IsNegative = false;
    uintmax_t AllocatedMemory = 0;
    uintmax_t WriteOffset = 0;

    uintmax_t Absolute = (uintmax_t)number;
    if (number < 0) {
        // Handle Negatives
        Absolute = (uintmax_t)(-(intmax_t)number); // Flip the value
        AllocatedMemory++; // For the -
        IsNegative = true;
    }

    // Get the size

    uintmax_t TempNumber = Absolute;

    do {
        TempNumber = TempNumber / 10;        
        AllocatedMemory++;
    } while (TempNumber != 0);

    // Set WriteOffset

    WriteOffset = AllocatedMemory - 1;

    // Allocate Memory
    char* Memory = malloc(AllocatedMemory + 1);
    if (Memory == NULL) {
        return NULL;
    }

    // Insert
    do {

        Memory[WriteOffset] = '0' + (Absolute % 10);
        WriteOffset--;

        Absolute = Absolute / 10;        
    } while (Absolute != 0);

    // Handle Negative value
    if (IsNegative) {
        Memory[0] = '-';
    }

    // Null Terminate
    Memory[AllocatedMemory] = '\0';

    return Memory;
}

char* ParseUMAX(uintmax_t number) {

    // Declare variables for tracking
    uintmax_t AllocatedMemory = 0;
    uintmax_t WriteOfffset = 0;

    // Get the size
    uintmax_t TempNumber = number;
    do {
        TempNumber = TempNumber / 10;
        AllocatedMemory++;
    } while (TempNumber != 0);

    // Set WriteOffset
    WriteOfffset = AllocatedMemory - 1;

    // Allocate Memory
    char* Memory = malloc(AllocatedMemory + 1);
    if (Memory == NULL) {
        return NULL;
    }

    // Insert    
    do {
        
        Memory[WriteOfffset] = '0' + (number % 10);
        WriteOfffset--;

        number = number / 10;
    } while (number != 0);

    // NULL Terminate
    Memory[AllocatedMemory] = '\0';

    return Memory;
}