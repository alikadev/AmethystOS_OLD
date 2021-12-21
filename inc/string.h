#ifndef __STRING_H__
#define __STRING_H__

#include "common.h"

char* itoa(int value, char* buffer, int base);
u8    strlen(char* str);
char* strcpy(char* dest, char* in);
char* strcat(char* dest, char* in);
char* strput(char* dest, char  in);

#endif