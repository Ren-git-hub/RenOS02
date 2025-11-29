#include "vga.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"
#include "timer/timer.h"
#include "keyboard/keyboard.h"

void kmain(void);

void kmain(void){
    print("Welcome to OS\r\n");
    initGdt();
    print("GDT is done!\r\n");
    initIdt();
    print("IDT is ready.\r\n");
    initKeyboard();
    for(;;); 

}