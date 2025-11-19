#include "idt.h"
#include "../stdint.h"
#include "../vga.h"

struct idt_entry_struct idtEntry[256];
struct idt_ptr_struct idtPtr;

void initIdt(){
    idtPtr.base = (uint32_t)&idtEntry;
    idtPtr.limit = sizeof(struct idt_entry_struct) * 256 -1 ;

    setMemory(&idtEntry, 0, sizeof(struct idt_entry_struct) * 256);
    
    //set up PIC chips: 0x20 commands and 0x21 data for maste PIC
    //                  0xA0 commandS  and 0xA1 data for slave PIC

    outPortB(0x20, 0x11);
    outPortB(0xA0, 0x11);

    outPortB(0x21, 0x20);
    outPortB(0xA1, 0x28);

    outPortB(0x21, 0x04);
    outPortB(0xA1, 0x02);

    outPortB(0x21, 0x01);
    outPortB(0xA1,0x01);

    outPortB(0x21, 0x0);
    outPortB(0xA1,0x0);

    //set isr0 to isr31
    for(int i=0; i<32; i++){
    setIdtGate(i, (uint32_t)isr[i], 0x08, 0x8E);
    }

    //ser irq0 to irq15
    for(int j=0; j<16; j++){
    setIdtGate(32+j, (uint32_t)irq[j], 0x08, 0x8E);
    }

    setIdtGate(128, (uint32_t)isr128, 0x08, 0x8E); //system call
    setIdtGate(177, (uint32_t)isr177, 0x08, 0x8E); //system call
};

void setIdtGate(uint32_t num, uint32_t base, uint16_t selector, uint8_t flags){
    idtEntry[num].base_low = base & 0xFFFF;
    idtEntry[num].base_high = (base >> 16) & 0xFFFF;
    idtEntry[num].selector = selector;
    idtEntry[num].always0 = 0;
    idtEntry[num].flags = flags | 0x60;

}

unsigned char* exceptionMessages[]={
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Ino Detected Overflow",
    "Out of Bound",
    "Invalid Opcode",
    "No Coprocessor",
    "Double fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown Interrupt",
    "Coprocessor fault",
    "Alignment fault",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isrHandler(struct IntRegisters* regs) {
    if(regs -> int_no < 32){
        print(exceptionMessages[regs -> int_no]);
        print("\n");
        print("Exception!! system halted \n");
        for(;;);
    }
};

void *irq_routines[16] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

void irq_istall_handler(int irq, void (*handler)(struct IntRegisters *r)){
    irq_routines[irq] = handler;
};

void irq_uninstall_handler(int irq){
    irq_routines[irq]=0;
};

void irq_handler(struct IntRegisters* regs){
    void (*handler)(struct IntRegisters *regs);

    handler = irq_routines[regs -> int_no - 32];

    if(handler){
        handler(regs);
    }

    if(regs -> int_no >= 40){
        outPortB(0xA0, 0x20);
    }

    outPortB(0x20,0x20);
};