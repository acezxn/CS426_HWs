#include <stdio.h>
#include <stdlib.h>

int main() {
	// The malicious count value	
	unsigned int count = 0x80000000;

	// The item argument used in alloca()
	size_t item = count * sizeof(unsigned int);


  	printf("0x%x * 0x%x = 0x%x = %d\n", count, sizeof(unsigned int), item, item);
}
