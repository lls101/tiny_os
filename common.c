#include "common.h"
void putchar(char ch);

void *memcopy(void *dest,const void *src,size_t n){
    uint8_t *d=(uint8_t*)dest;
    const uint8_t *s=(const uint8_t*)src;
    while(n--){
        *d++ = *s++;
    }
    return dest;
}

void *memset(void *buf, char c,size_t n){
    uint8_t *p = (uint8_t *)buf;
    while(n--){
        *p++ = c;
    }
    return buf;
}


char *strcopy(char *dest,const char *src){
    char* d=dest;
    while(*src){
        *d++ = *src++;
    }
    *d='\0';
    return dest;
}


int strcmp(const char *str1,const char *str2){
    while(*str1 && *str2){
        if(*str1 != *str2){
            break;
        }
        str1++;
        str2++;
    }
    return *(unsigned char*)str1-*(unsigned char*)str2;
}

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