#include <stdint.h>



#define CORE_GLOBAL_MEMORY 0x6000



#define ASMcall(ptr,args_size) \
	asm volatile( \
			"call *%0\n\t" \
			"addl %1, %%esp" \
			:: "r"(ptr), "r"(args_size) : "eax", "ebx", "ecx", "edx", "memory"); \




#define ASMpush4(val) \
	asm volatile("pushl %0" :: "r"(val) : "memory")
#define ASMpushU4(a) ASMpush4(a)
#define ASMpushI4(a) ASMpush4(a)
#define ASMpush1U4(a) ASMpush4(a)
#define ASMpush2U4(a,b) { ASMpush4(b); ASMpush4(a); }
#define ASMpush3U4(a,b,c) { ASMpush4(c); ASMpush4(b); ASMpush4(a); }
#define ASMpush1I4(a) ASMpush1U4(a)
#define ASMpush2I4(a,b) ASMpush2U4(a,b)
#define ASMpush3I4(a,b,c) ASMpush3U4(a,b,c)

#define ASMpushU2(val) \
	asm volatile("movzwl %0, %%eax\n\t" "pushl %%eax" :: "r"(val) : "eax", "memory")
#define ASMpushI2(val) \
	asm volatile("movswl %0, %%eax\n\t" "pushl %%eax" :: "r"(val) : "eax", "memory")
#define ASMpush1U2(a) ASMpushU2(a)
#define ASMpush2U2(a,b) { ASMpushU2(b); ASMpushU2(a); }
#define ASMpush3U2(a,b,c) { ASMpushU2(c); ASMpushU2(b); ASMpushU2(a); }
#define ASMpush1I2(a) ASMpushI2(a)
#define ASMpush2I2(a,b) { ASMpushI2(b); ASMpushI2(a); }
#define ASMpush3I2(a,b,c) { ASMpushI2(c); ASMpushI2(b); ASMpushI2(a); }

#define ASMpushU1(val) \
	asm volatile("movzbl %0, %%eax\n\t" "pushl %%eax" :: "r"(val) : "eax", "memory")
#define ASMpushI1(val) \
	asm volatile("movsbl %0, %%eax\n\t" "pushl %%eax" :: "r"(val) : "eax", "memory")
#define ASMpush1U1(a) ASMpushU1(a)
#define ASMpush2U1(a,b) { ASMpushU1(b); ASMpushU1(a); }
#define ASMpush3U1(a,b,c) { ASMpushU1(c); ASMpushU1(b); ASMpushU1(a); }
#define ASMpush1I1(a) ASMpushI1(a)
#define ASMpush2I1(a,b) { ASMpushI1(b); ASMpushI1(a); }
#define ASMpush3I1(a,b,c) { ASMpushI1(c); ASMpushI1(b); ASMpushI1(a); }

#define ASMpush8(val) \
	asm volatile( \
		"pushl %1\n\t" \
		"pushl %0" \
		:: "r"((uint32_t)(val)), \
		   "r"((uint32_t)((uint64_t)(val) >> 32)) \
		: "memory")
#define ASMpushU8(a) ASMpush8(a)
#define ASMpushI8(a) ASMpush8(a)
#define ASMpush1U8(a) ASMpush8(a)
#define ASMpush2U8(a,b) { ASMpush8(b); ASMpush8(a); }
#define ASMpush3U8(a,b,c) { ASMpush8(c); ASMpush8(b); ASMpush8(a); }
#define ASMpush1I8(a) ASMpush1U8(a)
#define ASMpush2I8(a,b) ASMpush2U8(a,b)
#define ASMpush3I8(a,b,c) ASMpush3U8(a,b,c)


#define ASMeaxS(val) \
	asm volatile("movl %0, %%eax" :: "r"(val) : "eax")
#define ASMebxS(val) \
	asm volatile("movl %0, %%ebx" :: "r"(val) : "ebx")
#define ASMecxS(val) \
	asm volatile("movl %0, %%ecx" :: "r"(val) : "ecx")
#define ASMedxS(val) \
	asm volatile("movl %0, %%edx" :: "r"(val) : "edx")
#define ASMeaxG(out) \
	asm volatile("movl %%eax, %0" : "=r"(out))
#define ASMebxG(out) \
	asm volatile("movl %%ebx, %0" : "=r"(out))
#define ASMecxG(out) \
	asm volatile("movl %%ecx, %0" : "=r"(out))
#define ASMedxG(out) \
	asm volatile("movl %%edx, %0" : "=r"(out))
#define ASMesiG(out) \
	asm volatile("movl %%esi, %0" : "=r"(out))
#define ASMediG(out) \
	asm volatile("movl %%edi, %0" : "=r"(out))
#define ASMebpG(out) \
	asm volatile("movl %%ebp, %0" : "=r"(out))
#define ASMespG(out) \
	asm volatile("movl %%esp, %0" : "=r"(out))


#define syscall(addr, ID, CMD, ...) \
	{ \
		uint32_t __args[] = { __VA_ARGS__ }; \
		asm volatile( \
			"call *%0" : \
			: "r"(addr), "a"(ID), "b"(CMD), "c"(0), "d"((uint32_t)__args) \
			: "memory"); \
	}



#define ASMinb(port,output) \
	asm("in %%dx, %%al" : "=a" (output) : "d" (port));
#define ASMoutb(port,data) \
	asm("out %%al, %%dx" : : "a" (data), "d" (port));
#define ASMinw(port,output) \
	asm("in %%dx, %%ax" : "=a" (output) : "d" (port));
#define ASMoutw(port,data) \
	asm("out %%ax, %%dx" : : "a" (data), "d" (port));
unsigned char inb(uint16_t port){
	unsigned char result;
	asm("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}
void outb(uint16_t port, uint8_t data){
	asm("out %%al, %%dx" : : "a" (data), "d" (port));
}
unsigned short inw(uint16_t port){
	unsigned short result;
	asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}
void outw(uint16_t port, uint16_t data){
	asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}

uint32_t getCoreIRT(){
	return ((uint32_t*)CORE_GLOBAL_MEMORY)[0];
}










typedef struct{
	uint32_t irt;     // address of custom interrupt handler
	void*    baseAdr; // base address of module to fix strings
	uint32_t count;   // number of args passed
	void*    ptr;     // pointer to array of args
} CoreData;



//#define U64 0x5540 // 55='U' 40=64bit
//#define I64 0x4940 // 49='I' 40=64bit
#define U32 0x5520 // 55='U' 20=32bit
#define I32 0x4920 // 49='I' 20=32bit
#define U16 0x5510 // 55='U' 10=16bit
#define I16 0x4910 // 49='I' 10=16bit
#define U8  0x5508 // 55='U' 08= 8bit
#define I8  0x4908 // 49='I' 08= 8bit

//typedef  int64_t i64;
//typedef uint64_t u64;
typedef  int32_t i32;
typedef uint32_t u32;
typedef  int16_t i16;
typedef uint16_t u16;
typedef   int8_t i8;
typedef  uint8_t u8;

#define fixptr (u32)core->baseAdr











//#define ASMcallPtr(ptr) \
	//asm volatile("call *%0" :: "r"(ptr) : "memory")
//#define ASMaddEsp(bytes) \
	asm volatile("addl $" #bytes ", %%esp" ::: "esp", "memory")















