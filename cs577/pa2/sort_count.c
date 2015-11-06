#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


inline int merge(int *array, int l_index, int r_index, int l_end_index);

#define gc getchar_unlocked
inline int read_int()
{
    int ret = 0;
    char c = gc();
    while(c<'0' || c>'9') c = gc();
    while(c>='0' && c<='9') {
        ret = 10 * ret + c - 48;
        c = gc();
    }
    return ret;
}

int merge_sort(int *array, int l_index, int r_index)
{
    int mid, c1, c2 ,c3;

    if (__builtin_expect(l_index >= r_index, 0))
        return 0;

    mid = l_index + ((r_index - l_index) >> 1);
    
    c1 = merge_sort(array, l_index, mid);
    c2 = merge_sort(array, mid + 1, r_index);
    c3 = merge(array, l_index, r_index, mid);
    return (c1 + c2 + c3);
}

inline int merge(int *array, int l_index, int r_index, int l_end_index)
{
    int *r_copy;
    int l_size = l_end_index - l_index + 1;
    int r_size = r_index - l_end_index;
    int i, j = 0, k = 0;
    int inv = 0;
    int l_copy[l_size];

    r_copy = array + l_index + l_size;

    memcpy(l_copy, array + l_index, (l_size)<<2);

    for (i = 0; i < (l_size + r_size); i++) {
        if (l_copy[j] <= r_copy[k]) {
            array[l_index + i] = l_copy[j++];
            if (j == l_size) {
                return inv;
            }
        } else {
            array[l_index + i] = r_copy[k++];
            inv += (l_size - j);
            if (k == r_size) {
                memcpy(array + l_index + i + 1, l_copy + j,
                       (l_size - j)<<2);
                return inv;
            }
        }
    }
    return inv;
}

int main(int argc, char **argv)
{
    int nruns, size, i;
    int inarray[100001];

    nruns = read_int();

    while (nruns >= 1) {
        size = read_int();
        for (i = 0; i < size; i++)
            inarray[i] = read_int();

        printf("%d\n", merge_sort(inarray, 0, size - 1));

        nruns--;
    }
}
