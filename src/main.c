#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#define PRG_NAME "File2Code"
#define PRG_VERSION "0.2"
#define PRG_YEAR "2013-2014"
#define PRG_AUTHOR "Stephan Laukien"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	/* show copyright */
	printf ("%s %s (%s)\n", PRG_NAME, PRG_VERSION, __DATE__);
	printf ("(c) %s by %s\n\n", PRG_YEAR, PRG_AUTHOR);
	
	/* get argument */
	if (argc == 1) {
		fprintf(stderr, "ERROR: no argument");
		return EXIT_FAILURE;
	}
	char *filename = strdup(argv[1]);

	/*  open file */
	int fd;
#ifdef __WIN32
	fd = open(filename, O_RDONLY|O_BINARY);
#else
	fd = open(filename, O_RDONLY);
#endif
	if (fd == -1) {
		fprintf(stderr, "ERROR: unable to read file (%s)", filename);
		free(filename);
		return EXIT_FAILURE;
	}

	/* find last separator */
	char *sep;
#ifdef __WIN32
	sep = strrchr(filename, '\\');
#else
	sep = strrchr(filename, '/');
#endif
	if (!sep) {
		sep = filename;
	}

	/* codename */
	char *codename = (char *)malloc(strlen(sep) + 2 + 1);
	strcpy(codename, filename);
	strcat(codename, ".c");

	/* create code */
	FILE *code;
	code = fopen(codename, "w");
	if (!code) {
		fprintf(stderr, "ERROR: unable to create file (%s)", codename);
		close(fd);
		free(codename);
		free(filename);
		return EXIT_FAILURE;
	}

	/* write header */
	char *charname = strdup(sep);
	int i;
	for (i = 0; i < strlen(charname); ++i) {
		if (!isalnum(charname[i])) {
			charname[i] = '_';
		}
	}

	fprintf(code, "unsigned char %s_data[] = {\n", charname);
	
	char buf[BUFFER_SIZE];
	int len;
	unsigned long size = 0;
	while ((len = read(fd, buf, BUFFER_SIZE))) {
		if (len == 0) {
			break;
		}

		/* data */
		int i;
		for (i = 0; i < len; ++i) {
			fprintf(code, "%d", buf[i]);
			if (len != BUFFER_SIZE && i < len -1) {
				fprintf(code,", ");
			}
		}

		/* size */
		size += len;
	}

	fprintf(code, "\n};\n");
	fprintf(code, "unsigned long %s_size = %ld;\n", charname, size);

	printf ( "CREATE: %s (%ld)\n", codename, size );

	/* close files */
	fclose(code);
	close(fd);

	/* free memory */
	free(charname);
	free(codename);
	free(filename);

	return EXIT_SUCCESS;
}
