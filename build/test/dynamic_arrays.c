#include <stdio.h>
#include <stdlib.h>

int main() {
  int* arr = (int*)malloc(5 * sizeof(int));

  int i = 0;
  for(i; i < 5; i++) {
    arr[i] = i;
    printf("arr[%d] = %d\n", i, arr[i]);
  }

  arr = (int*)realloc(arr, 10 * sizeof(int));
  for(i; i < 10; i++) {
    arr[i]= i;
    printf("arr[%d] = %d\n", i, arr[i]);
  }

  return 0;
}
