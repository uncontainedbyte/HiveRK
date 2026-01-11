section .bee_header
magic_header_start:
    db 0x3A, 0x44, 0xBE, 0xE0; :D BEE
global bee_metadata
bee_metadata:
format: db 0
bee_id:
	dd 6
name_of_bee:
	db "PIT", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
priority:
	db 1
num_exports:
	dd 4
exports:
	dd 1
	dd sleep
	dd 2
	dd register_handler
	dd 3
	dd get_ticks
	dd 4
	dd set_frequency
init: dd bee_init
extern sleep
extern register_handler
extern get_ticks
extern set_frequency
extern bee_init
