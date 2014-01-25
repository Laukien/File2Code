#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern unsigned char README_data[];
extern unsigned long README_size;

int main() {

	char *readme = (char *)malloc(README_size + 1);
	memcpy(readme, README_data, README_size);
	readme[README_size] = '\0';
	printf ( "%s\n", readme );
	free(readme);

	return EXIT_SUCCESS;
}

