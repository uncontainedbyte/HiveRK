







void int_to_string(int num, char *buffer) {
    // A temporary buffer to store the digits in reverse order.
    char temp_buffer[20];
    int i = 0;
    int sign = 0;

    // Handle the case where the number is 0.
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // Handle negative numbers.
    if (num < 0) {
        sign = 1;
        num = -num;
    }

    // Extract digits and store them in the temporary buffer.
    while (num > 0) {
        temp_buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Add the negative sign if the original number was negative.
    if (sign) {
        temp_buffer[i++] = '-';
    }

    // Add the null terminator to the temporary buffer.
    temp_buffer[i] = '\0';

    // Reverse the temporary buffer into the final buffer.
    int j = 0;
    while (i > 0) {
        buffer[j++] = temp_buffer[--i];
    }

    // Add the null terminator to the final buffer.
    buffer[j] = '\0';
}
void int_to_hex_string(int num, char *buffer) {
    // Temporary buffer to store hex digits in reverse order
    char temp_buffer[20];
    int i = 0;
    int sign = 0;

    // Handle zero
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        sign = 1;
        num = -num;
    }

    // Extract hex digits
    while (num > 0) {
        int digit = num % 16;
        if (digit < 10)
            temp_buffer[i++] = digit + '0';
        else
            temp_buffer[i++] = (digit - 10) + 'A'; // use 'a' for lowercase

        num /= 16;
    }

    // Add negative sign if needed
    if (sign) {
        temp_buffer[i++] = '-';
    }

    // Null-terminate temp buffer
    temp_buffer[i] = '\0';

    // Reverse into output buffer
    int j = 0;
    while (i > 0) {
        buffer[j++] = temp_buffer[--i];
    }

    // Null-terminate final buffer
    buffer[j] = '\0';
}
void SYSclear(){
	uint8_t* vidmem = (uint8_t*)0xb8000;
	for(int s=0;s<4000;s++){
		vidmem[s] = 0;
	}
}
void SYSprint(const char* str,int x,int y,char color){
	//color: 0=black 1=blue 2=green 3=cyan 4=red 5=magenta 6=brown 7=L-grey 8=D-grey
	//       9=L-blue A=L-green B=L-cyan C=L-red D=L-magenta E=L-brown F=white
	color = ((color>>4)&0xF)|((color<<4)&0xF0);
	uint8_t* vidmem = (uint8_t*)0xb8000;
	int i=0,vp=160*y+x*2,nl=0;
	while(str[i]!='\0'){
		if(str[i]=='\n'){i++;nl++;vp=160*y+x*2+nl*160;continue;}
		vidmem[vp] = str[i];
		vidmem[vp + 1] = color;
		i++;
		vp+=2;
	}
	
}
void SYSprintI(const int num,int x,int y,char color){
	char buf[20];
	int_to_string(num,buf);
	SYSprint(buf,x,y,color);
}
void SYSprintX(const int num,int x,int y,char color){
	char buf[20];
	int_to_hex_string(num,buf);
	SYSprint(buf,x,y,color);
}
void halt(){
	asm volatile("hlt");
}



