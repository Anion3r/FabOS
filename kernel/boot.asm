.intel_syntax noprefix
.set    MAGIC,       0x1BADB002
.set    ALIGN_4K,    0x01            // flags[0] = 1
.set    MEM_INFO,    0x01 << 1       // flags[1] = 1
.set    FLAGS,       ALIGN_4K | MEM_INFO    // flags[2] 到 flags[31] = 0
.set    CHECKSUM,    -(MAGIC + FLAGS)

// 设置 Multiboot 头部信息
.section .multiboot
.align 0x04
.long MAGIC
.long FLAGS
.long CHECKSUM


// 设置 .bss 段作为堆栈
.section .bss
.align 16
stack_bottom:
    .skip 32768     // 32 * 1024 = 32KB
stack_top:


.section .text
.global _start
.type _start, @function
_start:
    // 我们需要立即设置堆栈，否则无法使用用C语言写的函数
    mov esp, stack_top
    // 然后应该设置 GDTR 和 IDTR等信息，但我们暂时可以先不管
    /* GTDR */
    /* IDTR */
    /* 然后就需要我们调用内核的main函数，从这个函数开始，我们基本上就可以使用C语言写了
     * 但是需要注意的是，x86系统的堆栈需要16字节对齐，所以如果我们在前面push了16字节的整数倍，那么可以直接调用
     * 如果不是16字节的整数倍，那我们应当先对齐，然后再调用
     * 这里我们因为没有push任何东西，所以直接调用即可
     */
    call kernel_main

    /* 如果系统没有其他的事情可做（比如因为什么原因从 kernel_main跳出来了）
     * 我们应该使系统进入死循环，用hlt指令空转即可
     */
    cli
halt:
    hlt
    jmp halt

/* 为了方便我们调试和追踪程序，我们需要在这里设置一下 _kernel_start 的大小, '.' 代表当前位置，也就是 _kernel_start 的结束位置
 * 相当于当前位置减去函数开始位置
 */
.size _start, . - _start