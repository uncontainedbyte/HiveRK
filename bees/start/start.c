#include "../beeUtil.h"

#define DisplayBufferSIZE 32000 // 160*200=32000B


typedef struct{
	char* disBuf;
	uint32_t line_count;
	uint32_t cursor_x;
	uint32_t cursor_y;
	uint32_t scroll_offset;
} CombData;
typedef struct{
	CombData* comb;
	
} globalDataStruct;


void bee_init(CoreData* core){
	syscall(core->irt,0,1,3,sizeof(globalDataStruct));
	globalDataStruct* global;
	syscall(core->irt,0,2,3,(u32)&global);
	
	syscall(core->irt,7,1,sizeof(CombData),(u32)&global->comb);
	syscall(core->irt,7,1,DisplayBufferSIZE,(u32)&global->comb->disBuf);
	
	
	
	
	syscall(core->irt,1,2);
	//syscall(core->irt,2,6,0xb8000,0x02,4000-2);
	
	syscall(core->irt,1,4,0x02);
	
	
	
	//asm volatile("int $6");
	
	int ch=0,isData=0;
	while(1){
		syscall(core->irt,6,1,500);
		//syscall(core->irt,5,1,(u32)&isData);
		//if(isData){
			//syscall(core->irt,5,2,(u32)&ch);
			//ch&=0xFF;
			//if(!(ch&0x01000000))
				//syscall(core->irt,1,1,fixptr+(u32)"\ntyped: %x",ch);
		//}
	}
}






































