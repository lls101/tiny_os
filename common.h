#pragma once
/**
 * @file common.h
 * @brief This header file defines macros for handling variable argument lists.
 *
 * The macros defined in this file are wrappers around the built-in GCC macros
 * for handling variable argument lists. These macros are used to declare a 
 * variable argument list, initialize it, retrieve arguments from it, and clean 
 * it up.
 *
 * Macros:
 * - va_list: Defines a type for variable argument lists.
 * - va_start: Initializes a variable argument list.
 * - va_arg: Retrieves the next argument in the variable argument list.
 * - va_end: Cleans up the variable argument list.
 */
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end


typedef  int bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;



#define true 1
#define false 0
#define NULL ((void *)0)
#define align_up(value, align)  __builtin_align_up(value, align)
#define is_aligned(value,align) __builtin_is_aligned(value,align)
#define offsetof(type,member) __builtin_offsetof(type,member)


void *memset(void *buf,char c,size_t n);
void *memcopy(void* dest,const void* src,size_t n);
char *strcopy(char *dest,const char *src);
int strcmp(const char *str1,const char *str2);
void printf(const char *format, ...);