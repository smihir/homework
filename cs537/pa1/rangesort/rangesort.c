
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/stat.h>
#include <errno.h>

void
usage(char *prog) 
{
    fprintf(stderr, "usage: %s <-i inputfile> <-o outputfile> <-l lowvalue> "
                    "<-h highvalue>\n", prog);
    exit(1);
}

int
compar_f(const void *a, const void *b)
{
    if (((rec_t *)a)->key < ((rec_t *)b)->key)
        return -1;
    else if (((rec_t *)a)->key == ((rec_t *)b)->key)
        return 0;
    else
        return 1;
}

int
main(int argc, char *argv[])
{
    // arguments
    char *inFile = "/no/such/file";
    char *outFile = "/no/such/file";
    // input params
    int c, fd;
    opterr = 0;
    struct stat fs;
    rec_t *r;
    char *fp;
	int rc, i, j;

    while ((c = getopt(argc, argv, "i:o:")) != -1) {
	    switch (c) {
        case 'i':
            inFile = strdup(optarg);
            break;
        case 'o':
            outFile = strdup(optarg);
            break;
        default:
            usage(argv[0]);
	    }
    }

    // open and create output file
    fd = open(inFile, O_RDONLY);
    if (fd < 0) {
	perror("open");
	exit(1);
    }

    if (fstat(fd, &fs) == -1) {
        exit(errno);
    }

    fp = (char *)malloc(fs.st_size);

	rc = read(fd, fp, fs.st_size);
	if (rc < 0) {
	    perror("read");
	    exit(1);
	}
    (void) close(fd);

    if (fp == NULL) {
        exit(1);
    }

    qsort(fp, fs.st_size / sizeof(rec_t), sizeof(rec_t), compar_f);
#ifdef DEBUG
    for (i = 0; i<=(fs.st_size -  sizeof(rec_t)); i += sizeof(rec_t)) {
        r = (rec_t *)(fp + i);

        printf("key: %u rec:", r->key);
        for (j = 0; j < NUMRECS; j++) 
            printf("%u ", r->record[j]);
        printf("\n");
    }
#endif

    // open and create output file
    fd = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) {
	perror("open");
	exit(1);
    }
	rc = write(fd, fp, fs.st_size);
	if (rc != fs.st_size) {
	    perror("write");
	    exit(1);
	    // should probably remove file here but ...
	}
    close(fd);

    free(fp);

    return 0;
}

