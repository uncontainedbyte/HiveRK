
#include <stdint.h>
#include "../beeUtil.h"

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define DEFAULT_COLOR 0x02


void set_cursor(int offset);
int get_cursor();



void bee_init(CoreData* core){}

int hex_to_int(char hex_digit) {
	if(hex_digit >= '0' && hex_digit <= '9'){ return hex_digit - '0';
	}else if(hex_digit >= 'a' && hex_digit <= 'f'){ return hex_digit - 'a' + 10;
	}else if(hex_digit >= 'A' && hex_digit <= 'F'){ return hex_digit - 'A' + 10; }
	return -1;
}
int is_hex_digit(char c) {
	if(c >= '0' && c <= '9'){ return 1; }
	if(c >= 'a' && c <= 'f'){ return 1; }
	if(c >= 'A' && c <= 'F'){ return 1; }
	return 0;
}
void setChar(char character, int offset, int color) {
	if(offset%2!=0){return;}
	uint8_t *vidmem = (uint8_t *)VIDEO_ADDRESS;
	vidmem[offset] = character;
	vidmem[offset + 1] = (color<0)? DEFAULT_COLOR:color;
}
void shiftUp(){
	uint8_t* vidmem = (uint8_t*)VIDEO_ADDRESS;
	for(int s=0;s<MAX_ROWS*MAX_COLS*2-MAX_COLS*2;s++){
		vidmem[s]=vidmem[s+160];
	}
	for(int s=0;s<MAX_COLS;s++){
		vidmem[s*2+MAX_ROWS*MAX_COLS*2-MAX_COLS*2]=' ';
		vidmem[s*2+1+MAX_ROWS*MAX_COLS*2-MAX_COLS*2]=DEFAULT_COLOR;
	}
}
void printChar(char ch,int color){
	int offset = get_cursor();
	
	if(offset >= MAX_ROWS * MAX_COLS * 2){
		offset = offset - 2 * MAX_COLS;
		shiftUp();
	}
	if(ch == '\n'){
		offset += MAX_COLS * 2;
		offset -= offset%(2*MAX_COLS);
	}else if(ch == '\b'){
		offset -= 2;
		setChar(' ', offset,color);
	}else{
		setChar(ch, offset,color);
		offset += 2;
	}
	set_cursor(offset);
}
void printStr(char* str,int color){
	int index=0;
	while(str[index]!='\0'){
		printChar(str[index],color);
		index++;
	}
}
void print(CoreData* core){
	int* args = (int*)core->ptr;
	char* str = (char*)args[0];
	int color = DEFAULT_COLOR;
	
	//color: 0=black 1=blue 2=green 3=cyan 4=red 5=magenta 6=brown 7=L-grey 8=D-grey
	//       9=L-blue A=L-green B=L-cyan C=L-red D=L-magenta E=L-brown F=white
	
	int index=0,argsIndex=1;
	while(str[index]!='\0'){
		if(str[index]=='%'){
			index++;
			switch(str[index]){
				case 'c':{
					printChar((char)args[argsIndex++],color);
				}break;
				case 's':{
					printStr((char*)args[argsIndex++],color);
				}break;
				case 'd':
				case 'i':{
					int32_t num = args[argsIndex++];
					char buf[32];
					syscall(core->irt,2,1,I32,num,(u32)buf);
					printStr(buf,color);
				}break;
				case 'u':{
					uint32_t num = args[argsIndex++];
					char buf[32];
					syscall(core->irt,2,1,U32,num,(u32)buf);
					printStr(buf,color);
				}break;
				case 'x':{
					uint32_t num = args[argsIndex++];
					char buf[32];
					syscall(core->irt,2,3,U32,num,(u32)buf);
					printStr(buf,color);
				}break;
				case 'b':{
					uint32_t num = args[argsIndex++];
					char buf[32];
					syscall(core->irt,2,4,U32,num,(u32)buf);
					printStr(buf,color);
				}break;
				case 'p':{
					uint32_t num = args[argsIndex++];
					char buf[32];
					syscall(core->irt,2,3,U32,num,(u32)buf);
					printChar('0',color);
					printChar('x',color);
					printStr(buf,color);
				}break;
				case '#':{
					index++;
					if(str[index]=='$'){
						color = DEFAULT_COLOR;
					}else if(is_hex_digit(str[index])){
						char txtColor = hex_to_int(str[index++]);
						if(is_hex_digit(str[index])){
							char bgColor = hex_to_int(str[index++]);
							color = ((bgColor<<4)&0xF0) | (txtColor&0xF);
						}else{
							color = (DEFAULT_COLOR&0xF0) | (txtColor&0xF);
						}
					}
					continue;
				}break;
				case '%':{
					printChar('%',color);
				}break;
				default:
					printChar('%',color);
					continue;
			};
		}else{
			printChar(str[index],color);
		}
		index++;
	}
}

void set_cursor(int offset) {
	if(offset!=0) offset /= 2;
	outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	outb(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
	outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	outb(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}
int get_cursor() {
	outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	int offset = inb(VGA_DATA_REGISTER) << 8;
	outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	offset += inb(VGA_DATA_REGISTER);
	return offset * 2;
}
void cursor(CoreData* core){
	int* args = (int*)core->ptr;
	int type = args[0];
	
	switch(type){
		case 0:{
			int cursorX = args[1]%MAX_COLS;
			int cursorY = args[2]%MAX_ROWS;
			int offset = cursorY*MAX_COLS*2+cursorX*2;
			set_cursor(offset);
		}break;
		case 1:{
			int cursorX = args[1];
			int cursorY = args[2];
			int offset = get_cursor();
			cursorY += (offset/2)/MAX_COLS;
			cursorX += (offset/2)%MAX_COLS;
			cursorY %= MAX_ROWS;
			cursorX %= MAX_COLS;
			offset = cursorY*MAX_COLS*2+cursorX*2;
			set_cursor(offset);
		}break;
		case 2:{
			int* cursorX = (int*)args[1];
			int* cursorY = (int*)args[2];
			int pos = get_cursor();
			*cursorY = (pos/2)/MAX_COLS;
			*cursorX = (pos/2)%MAX_COLS;
		}break;
	};
}

void clear(CoreData* core){
	for(uint16_t i = 0; i < MAX_COLS * MAX_ROWS; ++i){
		uint8_t *vidmem = (uint8_t *)VIDEO_ADDRESS;
		vidmem[i*2] = ' ';
		vidmem[i*2 + 1] = DEFAULT_COLOR;
	}
	set_cursor(0);
}

void putc(CoreData* core){
	int* args = (int*)core->ptr;
	char ch = args[0];
	printChar(ch,DEFAULT_COLOR);
}






















