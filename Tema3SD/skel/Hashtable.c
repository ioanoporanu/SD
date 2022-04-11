 /*
 * Hashtable.c
 * Alexandru-Cosmin Mihai
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashtable.h"

/*
 * Functii de comparare a cheilor:
 */
int compare_function_ints(void *a, void *b) {
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b) {
        return 0;
    } else if (int_a < int_b) {
        return -1;
    } else {
        return 1;
    }
}

int compare_function_strings(void *a, void *b) {
    char *str_a = (char *)a;
    char *str_b = (char *)b;

    return strcmp(str_a, str_b);
}

/*
 * Functii de hashing:
 */
unsigned int hash_function_int(void *a) {
    /*
     * Credits: https://stackoverflow.com/a/12996028/7883884
     */
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

unsigned int hash_function_string(void *a) {
    /*
     * Credits: http://www.cse.yorku.ca/~oz/hash.html
     */
    unsigned char *puchar_a = (unsigned char*) a;
    unsigned long hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

    return hash;
}

/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
void init_ht(struct Hashtable *ht, int hmax, unsigned int (*hash_function)(void*), int (*compare_function)(void*, void*)) {
    /* TODO */
    int i;
  	ht->buckets = (struct LinkedList *) malloc(hmax * sizeof(struct LinkedList));
  	for (i = 0; i < hmax; i++) {
  		ht->buckets[i].head = NULL;
  		ht->buckets[i].size = 0;
   	}
  	ht->size = 0;
  	ht->hmax = hmax;
  	ht->hash_function = hash_function;
  	ht->compare_function = compare_function;
}

/*
 * Atentie! Desi cheia este trimisa ca un void pointer (deoarece nu se impune tipul ei), in momentul in care
 * se creeaza o noua intrare in hashtable (in cazul in care cheia nu se gaseste deja in ht), trebuie creata o copie
 * a valorii la care pointeaza key si adresa acestei copii trebuie salvata in structura info asociata intrarii din ht.
 * Pentru a sti cati octeti trebuie alocati si copiati, folositi parametrul key_size_bytes.
 *
 * Motivatie:
 * Este nevoie sa copiem valoarea la care pointeaza key deoarece dupa un apel put(ht, key_actual, value_actual),
 * valoarea la care pointeaza key_actual poate fi alterata (de ex: *key_actual++). Daca am folosi direct adresa
 * pointerului key_actual, practic s-ar modifica din afara hashtable-ului cheia unei intrari din hashtable.
 * Nu ne dorim acest lucru, fiindca exista riscul sa ajungem in situatia in care nu mai stim la ce cheie este
 * inregistrata o anumita valoare.
 */
void put(struct Hashtable *ht, void *key, int key_size_bytes, void *value) {
    /* TODO */
    struct info *item;
    struct Node *curr;;
    void *new_key;
    int hash_index;

    new_key = malloc(key_size_bytes);
    memcpy(new_key, key, key_size_bytes);
    item = malloc(sizeof(struct info));

    hash_index = ht->hash_function(key) % ht->hmax;

   
    		item->key = new_key;
    		item->value = value;
    		add_nth_node(&ht->buckets[hash_index], 0, item);
    		ht->size++;
    	
}


void* get(struct Hashtable *ht, void *key) {
    /* TODO */
    struct Node *curr;
	int hash_index = ht->hash_function(key) % ht->hmax;

	curr = ht->buckets[hash_index].head;
	if (curr != NULL) {
		while(curr != NULL) {
			if (ht->compare_function(key, ((struct info*)curr->data)->key) == 0) {
				break;
			}
			curr = curr->next;
		}
		if (curr != NULL) {
			return ((struct info*)curr->data)->value;
		}
	}
    return NULL;
}

/*
 * Functie care intoarce:
 * 1, daca pentru cheia key a fost asociata anterior o valoare in hashtable folosind functia put
 * 0, altfel.
 */
int has_key(struct Hashtable *ht, void *key) {
    /* TODO */
	struct Node *curr;

	int hash_index = ht->hash_function(key) % ht->hmax;

	if (ht->buckets[hash_index].size != 0) {
		curr = ht->buckets[hash_index].head;
    	while (curr != NULL) {
    		if (ht->compare_function(key, ((struct info *)(curr->data))->key) == 0) {
    			return 1;
    		}
    		curr = curr->next;
    	}
	}
    return 0;
}

/*
 * Procedura care elimina din hashtable intrarea asociata cheii key.
 * Atentie! Trebuie avuta grija la eliberarea intregii memorii folosite pentru o intrare din hashtable (adica memoria
 * pentru copia lui key --vezi observatia de la procedura put--, pentru structura info si pentru structura Node din
 * lista inlantuita).
 */
void remove_ht_entry(struct Hashtable *ht, void *key) {
    /* TODO */
    struct Node *curr, *rmv_node;
    struct info *rmv_info;
    int hash_index = ht->hash_function(key) % ht->hmax;
    int n=0;

    curr = ht->buckets[hash_index].head;
    if (curr == NULL) {
    	return;
    } else {
    	while (curr != NULL) {
    		if (ht->compare_function(key, ((struct info*)(curr->data))->key) == 0) {
    			break;
    		}
    		curr = curr->next;
    		++n;
    	}
    	if (curr != NULL) {
    		rmv_node = remove_nth_node(&ht->buckets[hash_index], n);
    		rmv_info = (struct info*)(rmv_node->data);
    		free(rmv_info->key);
    		free(rmv_info);
    		free(rmv_node);
    		--ht->size;
    	}
    }
}

/*
 * Procedura care elibereaza memoria folosita de toate intrarile din hashtable, dupa care elibereaza si memoria folosita
 * pentru a stoca structura hashtable.
 */
void free_ht(struct Hashtable *ht) {
    /* TODO */
    int i;
    struct LinkedList list;
    struct Node *curr, *prev;
    struct info *prev_info;
    for(i = 0; i < ht->hmax; i++) {
    	list = ht->buckets[i];
    	curr = list.head;
    	if(curr != NULL) {
    		while (curr != NULL) {
    			prev = curr;
    			curr = curr->next;
    			prev_info = (struct info*)(prev->data);
    			free(prev_info->key);
    			free(prev_info);
    			free(prev);
    		}
    	}
    }
    free(ht->buckets);
    free(ht);
}

int get_ht_size(struct Hashtable *ht) {
    if (ht == NULL) {
        return -1;
    }

    return ht->size;
}

int get_ht_hmax(struct Hashtable *ht) {
    if (ht == NULL) {
        return -1;
    }

    return ht->hmax;
}
