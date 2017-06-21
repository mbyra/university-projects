#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
	int filedsc = open("/dev/helloN", O_RDWR);
	if (write(filedsc, "siema eniu!", 11) != 11) {
		printf("less characters than requested was written\n");
	}
	lseek(filedsc, 15, SEEK_SET);

	if (write(filedsc, "LUBIE MISIE", 11) != 11) {
		printf("less characters than requested was written\n");
	}

}