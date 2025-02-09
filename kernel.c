#include "kernel.h"
#include "common.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

#define PANIC(fmt,...) \
    do{ \
        printf("Kernel panic at %s:%d: " fmt "\n", __FILE__,__LINE__, ##__VA_ARGS__);\ 
        while(1){} \
    }while(0)

extern char __bss[], __bss_end[], __stack_top[];

void putchar(char ch);
/**
 * @brief Performs an SBI (Supervisor Binary Interface) call.
 *
 * This function makes an SBI call with the provided arguments and returns the result.
 * The SBI call is made using the `ecall` instruction, which is a synchronous trap
 * that allows the supervisor mode to request services from the lower privilege levels.
 *
 * @param arg0 The first argument to pass to the SBI call.
 * @param arg1 The second argument to pass to the SBI call.
 * @param arg2 The third argument to pass to the SBI call.
 * @param arg3 The fourth argument to pass to the SBI call.
 * @param arg4 The fifth argument to pass to the SBI call.
 * @param arg5 The sixth argument to pass to the SBI call.
 * @param fid The function ID to specify the SBI function to call.
 * @param eid The extension ID to specify the SBI extension to call.
 * @return A struct sbiret containing the result of the SBI call.
 *         - .error: The error code returned by the SBI call.
 *         - .value: The value returned by the SBI call.
 */
struct sbiret   sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid){
    register long a0 __asm__("a0") = arg0;
    register long a1 __asm__("a1") = arg1;
    register long a2 __asm__("a2") = arg2;
    register long a3 __asm__("a3") = arg3;
    register long a4 __asm__("a4") = arg4;
    register long a5 __asm__("a5") = arg5;
    register long a6 __asm__("a6") = fid;
    register long a7 __asm__("a7") = eid;

    __asm__ __volatile__("ecall"
                         : "=r"(a0),"=r"(a1)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                           "r"(a6), "r"(a7)
                         : "memory");

    return (struct sbiret){
        .error = a0,
        .value = a1,
    };
}


void putchar(char ch){
    sbi_call(ch,0,0,0,0,0,0,1);
}



void kernel_main(void)
{
    // printf("\n\nHello %s\n", "World!");
    // printf("1 + 2 = %d, %x\n", 1 + 2,65456);
    // for(;;){
    //     __asm__ __volatile__ ("wfi");
    // }

    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    PANIC("booted!");
    printf("unreachable here!\n");
}



__attribute__((section(".text.boot")))
__attribute__((naked)) 
void boot(void)
{
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" // 设置栈指针
        "j kernel_main\n"       // 跳转到内核主函数
        :
        : [stack_top] "r"(__stack_top) // 将栈顶地址作为 %[stack_top] 传递
    );
}