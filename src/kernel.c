#include "vga.h"
#include "gdt.h"
#include "interrupts/idt.h"

void kmain(void);

void kmain(void){
    print("Welcome to OS\r\n");
    initGdt();
    print("GDT is done!\r\n");
    initIdt();
    print("IDT is ready.\r\n");
    // print (1/0);
}