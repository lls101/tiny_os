#include "common.h"
void putchar(char ch);

void printf(const char* fmt,...){
    va_list vargs;
    va_start(vargs, fmt);

    while(*fmt){
        if(*fmt=='%'){
            fmt++;
            switch(*fmt){
                case '\0':{
                    putchar('%');
                    goto end;
                }
                case '%':{
                    putchar('%');
                    break;
                }

                case 's':{
                    const char *s= va_arg(vargs,const char* );
                    while(*s){
                        putchar(*s);
                        s++;
                    }
                    break;
                }

                case 'd':{
                    int i = va_arg(vargs,int);
                    if(i<0){
                        putchar('-');
                        i=-i;
                    }
                    
                    int divisor = 1;
                    while(i/divisor>9){
                        
                        divisor *=10;
                    }
                    while (divisor>0){
                        putchar('0'+i/divisor);
                        i %= divisor;
                        divisor /=10;
                    }
                    break;
                }

                case 'x':{
                    int value = va_arg(vargs, int);
                    for (int i = 7; i >= 0; i--) {
                        int nibble = (value >> (i * 4)) & 0xf;
                        putchar("0123456789abcdef"[nibble]);
                    }
                }
            }

        }else{
            putchar(*fmt);
        }

        fmt++;

    }
    end:
        va_end(vargs);
}