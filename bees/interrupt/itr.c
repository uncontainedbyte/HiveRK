#include "basicInterrupts.h"


#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define PIC_EOI      0x20
void pic_remap(void) {
	uint8_t a1 = inb(PIC1_DATA);
	uint8_t a2 = inb(PIC2_DATA);
	outb(PIC1_COMMAND, 0x11);
	outb(PIC2_COMMAND, 0x11);
	outb(PIC1_DATA, 0x20); // master offset = 32
	outb(PIC2_DATA, 0x28); // slave offset = 40
	outb(PIC1_DATA, 0x04); // slave on IRQ2
	outb(PIC2_DATA, 0x02);
	outb(PIC1_DATA, 0x01);
	outb(PIC2_DATA, 0x01);
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

typedef struct{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t flags;
	/* Flags:
	 * Bit 7: "Interrupt is present"
	 * Bits 6-5: Privilege level of caller (0=kernel..3=user)
	 * Bit 4: Set to 0 for interrupt gates
	 * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
	uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;
typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;
typedef void (*irq_handler_t)(registers_t *);


typedef struct{
	idt_entry_t idt[256];
	idt_ptr_t   idt_ptr;
	uint32_t handler_functions[256];
} GlobalDataStruct;

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);
extern void isr48(void);
extern void isr49(void);
extern void isr50(void);
extern void isr51(void);
extern void isr52(void);
extern void isr53(void);
extern void isr54(void);
extern void isr55(void);
extern void isr56(void);
extern void isr57(void);
extern void isr58(void);
extern void isr59(void);
extern void isr60(void);
extern void isr61(void);
extern void isr62(void);
extern void isr63(void);
extern void isr64(void);
extern void isr65(void);
extern void isr66(void);
extern void isr67(void);
extern void isr68(void);
extern void isr69(void);
extern void isr70(void);
extern void isr71(void);
extern void isr72(void);
extern void isr73(void);
extern void isr74(void);
extern void isr75(void);
extern void isr76(void);
extern void isr77(void);
extern void isr78(void);
extern void isr79(void);
extern void isr80(void);
extern void isr81(void);
extern void isr82(void);
extern void isr83(void);
extern void isr84(void);
extern void isr85(void);
extern void isr86(void);
extern void isr87(void);
extern void isr88(void);
extern void isr89(void);
extern void isr90(void);
extern void isr91(void);
extern void isr92(void);
extern void isr93(void);
extern void isr94(void);
extern void isr95(void);
extern void isr96(void);
extern void isr97(void);
extern void isr98(void);
extern void isr99(void);
extern void isr100(void);
extern void isr101(void);
extern void isr102(void);
extern void isr103(void);
extern void isr104(void);
extern void isr105(void);
extern void isr106(void);
extern void isr107(void);
extern void isr108(void);
extern void isr109(void);
extern void isr110(void);
extern void isr111(void);
extern void isr112(void);
extern void isr113(void);
extern void isr114(void);
extern void isr115(void);
extern void isr116(void);
extern void isr117(void);
extern void isr118(void);
extern void isr119(void);
extern void isr120(void);
extern void isr121(void);
extern void isr122(void);
extern void isr123(void);
extern void isr124(void);
extern void isr125(void);
extern void isr126(void);
extern void isr127(void);
extern void isr128(void);
extern void isr129(void);
extern void isr130(void);
extern void isr131(void);
extern void isr132(void);
extern void isr133(void);
extern void isr134(void);
extern void isr135(void);
extern void isr136(void);
extern void isr137(void);
extern void isr138(void);
extern void isr139(void);
extern void isr140(void);
extern void isr141(void);
extern void isr142(void);
extern void isr143(void);
extern void isr144(void);
extern void isr145(void);
extern void isr146(void);
extern void isr147(void);
extern void isr148(void);
extern void isr149(void);
extern void isr150(void);
extern void isr151(void);
extern void isr152(void);
extern void isr153(void);
extern void isr154(void);
extern void isr155(void);
extern void isr156(void);
extern void isr157(void);
extern void isr158(void);
extern void isr159(void);
extern void isr160(void);
extern void isr161(void);
extern void isr162(void);
extern void isr163(void);
extern void isr164(void);
extern void isr165(void);
extern void isr166(void);
extern void isr167(void);
extern void isr168(void);
extern void isr169(void);
extern void isr170(void);
extern void isr171(void);
extern void isr172(void);
extern void isr173(void);
extern void isr174(void);
extern void isr175(void);
extern void isr176(void);
extern void isr177(void);
extern void isr178(void);
extern void isr179(void);
extern void isr180(void);
extern void isr181(void);
extern void isr182(void);
extern void isr183(void);
extern void isr184(void);
extern void isr185(void);
extern void isr186(void);
extern void isr187(void);
extern void isr188(void);
extern void isr189(void);
extern void isr190(void);
extern void isr191(void);
extern void isr192(void);
extern void isr193(void);
extern void isr194(void);
extern void isr195(void);
extern void isr196(void);
extern void isr197(void);
extern void isr198(void);
extern void isr199(void);
extern void isr200(void);
extern void isr201(void);
extern void isr202(void);
extern void isr203(void);
extern void isr204(void);
extern void isr205(void);
extern void isr206(void);
extern void isr207(void);
extern void isr208(void);
extern void isr209(void);
extern void isr210(void);
extern void isr211(void);
extern void isr212(void);
extern void isr213(void);
extern void isr214(void);
extern void isr215(void);
extern void isr216(void);
extern void isr217(void);
extern void isr218(void);
extern void isr219(void);
extern void isr220(void);
extern void isr221(void);
extern void isr222(void);
extern void isr223(void);
extern void isr224(void);
extern void isr225(void);
extern void isr226(void);
extern void isr227(void);
extern void isr228(void);
extern void isr229(void);
extern void isr230(void);
extern void isr231(void);
extern void isr232(void);
extern void isr233(void);
extern void isr234(void);
extern void isr235(void);
extern void isr236(void);
extern void isr237(void);
extern void isr238(void);
extern void isr239(void);
extern void isr240(void);
extern void isr241(void);
extern void isr242(void);
extern void isr243(void);
extern void isr244(void);
extern void isr245(void);
extern void isr246(void);
extern void isr247(void);
extern void isr248(void);
extern void isr249(void);
extern void isr250(void);
extern void isr251(void);
extern void isr252(void);
extern void isr253(void);
extern void isr254(void);
extern void isr255(void);

void set_idt_gate(GlobalDataStruct* global,int n, uint32_t handler, uint8_t flags){
	global->idt[n].offset_low  = handler & 0xFFFF;
	global->idt[n].selector    = 0x08;
	global->idt[n].zero        = 0;
	global->idt[n].flags       = flags;
	global->idt[n].offset_high = (handler >> 16) & 0xFFFF;
}
void isr_install(CoreData* core,GlobalDataStruct* global){
	set_idt_gate(global,0, (uint32_t)core->baseAdr+(uint32_t)isr0,systemFlags);
	set_idt_gate(global,1, (uint32_t)core->baseAdr+(uint32_t)isr1,systemFlags);
	set_idt_gate(global,2, (uint32_t)core->baseAdr+(uint32_t)isr2,systemFlags);
	set_idt_gate(global,3, (uint32_t)core->baseAdr+(uint32_t)isr3,systemFlags);
	set_idt_gate(global,4, (uint32_t)core->baseAdr+(uint32_t)isr4,systemFlags);
	set_idt_gate(global,5, (uint32_t)core->baseAdr+(uint32_t)isr5,systemFlags);
	set_idt_gate(global,6, (uint32_t)core->baseAdr+(uint32_t)isr6,systemFlags);
	set_idt_gate(global,7, (uint32_t)core->baseAdr+(uint32_t)isr7,systemFlags);
	set_idt_gate(global,8, (uint32_t)core->baseAdr+(uint32_t)isr8,systemFlags);
	set_idt_gate(global,9, (uint32_t)core->baseAdr+(uint32_t)isr9,systemFlags);
	set_idt_gate(global,10, (uint32_t)core->baseAdr+(uint32_t)isr10,systemFlags);
	set_idt_gate(global,11, (uint32_t)core->baseAdr+(uint32_t)isr11,systemFlags);
	set_idt_gate(global,12, (uint32_t)core->baseAdr+(uint32_t)isr12,systemFlags);
	set_idt_gate(global,13, (uint32_t)core->baseAdr+(uint32_t)isr13,systemFlags);
	set_idt_gate(global,14, (uint32_t)core->baseAdr+(uint32_t)isr14,systemFlags);
	set_idt_gate(global,15, (uint32_t)core->baseAdr+(uint32_t)isr15,systemFlags);
	set_idt_gate(global,16, (uint32_t)core->baseAdr+(uint32_t)isr16,systemFlags);
	set_idt_gate(global,17, (uint32_t)core->baseAdr+(uint32_t)isr17,systemFlags);
	set_idt_gate(global,18, (uint32_t)core->baseAdr+(uint32_t)isr18,systemFlags);
	set_idt_gate(global,19, (uint32_t)core->baseAdr+(uint32_t)isr19,systemFlags);
	set_idt_gate(global,20, (uint32_t)core->baseAdr+(uint32_t)isr20,systemFlags);
	set_idt_gate(global,21, (uint32_t)core->baseAdr+(uint32_t)isr21,systemFlags);
	set_idt_gate(global,22, (uint32_t)core->baseAdr+(uint32_t)isr22,systemFlags);
	set_idt_gate(global,23, (uint32_t)core->baseAdr+(uint32_t)isr23,systemFlags);
	set_idt_gate(global,24, (uint32_t)core->baseAdr+(uint32_t)isr24,systemFlags);
	set_idt_gate(global,25, (uint32_t)core->baseAdr+(uint32_t)isr25,systemFlags);
	set_idt_gate(global,26, (uint32_t)core->baseAdr+(uint32_t)isr26,systemFlags);
	set_idt_gate(global,27, (uint32_t)core->baseAdr+(uint32_t)isr27,systemFlags);
	set_idt_gate(global,28, (uint32_t)core->baseAdr+(uint32_t)isr28,systemFlags);
	set_idt_gate(global,29, (uint32_t)core->baseAdr+(uint32_t)isr29,systemFlags);
	set_idt_gate(global,30, (uint32_t)core->baseAdr+(uint32_t)isr30,systemFlags);
	set_idt_gate(global,31, (uint32_t)core->baseAdr+(uint32_t)isr31,systemFlags);
}
void irq_install(CoreData* core,GlobalDataStruct* global){
	set_idt_gate(global,32, (uint32_t)core->baseAdr+(uint32_t)irq0,systemFlags);
	set_idt_gate(global,33, (uint32_t)core->baseAdr+(uint32_t)irq1,systemFlags);
	set_idt_gate(global,34, (uint32_t)core->baseAdr+(uint32_t)irq2,systemFlags);
	set_idt_gate(global,35, (uint32_t)core->baseAdr+(uint32_t)irq3,systemFlags);
	set_idt_gate(global,36, (uint32_t)core->baseAdr+(uint32_t)irq4,systemFlags);
	set_idt_gate(global,37, (uint32_t)core->baseAdr+(uint32_t)irq5,systemFlags);
	set_idt_gate(global,38, (uint32_t)core->baseAdr+(uint32_t)irq6,systemFlags);
	set_idt_gate(global,39, (uint32_t)core->baseAdr+(uint32_t)irq7,systemFlags);
	set_idt_gate(global,40, (uint32_t)core->baseAdr+(uint32_t)irq8,systemFlags);
	set_idt_gate(global,41, (uint32_t)core->baseAdr+(uint32_t)irq9,systemFlags);
	set_idt_gate(global,42, (uint32_t)core->baseAdr+(uint32_t)irq10,systemFlags);
	set_idt_gate(global,43, (uint32_t)core->baseAdr+(uint32_t)irq11,systemFlags);
	set_idt_gate(global,44, (uint32_t)core->baseAdr+(uint32_t)irq12,systemFlags);
	set_idt_gate(global,45, (uint32_t)core->baseAdr+(uint32_t)irq13,systemFlags);
	set_idt_gate(global,46, (uint32_t)core->baseAdr+(uint32_t)irq14,systemFlags);
	set_idt_gate(global,47, (uint32_t)core->baseAdr+(uint32_t)irq15,systemFlags);
}
void install_extra_isr(CoreData* core,GlobalDataStruct* global){
	set_idt_gate(global,48, (uint32_t)core->baseAdr+(uint32_t)isr48,systemFlags);
	set_idt_gate(global,49,  (uint32_t)core->baseAdr+(uint32_t)isr49,systemFlags);
	set_idt_gate(global,50,  (uint32_t)core->baseAdr+(uint32_t)isr50,systemFlags);
	set_idt_gate(global,51,  (uint32_t)core->baseAdr+(uint32_t)isr51,systemFlags);
	set_idt_gate(global,52,  (uint32_t)core->baseAdr+(uint32_t)isr52,systemFlags);
	set_idt_gate(global,53,  (uint32_t)core->baseAdr+(uint32_t)isr53,systemFlags);
	set_idt_gate(global,54,  (uint32_t)core->baseAdr+(uint32_t)isr54,systemFlags);
	set_idt_gate(global,55,  (uint32_t)core->baseAdr+(uint32_t)isr55,systemFlags);
	set_idt_gate(global,56,  (uint32_t)core->baseAdr+(uint32_t)isr56,systemFlags);
	set_idt_gate(global,57,  (uint32_t)core->baseAdr+(uint32_t)isr57,systemFlags);
	set_idt_gate(global,58,  (uint32_t)core->baseAdr+(uint32_t)isr58,systemFlags);
	set_idt_gate(global,59,  (uint32_t)core->baseAdr+(uint32_t)isr59,systemFlags);
	set_idt_gate(global,60,  (uint32_t)core->baseAdr+(uint32_t)isr60,systemFlags);
	set_idt_gate(global,61,  (uint32_t)core->baseAdr+(uint32_t)isr61,systemFlags);
	set_idt_gate(global,62,  (uint32_t)core->baseAdr+(uint32_t)isr62,systemFlags);
	set_idt_gate(global,63,  (uint32_t)core->baseAdr+(uint32_t)isr63,systemFlags);
	set_idt_gate(global,64,  (uint32_t)core->baseAdr+(uint32_t)isr64,systemFlags);
	set_idt_gate(global,65,  (uint32_t)core->baseAdr+(uint32_t)isr65,systemFlags);
	set_idt_gate(global,66,  (uint32_t)core->baseAdr+(uint32_t)isr66,systemFlags);
	set_idt_gate(global,67,  (uint32_t)core->baseAdr+(uint32_t)isr67,systemFlags);
	set_idt_gate(global,68,  (uint32_t)core->baseAdr+(uint32_t)isr68,systemFlags);
	set_idt_gate(global,69,  (uint32_t)core->baseAdr+(uint32_t)isr69,systemFlags);
	set_idt_gate(global,70,  (uint32_t)core->baseAdr+(uint32_t)isr70,systemFlags);
	set_idt_gate(global,71,  (uint32_t)core->baseAdr+(uint32_t)isr71,systemFlags);
	set_idt_gate(global,72,  (uint32_t)core->baseAdr+(uint32_t)isr72,systemFlags);
	set_idt_gate(global,73,  (uint32_t)core->baseAdr+(uint32_t)isr73,systemFlags);
	set_idt_gate(global,74,  (uint32_t)core->baseAdr+(uint32_t)isr74,systemFlags);
	set_idt_gate(global,75,  (uint32_t)core->baseAdr+(uint32_t)isr75,systemFlags);
	set_idt_gate(global,76,  (uint32_t)core->baseAdr+(uint32_t)isr76,systemFlags);
	set_idt_gate(global,77,  (uint32_t)core->baseAdr+(uint32_t)isr77,systemFlags);
	set_idt_gate(global,78,  (uint32_t)core->baseAdr+(uint32_t)isr78,systemFlags);
	set_idt_gate(global,79,  (uint32_t)core->baseAdr+(uint32_t)isr79,systemFlags);
	set_idt_gate(global,80,  (uint32_t)core->baseAdr+(uint32_t)isr80,systemFlags);
	set_idt_gate(global,81,  (uint32_t)core->baseAdr+(uint32_t)isr81,systemFlags);
	set_idt_gate(global,82,  (uint32_t)core->baseAdr+(uint32_t)isr82,systemFlags);
	set_idt_gate(global,83,  (uint32_t)core->baseAdr+(uint32_t)isr83,systemFlags);
	set_idt_gate(global,84,  (uint32_t)core->baseAdr+(uint32_t)isr84,systemFlags);
	set_idt_gate(global,85,  (uint32_t)core->baseAdr+(uint32_t)isr85,systemFlags);
	set_idt_gate(global,86,  (uint32_t)core->baseAdr+(uint32_t)isr86,systemFlags);
	set_idt_gate(global,87,  (uint32_t)core->baseAdr+(uint32_t)isr87,systemFlags);
	set_idt_gate(global,88,  (uint32_t)core->baseAdr+(uint32_t)isr88,systemFlags);
	set_idt_gate(global,89,  (uint32_t)core->baseAdr+(uint32_t)isr89,systemFlags);
	set_idt_gate(global,90,  (uint32_t)core->baseAdr+(uint32_t)isr90,systemFlags);
	set_idt_gate(global,91,  (uint32_t)core->baseAdr+(uint32_t)isr91,systemFlags);
	set_idt_gate(global,92,  (uint32_t)core->baseAdr+(uint32_t)isr92,systemFlags);
	set_idt_gate(global,93,  (uint32_t)core->baseAdr+(uint32_t)isr93,systemFlags);
	set_idt_gate(global,94,  (uint32_t)core->baseAdr+(uint32_t)isr94,systemFlags);
	set_idt_gate(global,95,  (uint32_t)core->baseAdr+(uint32_t)isr95,systemFlags);
	set_idt_gate(global,96,  (uint32_t)core->baseAdr+(uint32_t)isr96,systemFlags);
	set_idt_gate(global,97,  (uint32_t)core->baseAdr+(uint32_t)isr97,systemFlags);
	set_idt_gate(global,98,  (uint32_t)core->baseAdr+(uint32_t)isr98,systemFlags);
	set_idt_gate(global,99,  (uint32_t)core->baseAdr+(uint32_t)isr99,systemFlags);
	set_idt_gate(global,100, (uint32_t)core->baseAdr+(uint32_t)isr100,systemFlags);
	set_idt_gate(global,101, (uint32_t)core->baseAdr+(uint32_t)isr101,systemFlags);
	set_idt_gate(global,102, (uint32_t)core->baseAdr+(uint32_t)isr102,systemFlags);
	set_idt_gate(global,103, (uint32_t)core->baseAdr+(uint32_t)isr103,systemFlags);
	set_idt_gate(global,104, (uint32_t)core->baseAdr+(uint32_t)isr104,systemFlags);
	set_idt_gate(global,105, (uint32_t)core->baseAdr+(uint32_t)isr105,systemFlags);
	set_idt_gate(global,106, (uint32_t)core->baseAdr+(uint32_t)isr106,systemFlags);
	set_idt_gate(global,107, (uint32_t)core->baseAdr+(uint32_t)isr107,systemFlags);
	set_idt_gate(global,108, (uint32_t)core->baseAdr+(uint32_t)isr108,systemFlags);
	set_idt_gate(global,109, (uint32_t)core->baseAdr+(uint32_t)isr109,systemFlags);
	set_idt_gate(global,110, (uint32_t)core->baseAdr+(uint32_t)isr110,systemFlags);
	set_idt_gate(global,111, (uint32_t)core->baseAdr+(uint32_t)isr111,systemFlags);
	set_idt_gate(global,112, (uint32_t)core->baseAdr+(uint32_t)isr112,systemFlags);
	set_idt_gate(global,113, (uint32_t)core->baseAdr+(uint32_t)isr113,systemFlags);
	set_idt_gate(global,114, (uint32_t)core->baseAdr+(uint32_t)isr114,systemFlags);
	set_idt_gate(global,115, (uint32_t)core->baseAdr+(uint32_t)isr115,systemFlags);
	set_idt_gate(global,116, (uint32_t)core->baseAdr+(uint32_t)isr116,systemFlags);
	set_idt_gate(global,117, (uint32_t)core->baseAdr+(uint32_t)isr117,systemFlags);
	set_idt_gate(global,118, (uint32_t)core->baseAdr+(uint32_t)isr118,systemFlags);
	set_idt_gate(global,119, (uint32_t)core->baseAdr+(uint32_t)isr119,systemFlags);
	set_idt_gate(global,120, (uint32_t)core->baseAdr+(uint32_t)isr120,systemFlags);
	set_idt_gate(global,121, (uint32_t)core->baseAdr+(uint32_t)isr121,systemFlags);
	set_idt_gate(global,122, (uint32_t)core->baseAdr+(uint32_t)isr122,systemFlags);
	set_idt_gate(global,123, (uint32_t)core->baseAdr+(uint32_t)isr123,systemFlags);
	set_idt_gate(global,124, (uint32_t)core->baseAdr+(uint32_t)isr124,systemFlags);
	set_idt_gate(global,125, (uint32_t)core->baseAdr+(uint32_t)isr125,systemFlags);
	set_idt_gate(global,126, (uint32_t)core->baseAdr+(uint32_t)isr126,systemFlags);
	set_idt_gate(global,127, (uint32_t)core->baseAdr+(uint32_t)isr127,systemFlags);
	set_idt_gate(global,128, (uint32_t)core->baseAdr+(uint32_t)isr128,systemFlags);
	set_idt_gate(global,129, (uint32_t)core->baseAdr+(uint32_t)isr129,systemFlags);
	set_idt_gate(global,130, (uint32_t)core->baseAdr+(uint32_t)isr130,systemFlags);
	set_idt_gate(global,131, (uint32_t)core->baseAdr+(uint32_t)isr131,systemFlags);
	set_idt_gate(global,132, (uint32_t)core->baseAdr+(uint32_t)isr132,systemFlags);
	set_idt_gate(global,133, (uint32_t)core->baseAdr+(uint32_t)isr133,systemFlags);
	set_idt_gate(global,134, (uint32_t)core->baseAdr+(uint32_t)isr134,systemFlags);
	set_idt_gate(global,135, (uint32_t)core->baseAdr+(uint32_t)isr135,systemFlags);
	set_idt_gate(global,136, (uint32_t)core->baseAdr+(uint32_t)isr136,systemFlags);
	set_idt_gate(global,137, (uint32_t)core->baseAdr+(uint32_t)isr137,systemFlags);
	set_idt_gate(global,138, (uint32_t)core->baseAdr+(uint32_t)isr138,systemFlags);
	set_idt_gate(global,139, (uint32_t)core->baseAdr+(uint32_t)isr139,systemFlags);
	set_idt_gate(global,140, (uint32_t)core->baseAdr+(uint32_t)isr140,systemFlags);
	set_idt_gate(global,141, (uint32_t)core->baseAdr+(uint32_t)isr141,systemFlags);
	set_idt_gate(global,142, (uint32_t)core->baseAdr+(uint32_t)isr142,systemFlags);
	set_idt_gate(global,143, (uint32_t)core->baseAdr+(uint32_t)isr143,systemFlags);
	set_idt_gate(global,144, (uint32_t)core->baseAdr+(uint32_t)isr144,systemFlags);
	set_idt_gate(global,145, (uint32_t)core->baseAdr+(uint32_t)isr145,systemFlags);
	set_idt_gate(global,146, (uint32_t)core->baseAdr+(uint32_t)isr146,systemFlags);
	set_idt_gate(global,147, (uint32_t)core->baseAdr+(uint32_t)isr147,systemFlags);
	set_idt_gate(global,148, (uint32_t)core->baseAdr+(uint32_t)isr148,systemFlags);
	set_idt_gate(global,149, (uint32_t)core->baseAdr+(uint32_t)isr149,systemFlags);
	set_idt_gate(global,150, (uint32_t)core->baseAdr+(uint32_t)isr150,systemFlags);
	set_idt_gate(global,151, (uint32_t)core->baseAdr+(uint32_t)isr151,systemFlags);
	set_idt_gate(global,152, (uint32_t)core->baseAdr+(uint32_t)isr152,systemFlags);
	set_idt_gate(global,153, (uint32_t)core->baseAdr+(uint32_t)isr153,systemFlags);
	set_idt_gate(global,154, (uint32_t)core->baseAdr+(uint32_t)isr154,systemFlags);
	set_idt_gate(global,155, (uint32_t)core->baseAdr+(uint32_t)isr155,systemFlags);
	set_idt_gate(global,156, (uint32_t)core->baseAdr+(uint32_t)isr156,systemFlags);
	set_idt_gate(global,157, (uint32_t)core->baseAdr+(uint32_t)isr157,systemFlags);
	set_idt_gate(global,158, (uint32_t)core->baseAdr+(uint32_t)isr158,systemFlags);
	set_idt_gate(global,159, (uint32_t)core->baseAdr+(uint32_t)isr159,systemFlags);
	set_idt_gate(global,160, (uint32_t)core->baseAdr+(uint32_t)isr160,systemFlags);
	set_idt_gate(global,161, (uint32_t)core->baseAdr+(uint32_t)isr161,systemFlags);
	set_idt_gate(global,162, (uint32_t)core->baseAdr+(uint32_t)isr162,systemFlags);
	set_idt_gate(global,163, (uint32_t)core->baseAdr+(uint32_t)isr163,systemFlags);
	set_idt_gate(global,164, (uint32_t)core->baseAdr+(uint32_t)isr164,systemFlags);
	set_idt_gate(global,165, (uint32_t)core->baseAdr+(uint32_t)isr165,systemFlags);
	set_idt_gate(global,166, (uint32_t)core->baseAdr+(uint32_t)isr166,systemFlags);
	set_idt_gate(global,167, (uint32_t)core->baseAdr+(uint32_t)isr167,systemFlags);
	set_idt_gate(global,168, (uint32_t)core->baseAdr+(uint32_t)isr168,systemFlags);
	set_idt_gate(global,169, (uint32_t)core->baseAdr+(uint32_t)isr169,systemFlags);
	set_idt_gate(global,170, (uint32_t)core->baseAdr+(uint32_t)isr170,systemFlags);
	set_idt_gate(global,171, (uint32_t)core->baseAdr+(uint32_t)isr171,systemFlags);
	set_idt_gate(global,172, (uint32_t)core->baseAdr+(uint32_t)isr172,systemFlags);
	set_idt_gate(global,173, (uint32_t)core->baseAdr+(uint32_t)isr173,systemFlags);
	set_idt_gate(global,174, (uint32_t)core->baseAdr+(uint32_t)isr174,systemFlags);
	set_idt_gate(global,175, (uint32_t)core->baseAdr+(uint32_t)isr175,systemFlags);
	set_idt_gate(global,176, (uint32_t)core->baseAdr+(uint32_t)isr176,systemFlags);
	set_idt_gate(global,177, (uint32_t)core->baseAdr+(uint32_t)isr177,systemFlags);
	set_idt_gate(global,178, (uint32_t)core->baseAdr+(uint32_t)isr178,systemFlags);
	set_idt_gate(global,179, (uint32_t)core->baseAdr+(uint32_t)isr179,systemFlags);
	set_idt_gate(global,180, (uint32_t)core->baseAdr+(uint32_t)isr180,systemFlags);
	set_idt_gate(global,181, (uint32_t)core->baseAdr+(uint32_t)isr181,systemFlags);
	set_idt_gate(global,182, (uint32_t)core->baseAdr+(uint32_t)isr182,systemFlags);
	set_idt_gate(global,183, (uint32_t)core->baseAdr+(uint32_t)isr183,systemFlags);
	set_idt_gate(global,184, (uint32_t)core->baseAdr+(uint32_t)isr184,systemFlags);
	set_idt_gate(global,185, (uint32_t)core->baseAdr+(uint32_t)isr185,systemFlags);
	set_idt_gate(global,186, (uint32_t)core->baseAdr+(uint32_t)isr186,systemFlags);
	set_idt_gate(global,187, (uint32_t)core->baseAdr+(uint32_t)isr187,systemFlags);
	set_idt_gate(global,188, (uint32_t)core->baseAdr+(uint32_t)isr188,systemFlags);
	set_idt_gate(global,189, (uint32_t)core->baseAdr+(uint32_t)isr189,systemFlags);
	set_idt_gate(global,190, (uint32_t)core->baseAdr+(uint32_t)isr190,systemFlags);
	set_idt_gate(global,191, (uint32_t)core->baseAdr+(uint32_t)isr191,systemFlags);
	set_idt_gate(global,192, (uint32_t)core->baseAdr+(uint32_t)isr192,systemFlags);
	set_idt_gate(global,193, (uint32_t)core->baseAdr+(uint32_t)isr193,systemFlags);
	set_idt_gate(global,194, (uint32_t)core->baseAdr+(uint32_t)isr194,systemFlags);
	set_idt_gate(global,195, (uint32_t)core->baseAdr+(uint32_t)isr195,systemFlags);
	set_idt_gate(global,196, (uint32_t)core->baseAdr+(uint32_t)isr196,systemFlags);
	set_idt_gate(global,197, (uint32_t)core->baseAdr+(uint32_t)isr197,systemFlags);
	set_idt_gate(global,198, (uint32_t)core->baseAdr+(uint32_t)isr198,systemFlags);
	set_idt_gate(global,199, (uint32_t)core->baseAdr+(uint32_t)isr199,systemFlags);
	set_idt_gate(global,200, (uint32_t)core->baseAdr+(uint32_t)isr200,systemFlags);
	set_idt_gate(global,201, (uint32_t)core->baseAdr+(uint32_t)isr201,systemFlags);
	set_idt_gate(global,202, (uint32_t)core->baseAdr+(uint32_t)isr202,systemFlags);
	set_idt_gate(global,203, (uint32_t)core->baseAdr+(uint32_t)isr203,systemFlags);
	set_idt_gate(global,204, (uint32_t)core->baseAdr+(uint32_t)isr204,systemFlags);
	set_idt_gate(global,205, (uint32_t)core->baseAdr+(uint32_t)isr205,systemFlags);
	set_idt_gate(global,206, (uint32_t)core->baseAdr+(uint32_t)isr206,systemFlags);
	set_idt_gate(global,207, (uint32_t)core->baseAdr+(uint32_t)isr207,systemFlags);
	set_idt_gate(global,208, (uint32_t)core->baseAdr+(uint32_t)isr208,systemFlags);
	set_idt_gate(global,209, (uint32_t)core->baseAdr+(uint32_t)isr209,systemFlags);
	set_idt_gate(global,210, (uint32_t)core->baseAdr+(uint32_t)isr210,systemFlags);
	set_idt_gate(global,211, (uint32_t)core->baseAdr+(uint32_t)isr211,systemFlags);
	set_idt_gate(global,212, (uint32_t)core->baseAdr+(uint32_t)isr212,systemFlags);
	set_idt_gate(global,213, (uint32_t)core->baseAdr+(uint32_t)isr213,systemFlags);
	set_idt_gate(global,214, (uint32_t)core->baseAdr+(uint32_t)isr214,systemFlags);
	set_idt_gate(global,215, (uint32_t)core->baseAdr+(uint32_t)isr215,systemFlags);
	set_idt_gate(global,216, (uint32_t)core->baseAdr+(uint32_t)isr216,systemFlags);
	set_idt_gate(global,217, (uint32_t)core->baseAdr+(uint32_t)isr217,systemFlags);
	set_idt_gate(global,218, (uint32_t)core->baseAdr+(uint32_t)isr218,systemFlags);
	set_idt_gate(global,219, (uint32_t)core->baseAdr+(uint32_t)isr219,systemFlags);
	set_idt_gate(global,220, (uint32_t)core->baseAdr+(uint32_t)isr220,systemFlags);
	set_idt_gate(global,221, (uint32_t)core->baseAdr+(uint32_t)isr221,systemFlags);
	set_idt_gate(global,222, (uint32_t)core->baseAdr+(uint32_t)isr222,systemFlags);
	set_idt_gate(global,223, (uint32_t)core->baseAdr+(uint32_t)isr223,systemFlags);
	set_idt_gate(global,224, (uint32_t)core->baseAdr+(uint32_t)isr224,systemFlags);
	set_idt_gate(global,225, (uint32_t)core->baseAdr+(uint32_t)isr225,systemFlags);
	set_idt_gate(global,226, (uint32_t)core->baseAdr+(uint32_t)isr226,systemFlags);
	set_idt_gate(global,227, (uint32_t)core->baseAdr+(uint32_t)isr227,systemFlags);
	set_idt_gate(global,228, (uint32_t)core->baseAdr+(uint32_t)isr228,systemFlags);
	set_idt_gate(global,229, (uint32_t)core->baseAdr+(uint32_t)isr229,systemFlags);
	set_idt_gate(global,230, (uint32_t)core->baseAdr+(uint32_t)isr230,systemFlags);
	set_idt_gate(global,231, (uint32_t)core->baseAdr+(uint32_t)isr231,systemFlags);
	set_idt_gate(global,232, (uint32_t)core->baseAdr+(uint32_t)isr232,systemFlags);
	set_idt_gate(global,233, (uint32_t)core->baseAdr+(uint32_t)isr233,systemFlags);
	set_idt_gate(global,234, (uint32_t)core->baseAdr+(uint32_t)isr234,systemFlags);
	set_idt_gate(global,235, (uint32_t)core->baseAdr+(uint32_t)isr235,systemFlags);
	set_idt_gate(global,236, (uint32_t)core->baseAdr+(uint32_t)isr236,systemFlags);
	set_idt_gate(global,237, (uint32_t)core->baseAdr+(uint32_t)isr237,systemFlags);
	set_idt_gate(global,238, (uint32_t)core->baseAdr+(uint32_t)isr238,systemFlags);
	set_idt_gate(global,239, (uint32_t)core->baseAdr+(uint32_t)isr239,systemFlags);
	set_idt_gate(global,240, (uint32_t)core->baseAdr+(uint32_t)isr240,systemFlags);
	set_idt_gate(global,241, (uint32_t)core->baseAdr+(uint32_t)isr241,systemFlags);
	set_idt_gate(global,242, (uint32_t)core->baseAdr+(uint32_t)isr242,systemFlags);
	set_idt_gate(global,243, (uint32_t)core->baseAdr+(uint32_t)isr243,systemFlags);
	set_idt_gate(global,244, (uint32_t)core->baseAdr+(uint32_t)isr244,systemFlags);
	set_idt_gate(global,245, (uint32_t)core->baseAdr+(uint32_t)isr245,systemFlags);
	set_idt_gate(global,246, (uint32_t)core->baseAdr+(uint32_t)isr246,systemFlags);
	set_idt_gate(global,247, (uint32_t)core->baseAdr+(uint32_t)isr247,systemFlags);
	set_idt_gate(global,248, (uint32_t)core->baseAdr+(uint32_t)isr248,systemFlags);
	set_idt_gate(global,249, (uint32_t)core->baseAdr+(uint32_t)isr249,systemFlags);
	set_idt_gate(global,250, (uint32_t)core->baseAdr+(uint32_t)isr250,systemFlags);
	set_idt_gate(global,251, (uint32_t)core->baseAdr+(uint32_t)isr251,systemFlags);
	set_idt_gate(global,252, (uint32_t)core->baseAdr+(uint32_t)isr252,systemFlags);
	set_idt_gate(global,253, (uint32_t)core->baseAdr+(uint32_t)isr253,systemFlags);
	set_idt_gate(global,254, (uint32_t)core->baseAdr+(uint32_t)isr254,systemFlags);
	set_idt_gate(global,255, (uint32_t)core->baseAdr+(uint32_t)isr255,systemFlags);
}
void pic_mask_all(void) {
	outb(0x21, 0xFF); // master
	outb(0xA1, 0xFF); // slave
}
void pic_unmask_irq(uint8_t irq) {
	uint16_t port = (irq < 8) ? 0x21 : 0xA1;
	uint8_t  mask = inb(port) & ~(1 << (irq % 8));
	outb(port, mask);
}
void disable_interrupts(){
	asm volatile("cli");
}
void enable_interrupts(){
	asm volatile("sti");
}
void idt_init(CoreData* core,GlobalDataStruct* global){
	disable_interrupts();
	
	for(int i = 0; i < 256; i++){
		global->idt[i] = (idt_entry_t){0};
		global->handler_functions[i] = 0;
	}
	
	isr_install(core,global);
	pic_remap();
	irq_install(core,global);
	install_extra_isr(core,global);
	
	global->idt_ptr.base = (uint32_t) &global->idt;
	global->idt_ptr.limit = 256 * sizeof(idt_entry_t) - 1;
	asm volatile("lidt (%0)" : : "r" (&global->idt_ptr));
	
	pic_mask_all();
}

void UNHANDLED_INTERRUPT(int i){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000;s++){
		vidmem[s] = 0;
	}
	char b[29];
	b[0]='!';b[1]=' ';b[2]='U';b[3]='n';b[4]='h';b[5]='a';b[6]='n';b[7]='d';b[8]='l';b[9]='e';
	b[10]='d';b[11]=' ';b[12]='I';b[13]='n';b[14]='t';b[15]='e';b[16]='r';b[17]='r';b[18]='u';
	b[19]='p';b[20]='t';
	b[21]=':';
	b[22]=' ';
	if(i==0){
		b[23]='0';
		b[24]=' ';
		b[25]='!';
		b[26]='\0';
	}else if(i<10){
		b[23]=i+'0';
		b[24]=' ';
		b[25]='!';
		b[26]='\0';
	}else if(i<100){
		b[23]=((i/10)%10)+'0';
		b[24]=(i%10)+'0';
		b[25]=' ';
		b[26]='!';
		b[27]='\0';
	}else{
		b[23]=((i/100)%10)+'0';
		b[24]=((i/10)%10)+'0';
		b[25]=(i%10)+'0';
		b[26]=' ';
		b[27]='!';
		b[28]='\0';
	}
	SYSprint(b,0,0,0x40);
	asm volatile("hlt");
}
void isr_handler(registers_t *r){
	GlobalDataStruct* global;
	CoreData* core;
	uint32_t irt = getCoreIRT();
	syscall(irt,0,2,4,(u32)&global);
	syscall(irt,0,3,4,(u32)&core);
	
	uint32_t handler = global->handler_functions[r->int_no];
	if(handler==0){ UNHANDLED_INTERRUPT(r->int_no); }
	
	asm volatile(
			"pushl %0\n\t"
			"call *%1\n\t"
			"addl $4, %%esp"
			:: "r"((uint32_t)r), "r"(handler)
			:"eax", "ebx", "ecx", "edx", "memory");
	
	if(31 < r->int_no && r->int_no < 48){
		if(r->int_no >= 40) outb(PIC2_COMMAND, PIC_EOI);
		outb(PIC1_COMMAND, PIC_EOI);
	}
}

void bee_init(CoreData* core){
	syscall(core->irt,0,1,4,sizeof(GlobalDataStruct));
	GlobalDataStruct* global;
	syscall(core->irt,0,2,4,(u32)&global);
	
	idt_init(core,global);
	
	enable_interrupts();
	
	registerBASIC(core);
}

void registerInterrupt(CoreData* core,GlobalDataStruct* global){
	disable_interrupts();
	
	u32* args = (u32*)core->ptr;
	u32 id = args[0];
	u8 flags = args[1];
	u32 handler = args[2];
	
	global->handler_functions[id] = handler;
	global->idt[id].flags = flags;
	
	enable_interrupts();
}



























