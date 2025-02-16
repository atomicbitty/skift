
#include "arch/x86/IDT.h"

extern uintptr_t __interrupt_vector[];

IDTEntry idt[IDT_ENTRY_COUNT];

IDTDescriptor idt_descriptor = {
    .size = sizeof(IDTEntry) * IDT_ENTRY_COUNT,
    .offset = (uint32_t)&idt[0],
};

void idt_initialize()
{
    for (int i = 0; i < 32; i++)
    {
        idt[i] = IDT_ENTRY(__interrupt_vector[i], 0x08, INTGATE);
    }

    for (int i = 32; i < 48; i++)
    {
        idt[i] = IDT_ENTRY(__interrupt_vector[i], 0x08, INTGATE);
    }

    idt[127] = IDT_ENTRY(__interrupt_vector[48], 0x08, INTGATE);
    idt[128] = IDT_ENTRY(__interrupt_vector[49], 0x08, TRAPGATE);

    idt_flush((uint32_t)&idt_descriptor);
}
