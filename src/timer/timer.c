#include "../stdint.h"
#include "../interrupts/idt.h"
#include "../vga.h"
#include "timer.h"

uint64_t ticks;
const uint32_t freq = 100;

void onIrq0(struct IntRegisters *regs){
    ticks++;
    print("Timer ticked!");
};

void initTimer(){
    ticks = 0;
    irq_install_handler(0,&onIrq0);

    // timer ticks on 1.1931816666 MHz frequency 
    uint32_t divisor = 1193180 / freq;

    // 0x43 - mode/ command register ; 0011 0110 ( 8 bits )
    outPortB(0x43,0x36);
    outPortB(0x40,(uint8_t)(divisor & 0xFF));
    outPortB(0x40, (uint8_t)((divisor >> 8) & 0xFF));

};