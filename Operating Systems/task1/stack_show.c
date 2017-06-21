#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

extern uint8_t __executable_start;
extern uint8_t __etext;
extern void *__libc_stack_end;

void stack_show() {

	uint64_t i = 5; // Variable created just to put something on stack...
	uint64_t *ptr = &i; // ...and get its address
	uint64_t address; // Address of function pointed by *ptr

	while ((void*) ptr <= __libc_stack_end) { // while ptr still points to stack

		if ((uint64_t*) *ptr >= (uint64_t*) &__executable_start && (uint64_t*) *ptr < (uint64_t*) &__etext) {
			// *ptr is possibly a legal address
			
			address = (uint64_t) *ptr;
			address -= 5; // to check if 5 bytes before address equal to value pointed by *ptr is byte 0xe8
			
			if (*(uint8_t*) address == 0xe8) {
				address += 1; // after e8 there is relative address of function
				// absolute address is a sum of relative address (4 bytes following e8) and return address (next 4 bytes)
				printf("%016lx\n", (uint64_t)*ptr + *(int32_t*) address); 
			}
		}

		ptr++;
	}

}

