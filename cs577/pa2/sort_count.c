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

void merge(count_inv * inv, int l_index, int r_index, int l_end_index);

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

void merge_sort(count_inv *inv, int l_index, int r_index)
{
    int mid;

    if (l_index >= r_index)
        return;

    mid = l_index + ((r_index - l_index)/2);
    
    merge_sort(inv, l_index, mid);
    merge_sort(inv, mid + 1, r_index);
    merge(inv, l_index, r_index, mid);
}

void merge(count_inv * inv, int l_index, int r_index, int l_end_index)
{
    int *l_copy, *r_copy;
    int l_size = l_end_index - l_index + 1;
    int r_size = r_index - l_end_index;
    int i, j = 0, k = 0;
    int *array = inv->array;
   
    // Last +1 is for SENTINEL 
    l_copy = malloc((l_size + 1) * sizeof(int));
    r_copy = malloc((r_size + 1) * sizeof(int));

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
            inv->inversions = inv->inversions + (l_size - j);
        }
    }
    free(l_copy);
    free(r_copy);
}

int main(int argc, char **argv)
{
    int nruns, size, i;
    int inarray[100001];
    count_inv inv_counter;
   
    inv_counter.array = inarray;
    scanf("%d", &nruns);

    while (nruns >= 1) {
        scanf("%d", &size);
        for (i = 0; i < size; i++)
            scanf("%d", &inarray[i]);

        inv_counter.inversions = 0;

        merge_sort(&inv_counter, 0, size - 1);
        printf("%d\n", inv_counter.inversions);
        
        nruns--;
    }
}
