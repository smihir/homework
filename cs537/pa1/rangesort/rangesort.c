
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
	int rc, j, set = 0;
    unsigned int i;
    unsigned int highval = 0, lowval = 1;
    unsigned int lowindex = 0, highindex;

    if (argc == 1)
        usage(argv[0]);

    while ((c = getopt(argc, argv, "i:o:l:h:")) != -1) {
	    switch (c) {
        case 'i':
            inFile = strdup(optarg);
            break;
        case 'o':
            outFile = strdup(optarg);
            break;
        case 'h':
            highval = atoi(optarg);
            break;
        case 'l':
            lowval = atoi(optarg);
            break;
        default:
            usage(argv[0]);
	    }
    }

    if (highval < lowval) {
	printf("higval is less than lowval\n");
	exit(1);
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

    highindex = ((fs.st_size)/sizeof(rec_t)) - 1;
    for (i = 0; i<=highindex; i++) {
        r = (rec_t *)(fp + i*(sizeof(rec_t)));

        if (r->key < lowval)
            continue;
        if (!set) {
            lowindex = i;
            set = 1;
        }

        if (r->key <= highval)
            continue;

        highindex = i;
        break;
    }

    printf("low %u, high %u\n", lowindex, highindex);
    // open and create output file
    fd = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) {
	perror("open");
	exit(1);
    }
	rc = write(fd, fp + lowindex*sizeof(rec_t), (highindex-lowindex)*sizeof(rec_t));
	if (rc != (highindex - lowindex)*sizeof(rec_t)) {
	    perror("write");
	    exit(1);
	    // should probably remove file here but ...
	}
    close(fd);

    free(fp);

    return 0;
}

