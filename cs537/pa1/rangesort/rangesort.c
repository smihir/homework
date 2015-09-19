#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>
#include "sort.h"

typedef struct __srec_t {
    unsigned int key;
    void *precord;
} srec_t;

void
usage(char *prog) 
{
    fprintf(stderr, "Usage: rangesort -i inputfile -o outputfile -l lowvalue "
                    "-h highvalue\n");
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
    int c, fd, pid, status, ofd;
    opterr = 0;
    struct stat fs;
    rec_t *r;
    srec_t *rn1, *rn2;
    char *f, *fp1, *fp2, *endptr;
	int rc, set = 0, count1 = 0, count2 = 0;
    unsigned int i;
    long unsigned int highval = ULLONG_MAX, lowval = ULLONG_MAX, median;
    unsigned int lowindex = 0, highindex, max = 0;

    if (argc == 1)
        usage(argv[0]);

    while ((c = getopt(argc, argv, "i:o:l:h:")) != -1) {
	    switch (c) {
        case 'i':
            inFile = strdup(optarg);
            set++;
            break;
        case 'o':
            outFile = strdup(optarg);
            set++;
            break;
        case 'h':
            highval = strtoul(optarg, (char **)&endptr, 10);
            if (endptr == optarg || errno == ERANGE) {
	            fprintf(stderr,"Error: Invalid range value\n");
	            exit(1);
            }
            set++;
            break;
        case 'l':
            lowval = strtoul(optarg, (char **)&endptr, 10);
            if (endptr == optarg || errno == ERANGE) {
	            fprintf(stderr,"Error: Invalid range value\n");
	            exit(1);
            }
            set++;
            break;
        default:
            usage(argv[0]);
	    }
    }


    if (set !=4 ) {
        usage(argv[0]);
	    exit(1);
    }

    if (highval > UINT_MAX || lowval > UINT_MAX) {
	    fprintf(stderr, "Error: Invalid range value\n");
	    exit(1);
    }

    if (highval < lowval) {
	    fprintf(stderr, "Error: Invalid range value\n");
	    exit(1);
    }

    // open and create output file
    ofd = open(inFile, O_RDONLY);
    if (ofd < 0) {
	    fprintf(stderr,"Error: Cannot open file %s\n", inFile);
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

    highindex = ((fs.st_size)/sizeof(rec_t));

    for (i = 0; i<highindex; i++) {
        r = (rec_t *)(f + i*(sizeof(rec_t)));

        if (r->key < lowval || r->key > highval)
            continue;
        if(r->key > max)
            max = r->key;
    }

    median = max/2;
    for (i = 0; i<highindex; i++) {
        r = (rec_t *)(f + i*(sizeof(rec_t)));
        rn1 = (srec_t *)(fp1 + count1*(sizeof(srec_t)));
        rn2 = (srec_t *)(fp2 + count2*(sizeof(srec_t)));

        if (r->key < lowval || r->key > highval)
            continue;

        if (r->key < median) {
            rn1->key = r->key;
            rn1->precord = r->record;
            count1++;
        } else {
            rn2->key = r->key;
            rn2->precord = r->record;
            count2++;
        }

    }

    lowindex = 0;

    // open and create output file
    fd = open(outFile, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) {
	    fprintf(stderr, "Error: Cannot open file %s\n", outFile);
	    exit(1);
    }
    
    pid = fork();

    if (pid < 0) {
        perror("fork() error");
        exit(1);
    } else if (pid == 0) {
        char * sfp1 = (char *)malloc(count1 * sizeof(rec_t));
        int l;
        rec_t *fr;
        srec_t *sr;
        
        if (sfp1 == NULL) {
            exit(1);
        }

        qsort(fp1, count1, sizeof(srec_t), compar_f);

        for (l = 0; l<count1; l++) {
            sr = (srec_t *)(fp1 + l*(sizeof(srec_t)));
            fr = (rec_t *)(sfp1 + l*(sizeof(rec_t)));
            fr->key = sr->key;
            memcpy(fr->record, sr->precord, NUMRECS * sizeof(unsigned int));
        }

        rc = pwrite(fd, sfp1, (count1) * sizeof(rec_t) , 0);
        if (rc != (count1) * sizeof(rec_t)) {
            perror("main write");
            exit(1);
        }
    } else {
        char * sfp2 = (char *)malloc(count2 * sizeof(rec_t));
        int k;
        rec_t *fr1;
        srec_t *sr1;

        qsort(fp2, count2, sizeof(srec_t), compar_f);

        for (k = 0; k<count2; k++) {
            sr1 = (srec_t *)(fp2 + k*(sizeof(srec_t)));
            fr1 = (rec_t *)(sfp2 + k*(sizeof(rec_t)));

            fr1->key = sr1->key;
            memcpy(fr1->record, sr1->precord, NUMRECS*sizeof(unsigned int));
        }

        rc = pwrite(fd, sfp2, (count2) * sizeof(rec_t),
                    (count1) * sizeof(rec_t));
        if (rc != (count2) * sizeof(rec_t)) {
            perror("child write");
            exit(1);
        }
	}

    close(fd);
    free(fp1);
    free(fp2);
    munmap(f, fs.st_size);
    (void) close(ofd);

    pid = wait(&status);

    return 0;
}
