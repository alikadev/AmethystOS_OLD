#ifndef __STRING_H__
#define __STRING_H__

#include "common.h"

char* itoa(int value, char* buffer, int base);
u8    strlen(char* str);
char* strcpy(char* dest, char* src);
char* strcat(char* dest, char* src);
char* strput(char* dest, char  src);
char* strpop(char* src);
i32 strcmp(const char* s1, const char* s2);

#endif