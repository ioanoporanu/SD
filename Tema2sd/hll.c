// Copyright 2020 <Oporanu Ioan Nicolae>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MBUCKETS 2048

unsigned int hash_function_int(void *a) {
  unsigned int uint_a = *((unsigned int *)a);
  uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
  uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
  uint_a = (uint_a >> 16u) ^ uint_a;
  return uint_a;
}

int nr_bytes(unsigned int a) {
  int j = 0;
  while (a != 0) {
    a = a / 2;
    j++;
  }
  return j;
}

int main(int argc, char *argv[]) {
  int *M;
  int *N;
  double S = 0;
  M = calloc(MBUCKETS, sizeof(int));
  N = malloc(MBUCKETS * sizeof(int));
  unsigned int a;
  FILE *f = fopen(argv[argc-1], "r");

if(!N) {
		free(M);
		fclose(f);
		return 0;
	}
	if(!M) {
		free(N);
		fclose(f);
		return 0;
	}
	if(f == NULL) {
		free(N);
		free(M);
		return 0;
	}

  while (!feof(f)) {
    fscanf(f, "%u", &a);
    fscanf(f, "\n");
    unsigned int hash;
    hash = hash_function_int(&a);
    unsigned int j = hash;
    int dim = nr_bytes(hash);
      j = j >> (dim - 11);
      hash = hash << (33 - dim + 11);
      int contor = 0;
      while (hash != 0) {
        hash = hash >> 1;
        contor++;
      }
      int nr0 = 32- contor;
      if (M[j] < nr0) {
        M[j] = nr0;
      }
    }

  for (int j = 0; j < MBUCKETS; j++) {
  	N[j] = 1;
  	if (M[j] != 0){
    for (int i = 0; i < M[j]-1; i++) {
        N[j] = N[j] * 2;
    }
  }
}

  for (int j = 0; j < MBUCKETS; j++) {
    if (M[j] != 0)
      S = S + 1/(double)N[j];
  }

  double alfa = (double)(0.7213 / (1 + 1.079 / MBUCKETS));
  double Z = 1 / S;
  double E = Z * MBUCKETS * MBUCKETS * alfa;
  printf("%d", (int)E);

  free(M);
  free(N);
  fclose(f);

  return 0;
}
