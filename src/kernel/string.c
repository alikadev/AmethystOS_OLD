#include "string.h"
#include "math.h"


//  itoa reverse swap
//      SOURCES : https://www.techiedelight.com/implement-itoa-function-in-c/
//  
//


// Function to swap two numbers
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

// Function to reverse `buffer[i…j]`
char* reverse(char buffer[], int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}

// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

u8 strlen(char* str){
    const char *s;
    for(s=str; *s; s++);
    return (s - str);
}

char* strcpy(char* dest, char* in){
    u8 ilen=strlen(in);
    for(int i=0; i<ilen ;i++){
        dest[i]=in[i];
    }
    dest[ilen+1]=0;
    return dest;
}

char* strcat(char* dest, char* in){
    u8 dlen=strlen(dest);
    u8 slen=strlen(in)+dlen;
    for(int i=dlen; i<slen ;i++){
        dest[i]=in[i];
    }
    dest[slen+1]=0;
    return dest;
}

char* strput(char* dest, char  in){
    u8 dlen=strlen(dest);
    dest[dlen+1]=in;
    dest[dlen+2]=0;
    return dest;
}