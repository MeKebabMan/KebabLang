#ifndef _KEBABLIB_H_
#define _KEBABLIB_H_

// A bunch of cool functions I've made to make my life easier!!!

// START - Tell the compiler to treat this like C
#ifdef __cplusplus
extern "C"
{
#endif

// !!! Includes !!!
#include <stdio.h>
#include <stdint.h>

// !!! Functions !!!

// Stick to using printf() if you do not need thread safety
// Locks the FileStream (Thread safety) & flushes the FileStream
//
// WARNING: PLEASE PROVIDE THE CORRECT DATA TYPES FOR THE VARIADIC ARGUMENTS! THIS FUNCTION WILL HAVE UNDEFINED BEHAVIOUR IF YOU DO NOT!!! Same thing with printf() 
//
// Supported Data Types:
// %zu - size_t
// %d - int
// %c - int 
// %s - char*
//
void KebabPrintf(FILE* __restrict__ FileStream, char* __restrict__ format, ...);

// Stick to using fputs() if you do not need thread safety
// Locks the FileStream (Thread safety) & flushes the FileStream
void KebabPrint(FILE* __restrict__ FileStream, char* __restrict__ content);

// Trims the buffer (Removes every space & tab at the start and end)
// returns NULL on failure
// returns buffer on SUCESSS
char* TrimBuffer(const char* buffer, size_t end);

// Checks if the buffer contains NUMBERS only..
// returns 0 on success AND yes it does contain only numbers
// returns -1 on failure 
// returns -2 if it DOESNT contain ONLY numbers
int IsNumberOnly(const char* buffer, size_t end);

// Checks if the character / letter is a operator
// returns 0 on YES
// returns -1 on NO
int IsOperator(const char character);

// Read & Write functions

// Reads the entire file
char* KebabReadFile(const char* __restrict__ file);

// Writes content into file (Uses fsync to ensure data was written)
int KebabWriteFile(const char* __restrict__ file, char* buffer, size_t bytes);

// Same exact thing as KebabWriteFile(2) but it uses provided file descripter 
int KebabWriteFD(int fd, char* buffer, size_t bytes);

// signed MAX int (Returns char* on success otherwise will return NULL on failure) 
char* ParseIMAX(intmax_t number);

// Unsigned MAX int (Returns char* on success otherwise will return NULL on failure)
char* ParseUMAX(uintmax_t number);

// END - Tell the compiler to treat this like C
#ifdef __cplusplus
}
#endif 


#endif 