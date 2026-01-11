
#include <stdint.h>
#include "../bees/beeUtil.h"
#include "ERROR_CODES.h"

#define SEARCH_LIMIT 1024*100; // in KB


static void* MEMORY = (void*)(16*1024);//16KB start

void printERR(char* str,int y) {
	uint8_t* vidmem = (uint8_t*)0xb8000;
	int i=0,vp=160*y;
	while(str[i]!='\0'){
		vidmem[vp] = str[i];
		vidmem[vp + 1] = 4;
		i++;
		vp+=2;
	}
}




void call(uint32_t func){
	asm volatile ("call *%0\n":: "r" (func):);
}

uint32_t load_4B(uint8_t* ptr){
	uint32_t out=ptr[0];
	out |= (((uint32_t)ptr[1])<<8)&0xFF00;
	out |= (((uint32_t)ptr[2])<<16)&0xFF0000;
	out |= (((uint32_t)ptr[3])<<24)&0xFF000000;
	return out;
}
void store_4B(const uint32_t v,uint8_t* ptr){
	ptr[0] =  v     &0xFF;
	ptr[1] = (v<<8 )&0xFF00;
	ptr[2] = (v<<16)&0xFF0000;
	ptr[3] = (v<<24)&0xFF000000;
}



typedef struct __attribute__((packed)){ uint32_t ID; uint32_t func; } BEE_FUNC;
typedef struct __attribute__((packed)){
	uint32_t baseAddress;
	uint32_t BeeID;
	uint8_t priority;
	uint8_t format;
	uint32_t exportCount;
	uint32_t initFunc;
	BEE_FUNC funcs[];
} BEE_DATA_PAGE;
typedef struct __attribute__((packed)){ char name[32]; BEE_DATA_PAGE* data; } BEE_NAME;
typedef struct __attribute__((packed))BEE_NAMES_PAGE{
	BEE_NAME names[14];
	
	struct BEE_NAMES_PAGE* nextPage;
	uint32_t __unused;
} BEE_NAMES_PAGE;
/*

BEE-NAMES-PAGE:
	times-14:
		32B-name . 4B-data-ptr
	
	4B-next-page-ptr
	4B-unused

BEE-DATA-PAGE:
	4B-beeID
	1B-priority
	1B-format
	4B-initFunc
	4B-num_exports
	exprt*num_exports:
		32B-name 4B-func 4B-ID

*/


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
void int_to_hex_string(int num, char *buffer) {
    // Temporary buffer to store hex digits in reverse order
    char temp_buffer[20];
    int i = 0;
    int sign = 0;

    // Handle zero
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        sign = 1;
        num = -num;
    }

    // Extract hex digits
    while (num > 0) {
        int digit = num % 16;
        if (digit < 10)
            temp_buffer[i++] = digit + '0';
        else
            temp_buffer[i++] = (digit - 10) + 'A'; // use 'a' for lowercase

        num /= 16;
    }

    // Add negative sign if needed
    if (sign) {
        temp_buffer[i++] = '-';
    }

    // Null-terminate temp buffer
    temp_buffer[i] = '\0';

    // Reverse into output buffer
    int j = 0;
    while (i > 0) {
        buffer[j++] = temp_buffer[--i];
    }

    // Null-terminate final buffer
    buffer[j] = '\0';
}

static uint32_t printADR = 0;
void print(const char* ptr,int x,int y,int color){
	asm volatile("pushl %0":: "r" (y));
	asm volatile("pushl %0":: "r" (x));
	asm volatile("pushl %0":: "r" (color));
	asm volatile("pushl %0":: "r" (ptr));
	asm volatile("call *%0":: "r" (printADR));
	asm volatile("addl $16, %%esp"::);
}



int ata_read_sector_safe(unsigned int lba, unsigned char *buf) {
	outb(0x1F6, 0xE0 | ((lba >> 24) & 0xF));
	outb(0x1F2, 1);
	outb(0x1F3, lba & 0xFF);
	outb(0x1F4, (lba >> 8) & 0xFF);
	outb(0x1F5, (lba >> 16) & 0xFF);
	outb(0x1F7, 0x20); // READ SECTOR
	while (inb(0x1F7) & 0x80);//Poll the Status Register until BSY clears
	unsigned char status = inb(0x1F7);
	if (status & 0x01) {
		//unsigned char error_code = inb(0x1F1);
		return -1;
	}
	for (int i = 0; i < 256; i++) {
		((short*)buf)[i] = inw(0x1F0);
	}
	
	return 0;
}
/*void ata_read_sector(unsigned int lba, unsigned char *buf) {
	outb(0x1F6, 0xE0 | ((lba >> 24) & 0xF));
	outb(0x1F2, 1);
	outb(0x1F3, lba & 0xFF);
	outb(0x1F4, (lba >> 8) & 0xFF);
	outb(0x1F5, (lba >> 16) & 0xFF);
	outb(0x1F7, 0x20); // READ SECTOR
	for (int i = 0; i < 256; i++)
		((short*)buf)[i] = inw(0x1F0);
}*/


uint32_t find_bee(uint32_t byteAddress,uint32_t searchLimit){
	uint8_t buf[1024];
	
	uint32_t startSector = byteAddress / 512;
	uint32_t startOffset = byteAddress % 512;
	
	for(int i=0;i<searchLimit;i++){
		if(ata_read_sector_safe(startSector+i,buf)){ return 0; }
		if(ata_read_sector_safe(startSector+i+1,buf+512)){ return 0; }
		uint32_t searchStart = 0;
		if(i==0){ searchStart = startOffset; }
		for(int j = searchStart; j < 512; j++){
			if(buf[j] == 0x3A && buf[j+1] == 0x44 && buf[j+2] == 0xBE && buf[j+3] == 0xE0){
				return (startSector + i) * 512 + j;
			}
		}
	}
	
	return 0;
}
uint32_t load_bee(uint32_t address, uint8_t* bufferIndex){
	uint32_t startAddress = 0;
	static uint32_t endAddress = 0;
	
	int state=0;
	for(int i=0;i<2048;i++){
		if(ata_read_sector_safe(address+i,bufferIndex+i*512)){ return 0; }
		uint8_t* buf = bufferIndex+i*512;
		for(int j=0;j<509;j++){// code is 4 bytes, so 512-3
			if(buf[j]==0x3A&&buf[j+1]==0x44&&buf[j+2]==0xBE&&buf[j+3]==0xE0){
				if(state==0){
					state=1;
					startAddress=(uint32_t)bufferIndex+i*512+j;
				}else if(state==1){
					state=1;
					endAddress=(uint32_t)bufferIndex+i*512+j;
					return startAddress;
				}
			}
		}
	}
	
	return 0;
}


uint32_t BEE_SCAN_AND_LOAD(uint8_t* bufferIndex){
	static uint32_t last_end=512;
	uint32_t search_limit = SEARCH_LIMIT;//1MB
	
	uint32_t start = find_bee(last_end,search_limit);
	if(start==0){ return 0; }
	uint32_t end = find_bee(start+4,search_limit)+4;
	last_end = end;
	if(end==0){ return 0; }
	
	uint32_t sectorStart = start/512;
	uint32_t sectorEnd = end/512;
	sectorEnd += (end%512>0)?1:0;
	
	uint32_t memoryUsed=0;
	for(int i=0;i<sectorEnd-sectorStart;i++){
		memoryUsed++;
		if(ata_read_sector_safe(sectorStart+i,bufferIndex+i*512)){ return 0; }
	}
	MEMORY+=512*memoryUsed;
	return (uint32_t)bufferIndex+ start%512;
}

void addNameToPage(BEE_NAMES_PAGE* page,int index,BEE_NAME* name){
	if(index>13){
		if(page->nextPage==0){
			page->nextPage = MEMORY;
			page->nextPage->nextPage=0;
			MEMORY+=512;
		}
		addNameToPage((BEE_NAMES_PAGE*)page->nextPage,index-14,name);
	}else{
		for(int i=0;i<32;i++){
			page->names[index].name[i] = name->name[i];
		}
		page->names[index].data = name->data;
		if(index+1<14){ page->names[index+1].name[0]='\0'; }
	}
}
BEE_DATA_PAGE* makeBeeData(uint8_t* buf){
	BEE_DATA_PAGE* data = MEMORY;
	
	data->baseAddress = (uint32_t)buf;
	
	uint8_t* index = buf+4;
	
	data->format = index[0];
	index+=1;
	data->BeeID = load_4B(index);
	index+=36;
	data->priority = index[0];
	index+=1;
	data->exportCount = load_4B(index);
	index+=4;
	uint32_t size = 14;
	
	for(int i=0;i<data->exportCount;i++){
		data->funcs[i].ID = load_4B(index);
		index+=4;
		data->funcs[i].func = ((uint32_t)buf)+load_4B(index);
		index+=4;
		size += 8;
	}
	
	data->initFunc = ((uint32_t)buf)+load_4B(index);
	size+=4;
	MEMORY += (size + 511) / 512 * 512;;
	return data;
}



void printA(const char* ptr,int x,int y,int color,uint32_t adr){
	asm volatile(
        "pushl  %[y]\n\t"
        "pushl  %[x]\n\t"
        "pushl  %[color]\n\t"
        "pushl  %[ptr]\n\t"
        "call   *%[adr]\n\t"
        "addl   $16, %%esp"
        : // No output operands
        : [y] "g" (y), [x] "g" (x), [color] "g" (color), [ptr] "g" (ptr), [adr] "g" (adr)
        : "eax", "ecx", "edx", "memory" // Clobbered registers
    );
}


// ############################
// ############################
// ############################




#define KBD_STATUS 0x64
#define KBD_DATA   0x60
static uint64_t rdtsc(void) {
	uint32_t lo, hi;
	asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((uint64_t)hi << 32) | lo;
}
int keyboard_has_data(void) {
	return inb(KBD_STATUS) & 1;
}
uint8_t keyboard_read(void) {
	return inb(KBD_DATA);
}

//## basic display functions ##

void SYSclear(){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000;s++){
		vidmem[s] = 0;
	}
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
void SYSshift(){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000-160;s++){
		vidmem[s]=vidmem[s+160];
	}
}
void SYSline(int y){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<160;s++){
		vidmem[s+160*y] = 0;
	}
}
void SYScursor(char cr,int x){
	static uint32_t last = 1;
	char s[2];
	uint32_t t = rdtsc()>>31;
	
	if(last<t){
		if(last+1<t) last = t;
		s[0]=cr;
		s[1]='\0';
		SYSprint(s,x,24,0xF0);
	}else{
		s[0]=' ';
		s[1]='\0';
		SYSprint(s,x,24,0xF0);
	}
}
void SYSreboot(void) {
	while(inb(0x64) & 0x02);
	outb(0x64, 0xFE);
	__asm__ volatile ("hlt");
}
void SYSprintI(const int num,int x,int y,char color){
	char buf[20];
	int_to_string(num,buf);
	SYSprint(buf,x,y,color);
}
void SYSprintX(const int num,int x,int y,char color){
	char buf[20];
	int_to_hex_string(num,buf);
	SYSprint(buf,x,y,color);
}

//## basic display functions ##


int checkForControlKeys(){
	char* str = "Press F1 for system control & configuration";
	SYSprint(str,0,0,0xF0);
	
	uint32_t delay = 1; // in seconds
	uint32_t start = rdtsc()>>32;
	while(start+delay>=rdtsc()>>32){
		if(keyboard_has_data()){
			uint8_t key = keyboard_read();
			if(key == 0x3B){
				int_to_string((uint32_t)key,str);
				SYSprint("Entering control & configuration Comb",0,1,0xF0);
				return 1;
			}
		}
	}
	
	return 0;
}

static const char scancode_to_ascii[128] = {
	// Numbers
	[0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
	[0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
	[0x0A] = '9', [0x0B] = '0',
	
	// Symbols (base)
	[0x0C] = '-', [0x0D] = '=',
	[0x1A] = '[', [0x1B] = ']',
	[0x27] = ';', [0x28] = '\'',
	[0x29] = '`',
	[0x2B] = '\\',
	[0x33] = ',', [0x34] = '.',
	[0x35] = '/',
	
	[0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r',
	[0x14] = 't', [0x15] = 'y', [0x16] = 'u', [0x17] = 'i',
	[0x18] = 'o', [0x19] = 'p',
	[0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f',
	[0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k',
	[0x26] = 'l',
	[0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v',
	[0x30] = 'b', [0x31] = 'n', [0x32] = 'm',
	
	[0x39] = ' ',   // Space
	[0x1C] = '\n',  // Enter
	[0x0E] = '\b',  // Backspace
};
char scancode_to_char(uint8_t sc){
	if(sc & 0x80) return 0;
	return scancode_to_ascii[sc];
}
int cmpStr(const char* s1,const char* s2){
	int i=0;
	while(s1[i]!='\0'&&s2[i]!='\0'){
		if(s1[i]!=s2[i]){ break; }
		i++;
	}
	return s1[i]==s2[i];
}
void parseArgs(const char* base,char* s1,char* s2,char* s3,char* s4,char* s5){
	s1[0]='\0';
	s2[0]='\0';
	s3[0]='\0';
	s4[0]='\0';
	s5[0]='\0';
	
	int i=0;
	int argi=0;
	while(base[i]!='\0'&&base[i]!=' '){
		s1[argi]=base[i];
		argi++; i++;
		s1[argi]='\0';
	}
	while(base[i]==' '){i++;}
	if(base[i]=='\0'){ return; }
	argi=0;
	while(base[i]!='\0'&&base[i]!=' '){
		s2[argi]=base[i];
		argi++; i++;
		s2[argi]='\0';
	}
	while(base[i]==' '){i++;}
	if(base[i]=='\0'){ return; }
	argi=0;
	while(base[i]!='\0'&&base[i]!=' '){
		s3[argi]=base[i];
		argi++; i++;
		s3[argi]='\0';
	}
	while(base[i]==' '){i++;}
	if(base[i]=='\0'){ return; }
	argi=0;
	while(base[i]!='\0'&&base[i]!=' '){
		s4[argi]=base[i];
		argi++; i++;
		s4[argi]='\0';
	}
	while(base[i]==' '){i++;}
	if(base[i]=='\0'){ return; }
	argi=0;
	while(base[i]!='\0'&&base[i]!=' '){
		s5[argi]=base[i];
		argi++; i++;
		s5[argi]='\0';
	}
}
int handleCMD(const char* buf,BEE_NAMES_PAGE* bees){
	char cmd[61],arg1[61],arg2[61],arg3[61],arg4[61];
	
	parseArgs(buf,cmd,arg1,arg2,arg3,arg4);
	
	if(cmpStr(cmd,"clear")){
		SYSclear();
	}else if(cmpStr(cmd,"tea")){
		SYSprint(("  {LBEk@u0%M@y*R*                \"*4@\"4@@@&,FB4[@B\n\
e#BB[&B+5w$1R'                       *4%%R, 4B4LB4\n\
&B4BnEw2P&M            ,,.              *4B&w7QJ4U\n\
4B44rTS4F         ,x444RMR444Ew,         `44$ZA[44\n\
4@4B4L,`       ,r44FDe&HuuuD@mDA4Lu        RE\"F1#4\n\
4E4@4R\"      e4&*]J#@BBB4@B4@BBwUuR4W       *N4$$B\n\
*RB4BF     44A,@A,4B444RB@@B@REBLTYW*4W      44444\n\
@BBBB    ,4R)K4MA***\"%@%|%%@@A'\"`>t\"E%\"4,    'jM&M\n\
4REU    ,4FnIw/4K4LE T%%4N%%N@J|44k rm$\4u    4BRB\n\
BM4P    4F44m4 444Q. w\\`|N%%R*IJ444kRY @T4    `M**\n\
44R    4Pe4BP\"      \"44w,244R,,     *[& w4&       \n\
M@`   JRjJB@ [B*    4444444444\"'   E jBB  4L      \n\
%F   ,4)jBBBLW4u*vm 4444444444L\"wK,44 BBL `4,     \n\
*    4`HjB4BL444&wr44444B%444444w4444hBBB %'B     \n\
    4E|  BB4L444444444444%44444444444\"B44  %T&    \n\
    4 [ UBBB|44444444444444444444444MuBB4   ufE   \n\
 zf4F ] UB&BL,44444444&RRRMR4444444R HBRB  4L BL. \n\
44JR  j H4Tk[ h*44&\",e4444444KEZR4M/  @k4J4@@ `4L&\n\
4EBL  `  [ [@)'| 5.4&4444444444TL~N@\"J@L[,e@&  j4L\n\
`N*Nw  `1  J% u~,r4#[K4444444;4z*RBw1[@hT4@&' ,@M,\n\
4``w*4k,  1\"0L444Er@wf&M*M\"* IE04$,4MBM  &F ,AOe&H\n\
  \"*4RkZ*4 U\"%444444R,'Jr4whw4E84E84uM` ' 4U@@%M\" \n\
      +4RMM\"K0R4444A4R4`   `` `4M4444%M*K4RRP     "),0,0,0x20);
	}else if(cmpStr(cmd,"gun")){
		SYSprint(("4444444444444444444444444444R*\"*444444444444444444\n\
4444444444444444444444444R\",rB4B f444444444444444R\n\
44444444444444444444444* zB44444L 444444444444A' -\n\
444444444444444444444\" rB44444444 f44444444F` wB44\n\
444444444444444444`  r44444444RA* ,,,,. `` ,44444'\n\
44444444444444444FJkB44444444M   444R`  ,444444E,B\n\
4444444444444444' r444444444F   444\"   rB444444LrH\n\
444444444444P=T r44444BE4444&,rE44E   4444444444&*\n\
44444A\"\"\"*BF,rw44444M   f4444444444B4444444444444B\n\
44M\"\" @ h   '4B444B'    J44444B44444444R\"  `*44444\n\
M z@     Nu    '*4BL   ,4&4444444BM@44R      `4444\n\
 8  '  \"@  H%u    `*44444k444M4444R4%4E      ,4444\n\
 8 |    [  H   `      \"f444444k22r44B44Bkw,w444444\n\
 8  W,,eu  H             `*444444444%4444444444444\n\
 \"`` \"\" H`                   \"` `\"*@44444444444444\n\
 8N *%m\".1NH                        44444444444444\n\
  `UNNKNM`                      `% ,'4444444444444\n\
w,                                   !444444444444\n\
444Ew,                                4E4440*44B  \n\
44444444rw,                           j `**  %u~@ \n\
4444444444` B@@BBL ,4BBLBK@4Wy       ,K           \n\
444444444E 4BBEEEE BEBKK44444444w   |             \n\
444444444  B444444.\\BB4444444444B4L.`%%%%%%%%%%%%%"),0,0,0x20);
	}else if(cmpStr(cmd,"reboot")){
		SYSreboot();
	}else if(cmpStr(cmd,"list-bees")||cmpStr(cmd,"lb")){
		BEE_NAMES_PAGE* cur=bees;
		int total = 0;
		for(int s=0;1;s+=2){
			if(s-14==0){ if(cur->nextPage==0){break;} s=0; cur=cur->nextPage; }
			if(cur->names[s].name[0]=='\0') break;
			SYSshift();
			SYSprint(cur->names[s].name,0,23,0x50);    total++;
			if(cur->names[s+1].name[0]=='\0') break;
			SYSprint(cur->names[s+1].name,33,23,0x50); total++;
		}
		SYSshift();
		SYSprint("Bees Loaded: ",0,23,0x20);
		SYSprintI(total,13,23,0x20);
	}else if(cmpStr(cmd,"exit")){
		return 1;
	}else if(cmpStr(cmd,"fox")){
		SYSprint(("................................................................................\n\
................     ......................................     ................\n\
..............  .....     ............................     .....  ..............\n\
.............   =====.     ..........................     .=====   .............\n\
.............   @@@@@+====-   ....................   -====+@@@@@   .............\n\
.............   @@@@@%%%%%#--   ...          ...   --#%%%%%@@@@@   .............\n\
.............   @@@@@@@@@@%==...  ............  ...==%@@@@@@@@@@   .............\n\
.............   @@@@@@@@@@%++===  .++++++++++.  ===++%@@@@@@@@@@   .............\n\
................  :@@@@@@@%++++++++++++++++++++++++++%@@@@@@@:  ................\n\
................  :@@@@@***++++++++++++++++++++++++++***@@@@@:  ................\n\
................  :##%%%+++**=----=++++++++++=----=**+++%%%##:  ................\n\
..................   -++++*@@:    .++++++++++.    :@@*++++-   ..................\n\
..................   -++++*@@:    .++++++++++.    :@@*++++-   ..................\n\
................  :##++++++**=----=++++++++++=----=**++++++##:  ................\n\
................  :##***++++++++++++++++++++++++++++++++***##:  ................\n\
..................   #@@+++++++++++==========+++++++++++@@#   ..................\n\
................  :@@@@@@@%+++++++*@@:    :@@*+++++++%@@@@@@@:  ................\n\
................  .::#@@@@@%%%%%%%%@@######@@%%%%%%%%@@@@@#::.  ................\n\
...................  +##@@@@@@@@###@@@@@@@@@@###@@@@@@@@##+  ...................\n\
.....................   @@@@@@@@  :@@@@@@@@@@:  @@@@@@@@   .....................\n\
........................  :@@@@@@@#          #@@@@@@@:  ........................\n\
........................  .:::::@@@##########@@@:::::.  ........................\n\
...........................     ################     ...........................\n\
................................                ................................\n\
................................................................................"),0,0,0x20);
	}
	
	return 0;
}
void ControlComb(BEE_NAMES_PAGE* bees){
	char buf[61];
	int index=0;
	
	int exit=0;
	while(!exit){
		if(keyboard_has_data()){
			char key = scancode_to_char(keyboard_read());
			if(key==0) continue;
			if(key=='\b'){if(index==0){continue;} SYSline(24); index--; buf[index]='\0'; }else
			if(key=='\n'){
				SYSprint(" ",index+3,24,0xF0);
				SYSshift();
				SYSline(24);
				if(index>0){if(handleCMD(buf,bees)){return;}}else{continue;}
				buf[0]='\0';
				index=0;
			}else
			if(index<60){
				buf[index] = key;
				index++;
				buf[index] = '\0';
			}
		}
		
		SYSprint(">> ",0,24,0xF0);
		SYSprint(buf,3,24,0xF0);
		SYScursor(2,index+3);
	}
	
}




// ############################
// ############################
// ############################

BEE_NAMES_PAGE* page0;
struct idNode{
	uint32_t id;
	CoreData data;
	void*    globalValues;
	uint32_t numFuncs;
	uint32_t funcs[];
};
#define MAX_BEES 1024
struct idNode* idNodes[1024];




uint32_t lookupID(uint32_t id){
	for(int s=0;s<MAX_BEES;s++){
		if(idNodes[s]->id == id) return s+1;
	}
	return 0;
}
void alloc(uint32_t size,void** data){
	*data = MEMORY;
	for(int s=0;s<size;s++){
		((uint32_t*)MEMORY)[s]=0;
	}
	MEMORY += size;
}
void (*allocF)(uint32_t,void**) = alloc;
void core_interrupt_handler(void){
	uint32_t ID=0,CMD=0,arsSize=0,dataPtr=0;
	ASMeaxG(ID);
	ASMebxG(CMD);
	ASMecxG(arsSize);
	ASMedxG(dataPtr);
	
	//SYSprintI(ID,20,19,0x20);
	//SYSprintI(CMD,20,20,0x20);
	if(ID==0){
		switch(CMD){
			case 1:{
				uint32_t* args = (uint32_t*)dataPtr;
				uint32_t id = args[0];
				uint32_t size = args[1];
				
				uint32_t node = lookupID(id);
				if(node==0){ ASMeaxS(CORE_ERROR_NO_MODULE); return; }
				node--;
				if(idNodes[node]->globalValues!=0){ return; }
				allocF(size,&idNodes[node]->globalValues);
			}break;
			case 2:{
				uint32_t* args = (uint32_t*)dataPtr;
				uint32_t id = args[0];
				uint32_t* ptr = (uint32_t*)(args[1]);
				
				uint32_t node = lookupID(id);
				if(node==0){ ASMeaxS(CORE_ERROR_NO_MODULE); return; }
				node--;
				*ptr = (uint32_t)idNodes[node]->globalValues;
			}break;
			case 3:{
				uint32_t* args = (uint32_t*)dataPtr;
				uint32_t id = args[0];
				uint32_t* ptr = (uint32_t*)(args[1]);
				
				uint32_t node = lookupID(id);
				if(node==0){ ASMeaxS(CORE_ERROR_NO_MODULE); return; }
				node--;
				*ptr = (uint32_t)&idNodes[node]->data;
			}break;
			case 4:{
				uint32_t* args = (uint32_t*)dataPtr;
				uint32_t* ptr = (uint32_t*)(args[0]);
				*ptr = (uint32_t)MEMORY;
			}break;
			case 5:{
				uint32_t* args = (uint32_t*)dataPtr;
				allocF = (void*)args[0];
			}break;
			default:{ ASMeaxS(CORE_ERROR_NO_CMD); return; }
		};
	}else{
		uint32_t node = lookupID(ID);
		if(node==0){ ASMeaxS(CORE_ERROR_NO_MODULE); return; }
		node--;
		
		if(idNodes[node]->funcs[CMD]==0){ ASMeaxS(CORE_ERROR_NO_CMD); return; }
		if(idNodes[node]->globalValues==0){
			CoreData* h = &idNodes[node]->data;
			h->count = arsSize;
			h->ptr = (void*)dataPtr;
			ASMpush4(h);
			ASMcall(idNodes[node]->funcs[CMD],4);
		}else{
			CoreData* h = &idNodes[node]->data;
			h->count = arsSize;
			h->ptr = (void*)dataPtr;
			ASMpush4(idNodes[node]->globalValues);
			ASMpush4(h);
			ASMcall(idNodes[node]->funcs[CMD],4);
		}
	}
	ASMeaxS(CORE_ERROR_NONE);
}
void setupLookupArray(BEE_NAMES_PAGE* page,int depth){
	int i=0;
	
	while(i<14&&page->names[i].name[0]!='\0'){
		idNodes[depth+i] = MEMORY;
		
		idNodes[depth+i]->id = page->names[i].data->BeeID;
		idNodes[depth+i]->data.irt = (uint32_t)core_interrupt_handler;
		idNodes[depth+i]->data.baseAdr = (void*)page->names[i].data->baseAddress;
		idNodes[depth+i]->data.count = 0;
		idNodes[depth+i]->data.ptr = 0;
		idNodes[depth+i]->globalValues = 0;
		
		int MAX_ID=-1,id=0;
		for(int s=0;s<page->names[i].data->exportCount;s++){
			id=page->names[i].data->funcs[s].ID;
			if(MAX_ID<id){ MAX_ID=id; }
		}
		for(int s=0;s<MAX_ID;s++){
			idNodes[depth+i]->funcs[s] = 0;
		}
		for(int s=0;s<page->names[i].data->exportCount;s++){
			id=page->names[i].data->funcs[s].ID;
			idNodes[depth+i]->funcs[id] = page->names[i].data->funcs[s].func;
		}
		idNodes[depth+i]->numFuncs = MAX_ID;
		MEMORY += (MAX_ID+1)*4+4+16+4+4;
		i++;
	}
	
	if(i<14){ return; }
	if(page->nextPage==0){ return; }
	setupLookupArray(page->nextPage,depth+14);
}
void run_inits(BEE_NAMES_PAGE* page){
	BEE_NAMES_PAGE* cur = page;
	
	for(int s=0;s<256;s++){
		uint32_t i=0,pry;
		cur = page;
		while(cur->names[i].name[0] != '\0'){
			pry = cur->names[i].data->priority;
			if(pry == s){
				uint32_t n = lookupID(cur->names[i].data->BeeID)-1;
				ASMpush4(&idNodes[n]->data);
				ASMcall(cur->names[i].data->initFunc,4);
			}
			i++;
			if(i==14&&cur->nextPage==0){ break; }else
			if(i==14){ i-=14; cur = cur->nextPage; }
		}
	}
}

void main(){
	page0 = MEMORY;
	page0->nextPage=0;
	page0->names[0].name[0] = '\0';
	int pageIndex=0;
	MEMORY+=512;
	uint32_t address;
	
	do{
		address = BEE_SCAN_AND_LOAD(MEMORY);
		if(address==0){ break; }
		
		BEE_NAME Data;
		for(int i=0;i<32;i++){
			Data.name[i] = ((uint8_t*)(address+9))[i];
		}
		Data.data = makeBeeData((void*)address);
		
		addNameToPage(page0,pageIndex,&Data);
		pageIndex++;
		
	}while(1);
	
	
	
	
	
	
	
	if(0&&checkForControlKeys()){
		ControlComb(page0);
	}
	
	
	uint32_t* v = (uint32_t*)CORE_GLOBAL_MEMORY;
	v[0] = (uint32_t)core_interrupt_handler;
	for(int s=0;s<MAX_BEES;s++){ idNodes[s]=0; }
	setupLookupArray(page0,0);
	run_inits(page0);
	
	
	
	
	
	//CoreData g;
	//g.baseAdr = (void*)page0->names[0].data->baseAddress;
	//g.irt = (uint32_t)core_interrupt_handler;
	
	//asm volatile("pushl %0":: "r" (&g));
	//asm volatile ("call *%0\n":: "r" (page0->names[0].data->initFunc):);
	//asm volatile("addl $4, %%esp"::);
	
	
	
	SYSprint("\
###############################\n\
###                         ###\n\
!!! this should not bee see !!!\n\
###                         ###\n\
###############################",0,0,0x40);
	
	while(1){};
}























