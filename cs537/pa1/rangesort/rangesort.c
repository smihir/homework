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

typedef struct __srec_t {
    unsigned int key;
    int (*precord)[NUMRECS];
} srec_t;

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
    if (((srec_t *)a)->key < ((srec_t *)b)->key)
        return -1;
    else if (((srec_t *)a)->key == ((srec_t *)b)->key)
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
    int c, fd, pid, status, psize, ofd;
    opterr = 0;
    struct stat fs;
    rec_t *r;
    srec_t *rn1, *rn2;
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
    ofd = open(inFile, O_RDONLY);
    if (ofd < 0) {
	perror("open");
	exit(1);
    }

    if (fstat(ofd, &fs) == -1) {
        exit(errno);
    }

    fp1 = (char *)malloc((fs.st_size / sizeof(rec_t)) * sizeof(srec_t));
    fp2 = (char *)malloc((fs.st_size / sizeof(rec_t)) * sizeof(srec_t));

    f = mmap(NULL, fs.st_size, PROT_READ, MAP_SHARED, ofd, 0);
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
        rn1 = (srec_t *)(fp1 + count1*(sizeof(srec_t)));
        rn2 = (srec_t *)(fp2 + count2*(sizeof(srec_t)));

        if (r->key < lowval || r->key > highval)
            continue;

        if (r->key < median) {
            rn1->key = r->key;
            rn1->precord = r->record;
            count1++;
            //printf("%d %d %llu %u %p\n", __LINE__, getpid(), rn1->precord, rn1->key, rn1);
        } else {
            rn2->key = r->key;
            rn2->precord = r->record;
            count2++;
            //printf("%d %d %llu %u %p\n", __LINE__, getpid(), rn2->precord, rn2->key, rn2);
        }

    }

    printf("count1 %d, count2 %d total %d %lu %lu\n", count1, count2, count1+ count2, sizeof(unsigned int), sizeof(void *));
    lowindex = 0;

    // open and create output file
    fd = open(outFile, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) {
	perror("open");
	exit(1);
    }
    
    //printf("pid parent %d\n",getpid());
    pid = fork();

    if (pid < 0) {
        perror("fork() error");
        exit(1);
    } else if (pid == 0) {
        //printf("parent start\n");
        char * sfp1 = (char *)malloc(count1 * sizeof(rec_t));
        int l;
        rec_t *fr;
        srec_t *sr;
        
        if (sfp1 == NULL) {
            //printf("%d %d\n", __LINE__, getpid());
            exit(1);
        }
        //printf("%d %d\n", __LINE__, getpid());
        qsort(fp1, count1, sizeof(srec_t), compar_f);
        //printf("%d %d\n", __LINE__, getpid());
#if 1
        for (l = 0; l<count1; l++) {
            sr = (srec_t *)(fp1 + l*(sizeof(srec_t)));
            fr = (rec_t *)(sfp1 + l*(sizeof(rec_t)));
            fr->key = sr->key;
            memcpy(fr->record, sr->precord, NUMRECS);
        }
#endif
        //printf("%d %d\n", __LINE__, getpid());

        rc = pwrite(fd, sfp1, (count1) * sizeof(rec_t) , 0);
        if (rc != (count1) * sizeof(rec_t)) {
            perror("main write");
            exit(1);
        }
        //printf("parent end\n");
    } else {
        //printf("pch start\n");
        char * sfp2 = (char *)malloc(count2 * sizeof(rec_t));
        int k;
        rec_t *fr1;
        srec_t *sr1;

        //printf("%d %d\n", __LINE__, getpid());
        qsort(fp2, count2, sizeof(srec_t), compar_f);
        //printf("%d %d\n", __LINE__, getpid());

        for (k = 0; k<count2; k++) {
            sr1 = (srec_t *)(fp2 + k*(sizeof(srec_t)));
            fr1 = (rec_t *)(sfp2 + k*(sizeof(rec_t)));
            //printf("%d %p %u\n", __LINE__, sr1, sr1->key);

            fr1->key = sr1->key;
            memcpy(fr1->record, sr1->precord, NUMRECS);
        }

        //printf("%d %d\n", __LINE__, getpid());
#if 1
        rc = pwrite(fd, fp2, (count2) * sizeof(rec_t), (count1) * sizeof(rec_t));
        if (rc != (count2) * sizeof(rec_t)) {
            perror("child write");
            exit(1);
        }
#endif
        //printf("pch end\n");
	}

    pid = wait(&status);
    printf("%d %d status = %d\n", __LINE__, getpid(), status);
    close(fd);
    free(fp1);
    free(fp2);
    munmap(f, fs.st_size);
    (void) close(ofd);


    return 0;
}
