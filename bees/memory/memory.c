#include "../beeUtil.h"
#include "../debug.h"


#define HEAP_SIZE  (1024*1024)  // 1MB
#define UNIT_SIZE  64
#define HEAP_TOTAL_UNITS (HEAP_SIZE / UNIT_SIZE)






typedef struct heap_block{
	uint16_t units;
	uint8_t  free;
	uint8_t  _pad;
	struct heap_block* next;
} heap_block_t;
_Static_assert(sizeof(heap_block_t) < UNIT_SIZE);
typedef struct{
	heap_block_t* heap;
} GlobalData;



uint16_t bytes_to_units(uint32_t payload_bytes){
	uint32_t total = payload_bytes + sizeof(heap_block_t);
	return (total + UNIT_SIZE - 1) / UNIT_SIZE;
}






void core_alloc(uint32_t size,void** data){
	syscall(getCoreIRT(),7,1,size,(u32)data);
}
void bee_init(CoreData* core){
	syscall(core->irt,0,1,7,sizeof(GlobalData));
	GlobalData* global;
	syscall(core->irt,0,2,7,(u32)&global);
	syscall(core->irt,0,4,(u32)&global->heap);
	
	global->heap = (heap_block_t*)(bytes_to_units((u32)global->heap)*UNIT_SIZE);
	
	global->heap->units = HEAP_TOTAL_UNITS;
	global->heap->free  = 1;
	global->heap->next  = 0;
	
	syscall(core->irt,0,5,(u32)core->baseAdr+(u32)core_alloc);
}
void kmalloc(CoreData* core,GlobalData* global){
	u32* args = (u32*)core->ptr;
	u32 size = args[0];
	void** data = (void**)args[1];
	
	if(size == 0){
		*data = 0;
		return;
	}
	
	uint16_t req_units = bytes_to_units(size);
	heap_block_t* cur = global->heap;
	
	while(cur){
		if(cur->free && cur->units >= req_units){
			if(cur->units > req_units){///split block into 2
				heap_block_t* split = (heap_block_t*)((uint8_t*)cur + req_units * UNIT_SIZE);
				
				split->units = cur->units - req_units;
				split->free  = 1;
				split->next  = cur->next;
				
				cur->units = req_units;
				cur->next  = split;
			}
			
			cur->free = 0;
			*data = ((uint8_t*)cur + sizeof(heap_block_t));
			return;
		}
		cur = cur->next;
	}
	
	*data = 0;
}
void kfree(CoreData* core,GlobalData* global){
	u32* args = (u32*)core->ptr;
	void* ptr = (void*)args[0];
	
	if(!ptr) return;
	
	heap_block_t* block = (heap_block_t*)((uint8_t*)ptr - sizeof(heap_block_t));
	
	if(block->free){
		while (1);
	}
	
	block->free = 1;
	
	if(block->next && block->next->free){
		block->units += block->next->units;
		block->next = block->next->next;
	}
	
	heap_block_t* last=0;
	heap_block_t* cur = global->heap;
	while(cur!=block){
		last = cur;
		cur = cur->next;
	}
	if(last!=0 && last->free){
		last->units += block->units;
		last->next = block->next;
	}
}


































































/*
void paging_init(GlobalData* global,void* bump_end) {
	uint32_t used_pages = ( (uint32_t)bump_end + PAGE_SIZE - 1) / PAGE_SIZE;
	uint32_t phys = used_pages*PAGE_SIZE;
	
	global->page_directory = (uint32_t*)phys;
	phys += PAGE_SIZE;
	
	uint32_t* id_table = (uint32_t*)phys;
	phys += PAGE_SIZE;
	
	global->page_table = (uint32_t*)phys;
	phys += PAGE_SIZE;
	
	for(int i = 0; i < 1024; i++){
		global->page_directory[i] = 0;
		id_table[i] = 0;
		global->page_table[i] = 0;
	}
	for(int i = 0; i < 256; i++){
		id_table[i] = i * PAGE_SIZE | 3; // PRESENT | WRITABLE
	}
	global->page_directory[0] = (uint32_t)id_table | 3;
	
	uint32_t heap_phys = phys; // start of heap physical memory (after page tables)
	for (int i = 0; i < HEAP_PAGES; i++) {
		global->page_table[i] = heap_phys | 3; // PRESENT | WRITABLE
		heap_phys += PAGE_SIZE;
	}
	uint32_t pd_index = HEAP_VADDR >> 22;
	global->page_directory[pd_index] = (uint32_t)global->page_table | 3;
	
	uint32_t pd_phys = (uint32_t)global->page_directory;
	asm volatile(
		"mov %0, %%cr3\n"
		"mov %%cr0, %%eax\n"
		"or $0x80000000, %%eax\n"  // set PG bit
		"mov %%eax, %%cr0\n"
		:: "r"(pd_phys)
		: "eax"
	);
	
	global->heap_start = (void*)HEAP_VADDR;
}
*/
