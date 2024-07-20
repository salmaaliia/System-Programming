#include <stdio.h>
#include "b.h"

#define G 0
#define B 1

int c = 2;
int d;
int f = 5;
int func(int p)
{
    static int temp = 10;
    f++;
    f = f + temp + c + p;
    if(f >= 50){
   	 printf("In file b.c f = %d\n", f);
   	 return G;
    }
    else
    {
	printf("In file b.c f = %d\n", f);
	return B;
    }
}