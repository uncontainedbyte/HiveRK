



[bits 16]
[org 0x1000]

_start:;{
	mov [BOOT_DRIVE], al
	
	mov si,spacer_msg
	call print_string
	mov si,loading_kernel_msg
	call print_string
	
	call load_kernel
	
	mov si,ld_switch_msg
	call print_string
	
	call scan_hardware
	
	cli; Disable interrupts
	
	lgdt [GDT_POINTER]; Load the GDT
	
	; Set the PE (Protected Mode Enable) bit in CR0
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	
	jmp CODE_SEG:mode32b_start; Far jump
	;jmp $
;}



scan_hardware:;{
	
	call scan_ram
	
	
	
	
	
	
	
	
	
	ret
;}

scan_ram:;{
	
	; Initialize loop counter and buffer pointer
	xor ebx, ebx            ; EBX must be 0 for the first call
	mov edi, 0x800         ; Start of the buffer to store memory regions
	mov ax, ds
	mov es, ax              ; ES must point to the segment of EDI
	
.get_next_region:
	; Set up registers for the BIOS call
	mov eax, 0xE820
	mov edx, 0x534D4150     ; "SMAP" signature
	mov ecx, 24             ; Size of the buffer to fill
	
	; Call the interrupt. The memory region data is stored at ES:DI
	int 0x15
	
	; Check the carry flag for errors or end of list
	jc .end_loop
	
	; The returned EBX is the continuation value.
	; If EBX is 0, we're done.
	cmp ebx, 0
	je .end_loop
	
	; The BIOS call already wrote the 24 bytes to ES:DI.
	; We just need to advance our buffer pointer for the next entry.
	add edi, 24
	
	; Loop again
	jmp .get_next_region
	
.end_loop:
	
	
	ret
;}










load_kernel:;{
	
	; The destination address for the read operation must be in ES:BX.
	; KERNEL_LOAD_ADDRESS is 0x90000.
	; This translates to a segment:offset of 0x9000:0x0000.
	mov ax, KERNEL_LOAD_ADDRESS >> 4  ; Get the segment (0x9000)
	mov es, ax
	mov bx, KERNEL_LOAD_ADDRESS & 0xF ; Get the offset (0x0000)
	mov ax,0
	mov al, KERNEL_SECTORS_TO_LOAD ; Number of sectors to read
	push ax
	mov ah, 0x02                 ; Function: Read Sectors
	mov ch, 0                    ; Cylinder number (your kernel starts on cylinder 0)
	mov cl, KERNEL_START_SECTOR  ; Starting sector number
	mov dh, 0                    ; Head number (head 0)
	mov dl, [BOOT_DRIVE]         ; Drive number from bootloader
	
	int 0x13   
	
	jc .load_error      ; If the carry flag is set, an error occurred
	pop dx
	cmp al,dl
	jne .partal_load_error
	
	ret                 ; Return to the 32-bit code that called us
	
.load_error:
	mov si, loading_error_msg
	call print_string   ; You can use a print_string function from your bootloader
	jmp $
.partal_load_error:
	mov si, partal_loading_error_msg
	call print_string   ; You can use a print_string function from your bootloader
	jmp $
;}
; Data for the error message
loading_error_msg db "Error: Failed to load kernel!", 10, 13, 0
partal_loading_error_msg db "Error: Failed to load entire kernel!", 10, 13, 0


spacer_msg db "----------------------------------------------------",10,13, 0
ld_switch_msg db "Switching To 32-Bit Mode...",10,13, 0
loading_kernel_msg db "Loading Kernel...",10,13, 0

print_string:;{
	mov ah, 0x0E      ; Teletype output function
.next_char:
	lodsb             ; Load byte at DS:SI into AL and increment SI
	cmp al, 0        ; Check if it's the null terminator
	je .done         ; If so, jump to done
	int 0x10         ; Call BIOS interrupt to print character in AL
	jmp .next_char   ; Repeat for the next character
.done:
	ret               ; Return from function
;}









[bits 32]
mode32b_start:;{
	; Reload all segment registers with the new 32-bit data segment
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	; Set up a stack
	mov esp, 0x90000          ; A common stack location
	
	call KERNEL_LOAD_ADDRESS
	jmp $
;}








GDT_START:
  GDT_NULL: db 0, 0, 0, 0, 0, 0, 0, 0
  GDT_CODE: db 0xFF, 0xFF, 0x00, 0x00, 0x00, 10011010b, 11001111b, 0x00
  GDT_DATA: db 0xFF, 0xFF, 0x00, 0x00, 0x00, 10010010b, 11001111b, 0x00
GDT_END:
GDT_POINTER:
  dw GDT_END - GDT_START - 1
  dd GDT_START

CODE_SEG equ GDT_CODE - GDT_START
DATA_SEG equ GDT_DATA - GDT_START



; Define constants for the kernel's location and size
KERNEL_LOAD_ADDRESS    equ 0x10000
KERNEL_START_SECTOR    equ 0x3       ; LBA address of the first sector of your kernel binary
KERNEL_SECTORS_TO_LOAD equ 128        ; Number of sectors to load (adjust as needed)
BOOT_DRIVE db 0

times 512 - ($ - $$) db 0






;times 2048 db 0
;times 2048 db 0
;times 2048 db 0
;times 2048 db 0
;times 2048 db 0
