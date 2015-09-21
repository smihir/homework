#include<stdio.h>

int main(int argc, char **argv)
{
    int in;

    while(1){
        scanf("%d", &in);
        if (in == 42) {
            return 0;
        }
        else {
            printf("%d\n",in);
        }
    }
}
