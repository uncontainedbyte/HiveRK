; === BEE METADATA AND HEADER TEMPLATE ===
;
; This file defines the low-level data for a bee, including the
; magic numbers, metadata, and pointers to the C functions.
;
; How to use:
; 1. Change the 'name_of_bee' constant.
; 2. Update the 'exports' list.
; 3. In the C file, implement the functions declared here.
;

section .bee_header
; Magic number (4 bytes) to identify the start of a bee binary.
magic_header_start:
;    db 0xDE, 0xAD, 0xBE, 0xEF
    db 0x3A, 0x44, 0xBE, 0xE0; :D BEE

global bee_metadata
bee_metadata:
; value for future changes/additions without breaking old bees
format: db 0
; unique ID for every bee, cant be 0
bee_id:
	dd 3
; The name of the bee (up to 32 bytes)
; Padded with null characters
name_of_bee:
	db "system_start", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; The priority level for loading (0 is highest)
priority:
	db 255

; The number of exported functions
num_exports:
	dd 0
; A list of exported function ids (4 bytes each) and their pointers
exports:

; The entry point for the bee
; The Queen will call this, which then jumps to the C init function.
init: dd bee_init

; === EXTERNAL C FUNCTIONS ===
; Declare external C functions that are used in this file.
extern bee_init
