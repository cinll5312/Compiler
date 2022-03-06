#include<stdio.h>
int main(){
  int i = 3;
  do{
    printf("%d\n",&i);
    i = i - 1;
  }while(i > 0);
}