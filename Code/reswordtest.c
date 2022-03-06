/********************* reswordtest.c ********************/
#include <stdio.h>
#include "resword.h"
int main()
{
  printf("isResword(\"DO\")=%d\n", isResword("DO"));
  printf("isResword(\"PROG\")=%d\n", isResword("PROG"));
  printf("isResword(\"BEGIN\")=%d\n", isResword("BEGIN"));
  printf("isResword(\"WRITE\")=%d\n", isResword("WRITE"));
  return 0;
}
