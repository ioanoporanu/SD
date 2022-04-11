// Copyright 2020 <Oporanu Ioan Nicolae>
#include<stdio.h>
#include<stdlib.h>

#define MAX 2000000

int main() {
  unsigned char *v;
  int a;
  v = calloc(MAX, sizeof(int));
  if (!v){
    return 0;
  }

  while (!feof(stdin)) {
    scanf("%d", &a);
    scanf("\n");
    v[a]++;
  }

  for (int i = 0; i <= MAX; i++) {
    if (v[i] != 0) {
      printf("%d %d\n", i, v[i]);
    }
  }

  free(v);
  return 0;
}
