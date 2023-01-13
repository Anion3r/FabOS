#include "vga.c"

void kernel_main()
{
    vga_init();
    vga_write_string("I can print anything now!\nOops, not everything, but I can print a lot of things!\n");
}
