section .bee_header
magic_header_start:
    db 0x3A, 0x44, 0xBE, 0xE0; :D BEE
global bee_metadata
bee_metadata:
format: db 0
bee_id:
	dd 7
name_of_bee:
	db "memory-manager", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
priority:
	db 0
num_exports:
	dd 2
exports:
	dd 1
	dd kmalloc
	dd 2
	dd kfree
init: dd bee_init
extern kmalloc
extern kfree
extern bee_init
