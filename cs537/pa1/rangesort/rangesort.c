#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

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
    int c, fd, pid, status, psize;
    opterr = 0;
    struct stat fs;
    rec_t *r, *rn1, *rn2;
    char *fp, *dst, *f, *fp1, *fp2;
	int rc, j, set = 0, count1 = 0, count2 = 0;
    unsigned int i;
    long unsigned int highval = 0, lowval = 1, median;
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
            highval = strtoul(optarg, (char **)NULL, 10);
            break;
        case 'l':
            lowval = strtoul(optarg, (char **)NULL, 10);
            break;
        default:
            usage(argv[0]);
	    }
    }

    if (highval < lowval) {
	printf("higval is less than lowval\n");
	exit(1);
    }

    if (highval > UINT_MAX || lowval > UINT_MAX) {
	printf("higval or lowval is greated than UINT_MAX\n");
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

    fp1 = (char *)malloc(fs.st_size);
    fp2 = (char *)malloc(fs.st_size);

#if 0
	rc = read(fd, fp, fs.st_size);
	if (rc < 0) {
	    perror("read");
	    exit(1);
	}
#endif
    f = mmap(NULL, fs.st_size, PROT_READ, MAP_SHARED, fd, 0);
    madvise(f, fs.st_size, MADV_SEQUENTIAL);

    if (f == NULL) {
        exit(1);
    }
    highindex = ((fs.st_size)/sizeof(rec_t)) - 1;
    unsigned int max = 0, min = 0;
    for (i = 0; i<=highindex; i++) {
        r = (rec_t *)(f + i*(sizeof(rec_t)));

        if (r->key < lowval || r->key > highval)
            continue;
        if(r->key > max)
            max = r->key;
    }
    median = max/2;
    for (i = 0; i<=highindex; i++) {
        r = (rec_t *)(f + i*(sizeof(rec_t)));
        rn1 = (rec_t *)(fp1 + count1*(sizeof(rec_t)));
        rn2 = (rec_t *)(fp2 + count2*(sizeof(rec_t)));

        if (r->key < lowval || r->key > highval)
            continue;

        if (r->key < median) {
            memcpy(rn1, r, sizeof(rec_t));
            count1++;
        } else {
            memcpy(rn2, r, sizeof(rec_t));
            count2++;
        }

    }

    printf("count1 %d, count2 %d total %d %lu\n", count1, count2, count1+ count2 +2, sizeof(unsigned int));
    munmap(f, fs.st_size);
    (void) close(fd);

    lowindex = 0;

    // open and create output file
    fd = open(outFile, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) {
	perror("open");
	exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork() error");
        exit(1);
    } else if (pid == 0) {
        printf("parent start\n");
        qsort(fp1, count1, sizeof(rec_t), compar_f);
        rc = pwrite(fd, fp1, (count1) * sizeof(rec_t) , 0);
        if (rc != (count1) * sizeof(rec_t)) {
            perror("main write");
            exit(1);
        }
        printf("parent end\n");
    } else {
        printf("pch start\n");
        qsort(fp2, count2, sizeof(rec_t), compar_f);
        rc = pwrite(fd, fp2, (count2) * sizeof(rec_t), (count1) * sizeof(rec_t));
        if (rc != (count2) * sizeof(rec_t)) {
            perror("child write");
            exit(1);
        }
        printf("pch end\n");
	}

    pid = wait(&status);
    close(fd);
    free(fp1);
    free(fp2);

    return 0;
}

