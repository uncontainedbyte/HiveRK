#include "../beeUtil.h"

typedef struct {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers_t;
typedef enum{
	IDT_FLAG_GATE_TASK              = 0x5,
	IDT_FLAG_GATE_16BIT_INT         = 0x6,
	IDT_FLAG_GATE_16BIT_TRAP        = 0x7,
	IDT_FLAG_GATE_32BIT_INT         = 0xE,
	IDT_FLAG_GATE_32BIT_TRAP        = 0xF,
	IDT_FLAG_RING0                  = (0 << 5),
	IDT_FLAG_RING1                  = (1 << 5),
	IDT_FLAG_RING2                  = (2 << 5),
	IDT_FLAG_RING3                  = (3 << 5),
	IDT_FLAG_PRESENT                = 0x80,
} IDT_FLAGS;
#define systemFlags IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT

void int_to_string(int num, char *buffer) {
    // A temporary buffer to store the digits in reverse order.
    char temp_buffer[20];
    int i = 0;
    int sign = 0;

    // Handle the case where the number is 0.
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // Handle negative numbers.
    if (num < 0) {
        sign = 1;
        num = -num;
    }

    // Extract digits and store them in the temporary buffer.
    while (num > 0) {
        temp_buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Add the negative sign if the original number was negative.
    if (sign) {
        temp_buffer[i++] = '-';
    }

    // Add the null terminator to the temporary buffer.
    temp_buffer[i] = '\0';

    // Reverse the temporary buffer into the final buffer.
    int j = 0;
    while (i > 0) {
        buffer[j++] = temp_buffer[--i];
    }

    // Add the null terminator to the final buffer.
    buffer[j] = '\0';
}
void SYSprint(const char* str,int x,int y,char color){
	//color: 0=black 1=blue 2=green 3=cyan 4=red 5=magenta 6=brown 7=L-grey 8=D-grey
	//       9=L-blue A=L-green B=L-cyan C=L-red D=L-magenta E=L-brown F=white
	color = ((color>>4)&0xF)|((color<<4)&0xF0);
	uint8_t* vidmem = (uint8_t*)0xb8000;
	int i=0,vp=160*y+x*2,nl=0;
	while(str[i]!='\0'){
		if(str[i]=='\n'){i++;nl++;vp=160*y+x*2+nl*160;continue;}
		vidmem[vp] = str[i];
		vidmem[vp + 1] = color;
		i++;
		vp+=2;
	}
	
}
void SYSprintI(const int num,int x,int y,char color){
	char buf[20];
	int_to_string(num,buf);
	SYSprint(buf,x,y,color);
}

void handle_DIVbyZERO(registers_t *r){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000;s++){
		vidmem[s] = 0;
	}
	char b[21];
	b[0]='!';b[1]=' ';b[2]='D';b[3]='i';b[4]='v';b[5]='i';b[6]='s';b[7]='i';b[8]='o';b[9]='n';
	b[10]=' ';b[11]='B';b[12]='y';b[13]=' ';b[14]='Z';b[15]='e';b[16]='r';b[17]='o';b[18]=' ';
	b[19]='!';b[20]='\0';
	SYSprint(b,0,0,0x40);
	asm volatile("hlt");
}


void handle_Breakpoint(registers_t *r){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000;s++){
		vidmem[s] = 0;
	}
	char b[15];
	b[0]='!';b[1]=' ';b[2]='B';b[3]='r';b[4]='e';b[5]='a';b[6]='k';b[7]='p';b[8]='o';b[9]='i';
	b[10]='n';b[11]='t';b[12]=' ';b[13]='!';b[14]='\0';
	SYSprint(b,0,0,0x40);
	asm volatile("hlt");
}


void handle_InvalidOpcode(registers_t *r){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000;s++){
		vidmem[s] = 0;
	}
	char b[19];
	b[0]='!';b[1]=' ';b[2]='I';b[3]='n';b[4]='v';b[5]='a';b[6]='l';b[7]='i';b[8]='d';b[9]=' ';
	b[10]='O';b[11]='p';b[12]='c';b[13]='o';b[14]='d';b[15]='e';b[16]=' ';b[17]='!';b[18]='\n';
	SYSprint(b,0,0,0x40);
	asm volatile("hlt");
}

void handle_DoubleFault(registers_t *r){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000;s++){
		vidmem[s] = 0;
	}
	char b[17];
	b[0]='!';b[1]=' ';b[2]='D';b[3]='o';b[4]='u';b[5]='b';b[6]='l';b[7]='e';b[8]=' ';b[9]='F';
	b[10]='a';b[11]='u';b[12]='l';b[13]='t';b[14]=' ';b[15]='!';b[16]='\0';
	SYSprint(b,0,0,0x40);
	asm volatile("hlt");
}




void handle_GeneralProtectionFault(registers_t *r){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000;s++){
		vidmem[s] = 0;
	}
	char b[29];
	b[0]='!';b[1]=' ';b[2]='G';b[3]='e';b[4]='n';b[5]='e';b[6]='r';b[7]='a';b[8]='l';b[9]=' ';
	b[10]='P';b[11]='r';b[12]='o';b[13]='t';b[14]='e';b[15]='c';b[16]='t';b[17]='i';b[18]='o';
	b[19]='n';b[20]=' ';b[21]='F';b[22]='a';b[23]='u';b[24]='l';b[25]='t';b[26]=' ';b[27]='!';
	b[28]='\0';
	SYSprint(b,0,0,0x40);
	asm volatile("hlt");
}

void registerBASIC(CoreData* core){
	
	syscall(core->irt,4,1,0,systemFlags,(u32)core->baseAdr+(u32)handle_DIVbyZERO);
	
	
	syscall(core->irt,4,1,3,systemFlags,(u32)core->baseAdr+(u32)handle_Breakpoint);
	
	
	syscall(core->irt,4,1,6,systemFlags,(u32)core->baseAdr+(u32)handle_InvalidOpcode);
	
	syscall(core->irt,4,1,8,systemFlags,(u32)core->baseAdr+(u32)handle_DoubleFault);
	
	
	
	
	syscall(core->irt,4,1,13,systemFlags,(u32)core->baseAdr+(u32)handle_GeneralProtectionFault);
}
