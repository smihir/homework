#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define SENTINEL INT_MAX

typedef struct {
    int * array;
    int inversions;
}count_inv;

/*
 * function merge-sort(array, l-index, r-index)
 *     if (l-index > r-index)
 *         return;
 *
 *      merge-sort(array, l-index, r-index/2);
 *      merge-sort(array, r-index/2 + 1, r-index);
 *      merge(array, l-index, r-index, r-index/2);
 *      return;
 */

void merge(int * array, int l_index, int r_index, int l_end_index);

void print_array(char *s, int * array, int size)
{
    int i;
    printf("%s\n", s);
    printf("==============\n");
    for (i = 0; i < size; i++) {
        printf("%d ", array[i]);
            if ((i + 1) % 16 == 0)
                printf("\n");
    }
    printf("\n");
    printf("==============\n");

}

void merge_sort(int *array, int l_index, int r_index)
{
    int mid;

    if (l_index >= r_index)
        return;

    mid = l_index + ((r_index - l_index)/2);
    
    merge_sort(array, l_index, mid);
    merge_sort(array, mid + 1, r_index);
    merge(array, l_index, r_index, mid);
}

void merge(int * array, int l_index, int r_index, int l_end_index)
{
    int *l_copy, *r_copy;
    int l_size = l_end_index - l_index + 1;
    int r_size = r_index - l_end_index;
    int i, j = 0, k = 0;
   
    // Last +1 is for SENTINEL 
    l_copy = malloc(l_size + 1);
    r_copy = malloc(r_size + 1);

    memcpy(l_copy, array + l_index, l_size*sizeof(int));
    memcpy(r_copy, array + l_end_index + 1, r_size*sizeof(int));

    l_copy[l_size] = SENTINEL;
    r_copy[r_size] = SENTINEL;

    for (i = 0; i < (l_size + r_size); i++) {
        if (l_copy[j] <= r_copy[k]) {
            array[l_index + i] = l_copy[j];
            j++;
        } else {
            array[l_index + i] = r_copy[k];
            k++;
        }
    }
    free(l_copy);
    free(r_copy);
}

int main(int argc, char **argv)
{
    int nruns, size, i;
    int inarray[100001];
    
    scanf("%d", &nruns);

    while (nruns >= 1) {
        scanf("%d", &size);
        for (i = 0; i < size; i++)
            scanf("%d", &inarray[i]);
        print_array("In Array:", inarray, size);

        merge_sort(inarray, 0, size - 1);
        print_array("Out Array:", inarray, size);
        
        nruns--;
    }
}
