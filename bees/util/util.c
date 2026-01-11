
#include "../beeUtil.h"



void bee_init(CoreData* core){}

void num_to_str_unsigned(uint32_t num,char* buf){
	char temp_buffer[32];
	int i = 0;
	
	if(num == 0){
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}
	
	while(num > 0){
		temp_buffer[i++] = (num % 10) + '0';
		num /= 10;
	}
	
	temp_buffer[i] = '\0';
	
	int j = 0;
	while(i > 0){
		buf[j++] = temp_buffer[--i];
	}
	
	buf[j] = '\0';
}
void num_to_str_signed(int32_t num,char* buf,uint32_t min,uint32_t max,uint32_t size){
	char temp_buffer[32];
	int i = 0;
	char sign = 0;
	char add1 = 0;
	
	num = num&size;
	
	if(num == 0){
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}
	
	if(num == min){
		sign = 1;
		add1 = 1;
		num = max;
	}else if(num < 0){
		sign = 1;
		num = -num;
	}
	
	while(num > 0){
		temp_buffer[i++] = (num % 10) + '0';
		num /= 10;
	}
	
	if(sign){
		temp_buffer[i++] = '-';
	}
	
	temp_buffer[i] = '\0';
	
	int j = 0;
	while(i > 0){
		buf[j++] = temp_buffer[--i];
	}
	
	buf[j] = '\0';
	buf[j-1] += add1;
}
void num_to_str(CoreData* core){
	int* args = (int*)core->ptr;
	int type = args[0];
	
	switch(type){
		case I32:{
			int32_t num = args[1];
			char* buf = (char*)args[2];
			num_to_str_signed(num,buf,-2147483648,2147483647,0xFFFFFFFF);
		}break;
		case U32:{
			uint32_t num = args[1];
			char* buf = (char*)args[2];
			num_to_str_unsigned(num,buf);
		}break;
		case I16:{
			int16_t num = args[1];
			char* buf = (char*)args[2];
			num_to_str_signed(num,buf,-32768,32767,0x0000FFFF);
		}break;
		case U16:{
			uint16_t num = args[1];
			char* buf = (char*)args[2];
			num_to_str_unsigned(num,buf);
		}break;
		case I8:{
			int8_t num = args[1];
			char* buf = (char*)args[2];
			num_to_str_signed(num,buf,-128,127,0x000000FF);
		}break;
		case U8:{
			uint8_t num = args[1];
			char* buf = (char*)args[2];
			num_to_str_unsigned(num,buf);
		}break;
	};
}

void pow(CoreData* core){
	int* args = (int*)core->ptr;
	int type = args[0];
	
	switch(type){
		case I32:{
			int32_t* num1 = (int32_t*)(args[1]);
			int32_t num2 = args[2];
			if(num2<0){ *num1 = 0; }
			for(int32_t s=0;s<num2-1;s++){
				*num1 += *num1;
			}
		}break;
		case U32:{
			uint32_t* num1 = (uint32_t*)(args[1]);
			uint32_t num2 = args[2];
			if(num2<0){ *num1 = 0; }
			for(uint32_t s=0;s<num2-1;s++){
				*num1 += *num1;
			}
		}break;
		case I16:{
			int16_t* num1 = (int16_t*)(args[1]);
			int16_t num2 = args[2];
			if(num2<0){ *num1 = 0; }
			for(int16_t s=0;s<num2-1;s++){
				*num1 += *num1;
			}
		}break;
		case U16:{
			uint16_t* num1 = (uint16_t*)(args[1]);
			uint16_t num2 = args[2];
			if(num2<0){ *num1 = 0; }
			for(uint16_t s=0;s<num2-1;s++){
				*num1 += *num1;
			}
		}break;
		case I8:{
			int8_t* num1 = (int8_t*)(args[1]);
			int8_t num2 = args[2];
			if(num2<0){ *num1 = 0; }
			for(int8_t s=0;s<num2-1;s++){
				*num1 += *num1;
			}
		}break;
		case U8:{
			uint8_t* num1 = (uint8_t*)(args[1]);
			uint8_t num2 = args[2];
			if(num2<0){ *num1 = 0; }
			for(uint8_t s=0;s<num2-1;s++){
				*num1 += *num1;
			}
		}break;
	}
}

void num_to_hex(CoreData* core){
	int* args = (int*)core->ptr;
	int type = args[0];
	uint32_t num = args[1];
	char* buf = (char*)args[2];
	char* hex = (u32)core->baseAdr+"0123456789ABCDEF";
	
	switch(type){
		case I32:
		case U32:{}break;//already limited
		case I16:
		case U16:{
			num &= 0x0000FFFF;
		}break;
		case I8:
		case U8:{
			num &= 0x000000FF;
		}break;
	}
	
	char temp_buffer[32];
	int i = 0;
	
	if(num == 0){
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}
	
	while(num > 0){
		temp_buffer[i++] = hex[(num % 16)];
		num /= 16;
	}
	
	switch(type){
		case I32:
		case U32:{ while(i<8){temp_buffer[i++] = '0';} }break;
		case I16:
		case U16:{ while(i<4){temp_buffer[i++] = '0';} }break;
		case I8:
		case U8:{ while(i<2){temp_buffer[i++] = '0';} }break;
	}
	
	temp_buffer[i] = '\0';
	
	int j = 0;
	while(i > 0){
		buf[j++] = temp_buffer[--i];
	}
	
	buf[j] = '\0';
}

void num_to_bin(CoreData* core){
	int* args = (int*)core->ptr;
	int type = args[0];
	uint32_t num = args[1];
	char* buf = (char*)args[2];
	
	char bitness;
	switch(type){
		case I32:
		case U32:{bitness=32;}break;//already limited
		case I16:
		case U16:{bitness=16;
			num &= 0x0000FFFF;
		}break;
		case I8:
		case U8:{bitness=8;
			num &= 0x000000FF;
		}break;
	}
	
	char temp_buffer[70];
	int i = 0;
	
	if(num == 0){
		for(int s=0;s<bitness;s++){
			buf[s] = '0';
		}
		buf[bitness] = '\0';
		return;
	}
	
	while(num > 0){
		temp_buffer[i++] = (num % 2) + '0';
		num /= 2;
	}
	
	while(i<bitness){temp_buffer[i++] = '0';}
	
	temp_buffer[i] = '\0';
	
	int j = 0;
	while(i > 0){
		buf[j++] = temp_buffer[--i];
	}
	
	buf[j] = '\0';
}

void memcpy(CoreData* core){
	int* args = (int*)core->ptr;
	u8* dst = (u8*)(args[0]);
	u8* src  = (u8*)(args[1]);
	u32 size = (u32)(args[2]);
	
	for(u32 i = 0; i < size; i++){
		dst[i] = src[i];
	}
}
void memset(CoreData* core){
	int* args = (int*)core->ptr;
	u8* ptr = (u8 *)(args[0]);
	u8  value = (u8)(args[1]);
	u32 size = args[2];
	
	for(u32 i = 0; i < size; i++){
		ptr[i] = value;
	}
}






















