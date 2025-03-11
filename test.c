#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  // 为了使用 sleep

// 全局变量保持不变
int global_init = 10;     // .data
int global_uninit;        // .bss
const int global_const = 20; // .rodata

// 添加线程局部存储变量
__thread int tls_data = 100;    // 会在 .tdata
__thread int tls_bss;           // 会在 .tbss

// 线程函数的参数结构体
struct thread_args {
    int thread_id;
    int thread_local;    
};

void* thread_function(void* arg) {
    struct thread_args* args = (struct thread_args*)arg;
    int local = 30 + args->thread_id;      
    static int static_init = 40;  
    static int static_uninit;     
    char *heap_mem = malloc(1024); 

    printf("\nThread %d:\n", args->thread_id);
    printf(".text   (thread_function): %p\n", thread_function);
    printf(".rodata (global_const): %p\n", &global_const);
    printf(".data   (global_init): %p\n", &global_init);
    printf(".data   (static_init): %p\n", &static_init);
    printf(".bss    (global_uninit): %p\n", &global_uninit);
    printf(".bss    (static_uninit): %p\n", &static_uninit);
    printf("Heap    (heap_mem): %p\n", heap_mem);
    printf("Stack   (local): %p\n", &local);
    printf("TLS     (tls_data): %p\n", &tls_data);
    printf("TLS     (tls_bss): %p\n", &tls_bss);
    
    // 修改TLS值以验证每个线程的独立性
    tls_data += args->thread_id;
    tls_bss = args->thread_id * 10;
    printf("TLS data value: %d\n", tls_data);
    printf("TLS bss value: %d\n", tls_bss);

    // 打印进程映射
    char cmd[100];
    printf("\nThread %d memory mapping:\n", args->thread_id);
    sprintf(cmd, "cat /proc/%d/maps", getpid());
    system(cmd);
    
    while (1) {
        sleep(1);  // 避免 CPU 占用过高
    }

    free(heap_mem);
    return NULL;
}

int main() {
    pthread_t threads[6];
    struct thread_args args[6];

    // 创建6个线程
    for (int i = 0; i < 6; i++) {
        args[i].thread_id = i;
        args[i].thread_local = i * 100;
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            perror("pthread_create failed");
            return 1;
        }
        // 添加短暂延时，让输出更清晰
        usleep(100000);  // 100ms
    }

    // 主线程的变量
    int local = 30;      
    static int static_init = 40;  
    static int static_uninit;     
    char *heap_mem = malloc(1024); 

    printf("\nMain thread:\n");
    printf(".text   (main): %p\n", main);
    printf(".rodata (global_const): %p\n", &global_const);
    printf(".data   (global_init): %p\n", &global_init);
    printf(".data   (static_init): %p\n", &static_init);
    printf(".bss    (global_uninit): %p\n", &global_uninit);
    printf(".bss    (static_uninit): %p\n", &static_uninit);
    printf("Heap    (heap_mem): %p\n", heap_mem);
    printf("Stack   (local): %p\n", &local);
    printf("TLS     (tls_data): %p\n", &tls_data);
    printf("TLS     (tls_bss): %p\n", &tls_bss);

    // 打印主线程的进程映射
    printf("\nMain thread memory mapping:\n");
    system("cat /proc/$$/maps");

    
    for (int i = 0; i < 6; i++) {
        pthread_join(threads[i], NULL);
    }
    while (1) {
        sleep(1);
    }

    free(heap_mem);
    return 0;
}