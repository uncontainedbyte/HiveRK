
compile_c(){
	local filename=$(basename "$1")
	gcc -m32 -ffreestanding -c "$1" -o "binarys/${filename%.c}.o" -nostartfiles -fno-pic -fno-stack-protector
	if [ $? -eq 0 ]; then
		echo "Compiled ($1) into (binarys/${filename%.c}.o)"
	else
		echo "Failed to compile ($1)"
	fi
}
compile_asm16() {
	local filename=$(basename "$1")
	nasm "$1" -f bin -o "binarys/${filename%.asm}.bin"
	if [ $? -eq 0 ]; then
		echo "Compiled ($1) into (binarys/${filename%.asm}.bin)"
	else
		echo "Failed To Compile ($1)"
	fi
}
compile_asm32() {
	local filename=$(basename "$1")
	nasm "$1" -f elf -o "binarys/${filename%.asm}.o"
	if [ $? -eq 0 ]; then
		echo "Compiled ($1) into (binarys/${filename%.asm}.o)"
	else
		echo "Failed To Compile ($1)"
	fi
}
link_kernel(){
	echo "Linking kernel files into kernel.bin"
	local object_files=()
	for obj_file in "$@"; do
		# Prepend the 'binarys/' directory to each filename
		object_files+="binarys/$obj_file "
	done
	echo "${object_files[@]}"
	ld -m elf_i386 -o binarys/kernel.bin -T linker-script.lds binarys/kernel-entry.o ${object_files[@]} --oformat binary
}
create_floppy() {
	echo "Creating floppy.img"
	#dd if=/dev/zero of=blank.bin bs=1024 count=1440
	cat binarys/bootloader.bin binarys/stage2.bin binarys/kernel.bin bees/*.bee blank.bin> floppy.img
	cat binarys/bootloader.bin binarys/stage2.bin binarys/kernel.bin bees/*.bee> OS.img
}


compile_bee() {
	# Exit if any command fails
	set -e
	local filename=$(basename "$1")
	local bee_name=$1
	echo "--- Compiling Bee: ${bee_name} ---"
	
	# Step 1: Compile the C source file for the bee
	local c_file="${bee_name}.c"
	local c_obj="bees/${filename}.o"
	gcc -m32 -ffreestanding -c "$c_file" -o "$c_obj" -nostartfiles -fno-pic -fno-stack-protector
	
	# Step 2: Compile the Assembly metadata file for the bee
	local asm_file="${bee_name}_meta.asm"
	local asm_obj="bees/${filename}_meta.o"
	nasm -f elf "$asm_file" -o "$asm_obj"
	
	# Step 3: Link the object files into a single binary
	local linker_script="bees/${filename}-linker.lds"
	cat > "$linker_script" <<EOF
/* This is a temporary linker script for a single bee */
SECTIONS {
	. = 0x00000000;/* Set the starting address for the output file */
	.bee_header : {
		*(.bee_header)
	}
	.bee_metadata : {
		*(.bee_metadata)
	}
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
	.rodata : {
		*(.rodata)
	}
	.bss : {
		*(.bss)
	}
}
EOF
	
	local bin_file="bees/${filename}.bee"
	ld -m elf_i386 -T "$linker_script" -o "$bin_file" "$asm_obj" "$c_obj" --oformat binary
	
	# Step 4: Add the magic number to the end of the binary
	echo -ne "\x3A\x44\xBE\xE0" | dd of="$bin_file" bs=1 seek=$(stat -c%s "$bin_file") conv=notrunc
	
	# Step 5: Clean up temporary files
	rm "$c_obj" "$asm_obj" "$linker_script"
	
	echo "--- Successfully compiled ${bee_name} into ${bin_file} ---"
	
	# Reset exit on failure for the main script
	set +e
}
compile_bee_v2() {
	set -e
	
	local bee_dir="$1"
	local bee_name
	bee_name=$(basename "$bee_dir")
	
	echo "--- Compiling Bee: ${bee_name} ---"
	
	mkdir -p bees
	
	local objs=()
	local meta_obj=""
	
	#
	# Step 1: Compile all C files
	#
	for c_file in "$bee_dir"/*.c; do
		[ -e "$c_file" ] || continue
		local obj="bees/$(basename "$c_file").o"
		gcc -m32 -ffreestanding -c "$c_file" -o "$obj" \
			-nostartfiles -fno-pic -fno-stack-protector
		objs+=("$obj")
	done
	
	#
	# Step 2: Compile all ASM files
	#
	for asm_file in "$bee_dir"/*.asm; do
		[ -e "$asm_file" ] || continue
		local obj="bees/$(basename "$asm_file").o"
		nasm -f elf "$asm_file" -o "$obj"
		
		if [[ "$asm_file" == *_meta.asm ]]; then
			meta_obj="$obj"
		else
			objs+=("$obj")
		fi
	done
	
	if [[ -z "$meta_obj" ]]; then
		echo "ERROR: No *_meta.asm file found"
		exit 1
	fi
	#echo "${objs[@]}"
	#
	# Step 3: Linker script
	#
	local linker_script="bees/${bee_name}-linker.lds"
	cat > "$linker_script" <<EOF
SECTIONS {
	. = 0x00000000;
	
	.bee_header : { *(.bee_header) }
	.bee_metadata : { *(.bee_metadata) }
	
	.text : { *(.text*) }
	.rodata : { *(.rodata*) }
	.data : { *(.data*) }
	.bss : { *(.bss*) }
}
EOF
	
	#
	# Step 4: Link — META OBJECT FIRST
	#
	local bin_file="bees/${bee_name}.bee"
	ld -m elf_i386 -T "$linker_script" \
		-o "$bin_file" \
		"$meta_obj" "${objs[@]}" \
		--oformat binary
	
	#
	# Step 5: Append magic number
	#
	echo -ne "\x3A\x44\xBE\xE0" | \
		dd of="$bin_file" bs=1 seek=$(stat -c%s "$bin_file") conv=notrunc
	
	#
	# Step 6: Cleanup
	#
	rm -f bees/*.o "$linker_script"
	
	echo "--- Successfully compiled ${bee_name} into ${bin_file} ---"
	
	set +e
}




#compile_c "lib/memory_API.c"
#compile_c "kernel/arch/interrupt.c"
#compile_c "kernel/drivers/ports.c"
#compile_c "lib/util.c"
#compile_c "kernel/drivers/display.c"
#compile_asm32 "kernel/arch/asm_interrupt.asm"
#compile_c "kernel/drivers/keyboard.c"
#compile_c "lib/keyboard_API.c"




compile_bee_v2 "bees/display/"
#compile_bee_v2 "bees/interrupt/"
#compile_bee_v2 "bees/keyboard/"
#compile_bee_v2 "bees/PIT/"
#compile_bee_v2 "bees/memory/"

compile_bee_v2 "bees/util/"
compile_bee_v2 "bees/start/"

#compile_asm16 "queen/bootloader.asm"
#compile_asm16 "queen/stage2.asm"
#compile_asm32 "queen/kernel-entry.asm"

compile_c "queen/main.c"
#compile_asm32 "queen/main.asm"

link_kernel "main.o" #"memory_API.o" "interrupt.o" "ports.o" "util.o" "display.o" "asm_interrupt.o" "keyboard.o" "keyboard_API.o"


create_floppy
