#include "../beeUtil.h"



#define PIT_CHANNEL0_DATA 0x40
#define PIT_COMMAND       0x43
#define PIT_FREQUENCY     1193182  // PIT input clock in Hz
void pic_unmask_irq(uint8_t irq) {
	uint16_t port = (irq < 8) ? 0x21 : 0xA1;
	uint8_t  mask = inb(port) & ~(1 << (irq % 8));
	outb(port, mask);
}


typedef struct{
	uint32_t ticks;
	void (*tick_handler)(void);
	uint32_t freq_hz;
} GlobalData;


typedef struct {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers_t;
void interrupt_handler(registers_t *r) {
	GlobalData* global;
	uint32_t irt = getCoreIRT();
	syscall(irt,0,2,6,(u32)&global);
	
	
	global->ticks++;
	
	if(global->tick_handler)
		global->tick_handler();
}
void bee_init(CoreData* core){
	uint32_t frequency = 100;
	
	syscall(core->irt,0,1,6,sizeof(GlobalData));
	GlobalData* global;
	syscall(core->irt,0,2,6,(u32)&global);
	global->ticks = 0;
	global->tick_handler = 0;
	global->freq_hz = frequency;
	
	// Calculate divisor
	uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);
	// Command: channel 0, lo/hi byte, mode 3 (square wave)
	outb(PIT_COMMAND, 0x36);
	// Set divisor
	outb(PIT_CHANNEL0_DATA, divisor & 0xFF);       // low byte
	outb(PIT_CHANNEL0_DATA, (divisor >> 8) & 0xFF); // high byte
	
	syscall(core->irt,4,1,32,0x8E,(u32)core->baseAdr+(u32)interrupt_handler);
	pic_unmask_irq(0);
}

void sleep(CoreData* core,GlobalData* global){
	u32* args = (u32*)core->ptr;
	uint32_t ms = args[0];
	
	uint32_t start = global->ticks;
	uint32_t target_ticks = start + (global->freq_hz * ms) / 1000;
	
	while(global->ticks < target_ticks)
		; // busy wait
}
void register_handler(CoreData* core,GlobalData* global){
	u32* args = (u32*)core->ptr;
	global->tick_handler = (void(*)(void))(args[0]);
}
void get_ticks(CoreData* core,GlobalData* global){
	u32* args = (u32*)core->ptr;
	*((u32*)(args[0])) = global->ticks;
}
void set_frequency(CoreData* core,GlobalData* global){
	u32* args = (u32*)core->ptr;
	u32 frequency = args[0];
	global->freq_hz = frequency;
	uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);
	
	outb(PIT_COMMAND, 0x36);
	outb(PIT_CHANNEL0_DATA, divisor & 0xFF);
	outb(PIT_CHANNEL0_DATA, (divisor >> 8) & 0xFF);
}































































