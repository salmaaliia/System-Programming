#include <stdio.h>
#include "b.h"

#define RET 0
int s = 4;
int k;

int main()
{
   static int l = 15;
   s = s + l;
   func(s);

   return 0;
}