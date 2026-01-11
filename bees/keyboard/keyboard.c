#include "../beeUtil.h"




void pic_unmask_irq(uint8_t irq) {
	uint16_t port = (irq < 8) ? 0x21 : 0xA1;
	uint8_t  mask = inb(port) & ~(1 << (irq % 8));
	outb(port, mask);
}

#define KMOD_SHIFT  (1 << 0)
#define KMOD_CTRL   (1 << 1)
#define KMOD_ALT    (1 << 2)
#define KMOD_SUPER  (1 << 3)
#define KMOD_CAPS   (1 << 4)
#define KMOD_NUM    (1 << 5)

#define KFLAG_RELEASE   (1 << 0)  // key up
#define KFLAG_EXTENDED  (1 << 1)  // 0xE0 scancode
#define KFLAG_REPEAT    (1 << 2)
#define KFLAG_SYSTEM    (1 << 3)  // special combo

uint32_t make_key_event(uint8_t ascii,uint8_t scancode,uint8_t mods,uint8_t flags){
	return  (uint32_t)ascii       | ((uint32_t)scancode << 8)
		 | ((uint32_t)mods << 16) | ((uint32_t)flags    << 24);
}

typedef struct{
	uint32_t buffer[64];
	volatile uint8_t head, tail;
	uint8_t shift;
} GlobalData;

uint8_t key_map(uint8_t sc) {
	switch (sc) {
		case 0x02: return '1';
		case 0x03: return '2';
		case 0x04: return '3';
		case 0x05: return '4';
		case 0x06: return '5';
		case 0x07: return '6';
		case 0x08: return '7';
		case 0x09: return '8';
		case 0x0A: return '9';
		case 0x0B: return '0';
		
		case 0x0C: return '-';
		case 0x0D: return '=';
		
		case 0x10: return 'q';
		case 0x11: return 'w';
		case 0x12: return 'e';
		case 0x13: return 'r';
		case 0x14: return 't';
		case 0x15: return 'y';
		case 0x16: return 'u';
		case 0x17: return 'i';
		case 0x18: return 'o';
		case 0x19: return 'p';
		
		case 0x1A: return '[';
		case 0x1B: return ']';
		
		case 0x1E: return 'a';
		case 0x1F: return 's';
		case 0x20: return 'd';
		case 0x21: return 'f';
		case 0x22: return 'g';
		case 0x23: return 'h';
		case 0x24: return 'j';
		case 0x25: return 'k';
		case 0x26: return 'l';
		
		case 0x27: return ';';
		case 0x28: return '\'';
		case 0x29: return '`';
		
		case 0x2C: return 'z';
		case 0x2D: return 'x';
		case 0x2E: return 'c';
		case 0x2F: return 'v';
		case 0x30: return 'b';
		case 0x31: return 'n';
		case 0x32: return 'm';
		
		case 0x33: return ',';
		case 0x34: return '.';
		case 0x35: return '/';
		
		case 0x39: return ' ';
		
		case 0x0E: return '\b'; // backspace
		case 0x1C: return '\n'; // enter
		case 0x0F: return '\t'; // tab
		
		default:
			return 0;
	}
}
uint8_t shift_map(uint8_t sc) {
	switch(sc){
		case 0x02: return '!';
		case 0x03: return '@';
		case 0x04: return '#';
		case 0x05: return '$';
		case 0x06: return '%';
		case 0x07: return '^';
		case 0x08: return '&';
		case 0x09: return '*';
		case 0x0A: return '(';
		case 0x0B: return ')';
		
		case 0x0C: return '_';
		case 0x0D: return '+';
		
		case 0x10: return 'Q';
		case 0x11: return 'W';
		case 0x12: return 'E';
		case 0x13: return 'R';
		case 0x14: return 'T';
		case 0x15: return 'Y';
		case 0x16: return 'U';
		case 0x17: return 'I';
		case 0x18: return 'O';
		case 0x19: return 'P';
		
		case 0x1A: return '{';
		case 0x1B: return '}';
		
		case 0x1E: return 'A';
		case 0x1F: return 'S';
		case 0x20: return 'D';
		case 0x21: return 'F';
		case 0x22: return 'G';
		case 0x23: return 'H';
		case 0x24: return 'J';
		case 0x25: return 'K';
		case 0x26: return 'L';
		
		case 0x27: return ':';
		case 0x28: return '"';
		case 0x29: return '~';
		
		case 0x2C: return 'Z';
		case 0x2D: return 'X';
		case 0x2E: return 'C';
		case 0x2F: return 'V';
		case 0x30: return 'B';
		case 0x31: return 'N';
		case 0x32: return 'M';
		
		case 0x33: return '<';
		case 0x34: return '>';
		case 0x35: return '?';
		
		case 0x39: return ' ';
		
		case 0x0E: return '\b';
		case 0x1C: return '\n';
		case 0x0F: return '\t';
		
		default:
			return 0;
	}
}

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
	syscall(irt,0,2,5,(u32)&global);
	
	uint8_t scancode = inb(0x60);
	
	uint8_t sc = inb(0x60);
	
	uint8_t released = sc & 0x80;
	sc &= 0x7F;
	
	if (sc == 42 || sc == 54) global->shift = !released;
	
	uint8_t ascii = global->shift ? shift_map(sc) : key_map(sc);
	uint8_t mods  = global->shift ? KMOD_SHIFT : 0;
	uint8_t flags = released ? KFLAG_RELEASE : 0;
	
	uint32_t key = make_key_event(ascii, sc, mods, flags);
	
	global->buffer[global->head] = key;
	global->head = (global->head + 1) % 64;
}
void bee_init(CoreData* core){
	syscall(core->irt,0,1,5,sizeof(GlobalData));
	GlobalData* global;
	syscall(core->irt,0,2,5,(u32)&global);
	global->head = 0;
	global->tail = 0;
	global->shift = 0;
	
	syscall(core->irt,4,1,33,0x8E,(u32)core->baseAdr+(u32)interrupt_handler);
	pic_unmask_irq(1);
}

void get_key(CoreData* core, GlobalData* global){
	asm volatile("cli");
	
	u32* args = (u32*)core->ptr;
	u32* output = (u32*)(args[0]);
	
	if(global->head == global->tail){
		*output = 0;
		asm volatile("sti");
		return;
	}
	
	*output = global->buffer[global->tail];
	global->tail = (global->tail + 1) % 64;
	asm volatile("sti");
}
void is_there_data(CoreData* core, GlobalData* global){
	u32* args = (u32*)core->ptr;
	u32* output = (u32*)(args[0]);
	if(global->head == global->tail){
		*output = 0;
	}else{ *output = 1; }
}


















































