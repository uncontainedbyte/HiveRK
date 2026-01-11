section .bee_header
magic_header_start:
    db 0x3A, 0x44, 0xBE, 0xE0; :D BEE
global bee_metadata
bee_metadata:
format: db 0
bee_id:
	dd 4
name_of_bee:
	db "interrupt", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
priority:
	db 0
num_exports:
	dd 1
exports:
	dd 1
	dd registerInterrupt
init: dd bee_init
extern registerInterrupt
extern bee_init
