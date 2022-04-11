// Copyright 2020 <Oporanu Ioan Nicolae>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 100

struct info {
  char value[NMAX];
  int freq;
};

struct Hashtable {
  struct info *buckets;
  int hmax;
  unsigned int (*hash_function_string)(void *);
};

unsigned int hash_function_string(void *a) {
  unsigned char *puchar_a = (unsigned char *)a;
  __uint32_t hash = 5381;
  __int32_t c;
  while ((c = *puchar_a++))
    hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */
  return hash;
}

void free_ht(struct Hashtable *ht) {
  free(ht->buckets);
  free(ht);
}

void init_ht(struct Hashtable *ht, int hmax,
             unsigned int (*hash_function)(void *)) {
  ht->buckets = calloc(hmax, sizeof(struct info));
  ht->hmax = hmax;
  ht->hash_function_string = hash_function;
}

void insert(struct Hashtable *ht, char *a, int m) {
  int hash = ht->hash_function_string(a) % (ht->hmax);
  if (ht->buckets[hash].freq == 0) {
    snprintf(ht->buckets[hash].value, sizeof(ht->buckets[hash].value) + 1, "%s",
             a);
    ht->buckets[hash].freq++;
  } else {
    if (ht->buckets[hash].freq != 0 &&
        strcmp(ht->buckets[hash].value, a) == 0) {
      ht->buckets[hash].freq++;
    }
  }

  if (ht->buckets[hash].freq != 0 && strcmp(ht->buckets[hash].value, a) != 0) {
    for (int i = hash + 1; i < m; i++) {
      if (ht->buckets[i].freq != 0 && strcmp(ht->buckets[i].value, a) == 0) {
        ht->buckets[i].freq++;
        break;
      }

      if (ht->buckets[i].freq == 0) {
        snprintf(ht->buckets[i].value, sizeof(ht->buckets[i].value) + 1, "%s",
                 a);
        ht->buckets[i].freq++;
        break;
      }

      if (i == m - 1 && ht->buckets[i].freq != 0 &&
          strcmp(ht->buckets[i].value, a) != 0) {
        for (int j = 0; j < hash; j++) {
          if (ht->buckets[j].freq != 0 &&
              strcmp(ht->buckets[j].value, a) == 0) {
            ht->buckets[j].freq++;
            break;
          }

          if (ht->buckets[j].freq == 0) {
            snprintf(ht->buckets[j].value, sizeof(ht->buckets[j].value) + 1,
                     "%s", a);
            ht->buckets[j].freq++;
            break;
          }
        }
      }
    }
  }
}

int main() {
  char a[NMAX];
  int m = 0;

  while (!feof(stdin)) {
    scanf("%s", a);
    scanf("\n");
    m++;
  }

  struct Hashtable *ht = malloc(sizeof(struct Hashtable));
  if (!ht) {
    return 0;
  }
  init_ht(ht, m, hash_function_string);

  fseek(stdin, 0, SEEK_SET);

  while (!feof(stdin)) {
    scanf("%s", a);
    scanf("\n");
    insert(ht, a, m);
  }

  for (int i = 0; i < m; i++) {
    if (strcmp(ht->buckets[i].value, "\0") != 0) {
      printf("%s %d\n", ht->buckets[i].value, ht->buckets[i].freq);
    }
  }

  free_ht(ht);
  return 0;
}
