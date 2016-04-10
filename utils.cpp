#include <stdio.h>
#include <stdint.h>
#include "utils.h"

void hexdump(uint8_t* p, unsigned int bytes){
	printf("Dump of address: %p, %u bytes", p, bytes);
	uint8_t* end = p+bytes;
	int counter = 0;
	while(p < end){
		if((counter & 0xf) == 0 ){ printf("\n  0x%04x:  ", counter);}
		printf(" %02x%02x", *p, *(p+1));
		p += 2;
		counter += 2;
	}
	printf("\n");
}
