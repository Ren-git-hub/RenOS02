#include "vga.h"
#include "gdt.h"

void kmain(void);

void kmain(void){
    print("Welcome to OS\r\n");
    initGdt();
    print("GDT is done!\r\n");
}