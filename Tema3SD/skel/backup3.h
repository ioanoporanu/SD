#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "publications.h"
#include "Queue.h"
#include "Hashtable.h"

#define NMAX 5000

typedef struct author {
  char* author_name;
  int64_t author_id;
  char* institution;
} Author;

typedef struct paper {
  int citations;
  int paper_number;

  char *title;
  int64_t id;
  int year;
  char* venue;

  Author** authors;
    int num_authors;

  char** fields;
    int num_fields;
        
  int64_t* references;
    int num_refs;
} Paper;

struct publications_data {
 
  struct Hashtable *ht_influenced;
  struct Hashtable *ht;
  struct Hashtable *ht_aux;
  struct Hashtable *ht_venue;
  //struct Hashtable *ht_influenced;
};


PublData* init_publ_data(void) {

  PublData* data = malloc(sizeof(PublData));

  data->ht = malloc(sizeof(struct Hashtable));
   int i;
    data->ht->buckets = (struct LinkedList *) malloc(NMAX * sizeof(struct LinkedList));
   
    data->ht->size = 0;
    data->ht->hmax = NMAX;
    data->ht->hash_function = hash_function_int;
    data->ht->compare_function = compare_function_ints;
  data->ht_aux=malloc(sizeof(struct Hashtable));
  
    data->ht_aux->buckets = (struct LinkedList *) malloc(NMAX * sizeof(struct LinkedList));
   
    data->ht_aux->size = 0;
    data->ht_aux->hmax = NMAX;
    data->ht_aux->hash_function = hash_function_int;
   data->ht_aux->compare_function = compare_function_ints;
     
   data->ht_influenced=malloc(sizeof(struct Hashtable));

    data->ht_influenced->buckets = (struct LinkedList *) malloc(NMAX * sizeof(struct LinkedList));
   
    data->ht_influenced->size = 0;
    data->ht_influenced->hmax = NMAX;
    data->ht_influenced->hash_function = hash_function_int;


    data->ht_venue=malloc(sizeof(struct Hashtable));

    data->ht_venue->buckets = (struct LinkedList *) malloc(NMAX * sizeof(struct LinkedList));
   
    data->ht_venue->size = 0;
    data->ht_venue->hmax = NMAX;
    data->ht_venue->hash_function = hash_function_int;
   data->ht_venue->compare_function = compare_function_ints;
  for (i = 0; i < NMAX; i++) {
      data->ht->buckets[i].head = NULL;
      data->ht->buckets[i].size = 0;
            data->ht_aux->buckets[i].head = NULL;
      data->ht_aux->buckets[i].size = 0;
       data->ht_influenced->buckets[i].head = NULL;
      data->ht_influenced->buckets[i].size = 0;
      data->ht_venue->buckets[i].head = NULL;
      data->ht_venue->buckets[i].size = 0;
    }
  
  return data;
}
  struct hh{
  int64_t id;
  int freq;
 } ;

 struct influenced{
  int64_t id;
  struct LinkedList* list;
  struct Node * tail;
  int nr_ord;
 };
 struct venue{
  char*  venue;
  struct LinkedList* list;
  struct Node * tail;
 };

void put_venue(struct Hashtable *ht, char* venue,const int64_t id) {
    /* TODO */
    struct Node *curr;
   struct Node *curr1;
   unsigned long id_aux =id;
   int ind=hash_function_string(venue) % ht->hmax;
    curr=ht->buckets[ind].head;
     curr1=curr;

  curr=ht->buckets[ind].head;
  while(curr!=NULL){
    curr1=curr;
    if(strcmp(*(&((struct venue *)curr->data)->venue),*(&venue))==0){
      printf("sex");
      
      
      ((struct venue *)curr->data)->tail->next=malloc(sizeof(struct Node));
      ((struct venue *)curr->data)->tail->next->data=id;
      ((struct venue *)curr->data)->tail->next->next=NULL;
      ((struct venue *)curr->data)->tail=((struct venue *)curr->data)->tail->next;
      
      

      
      return;
    }
    curr=curr->next;
  }


  struct  venue *new=malloc(sizeof(struct venue));

 //struct  influenced *new=malloc(sizeof(struct influenced));
  new->venue=venue;
  new->list=malloc(sizeof(struct LinkedList));
  new->list->head=malloc(sizeof( struct Node));
  new->list->head->data=id;
  new->list->head->next=NULL;
  new->tail=new->list->head;
  ht->size++;


struct Node* new_node=malloc(sizeof(struct Node));
new_node->data=new;
new_node->next=NULL;
if(curr1==NULL){
  ht->buckets[ind].head=new_node;
  ht->buckets[ind].size++;
}else{
curr1->next=new_node;
  ht->buckets[ind].size++;

}

        ht->buckets[ind].tail = new_node;
  //add_nth_node(&ht->buckets[ind],ht->buckets[ind].size,new);
    

  


}

void put_aux(struct Hashtable *ht, const int64_t id) {
    /* TODO */
    struct Node *curr;
    struct Node*curr1;
   
   unsigned long id_aux =id;
   int ind=ht->hash_function(&id_aux) % ht->hmax;

  curr=ht->buckets[ind].head;
  while(curr!=NULL){
    curr1=curr;
    if(((struct hh *)curr->data)->id==id){
      ((struct hh *)curr->data)->freq++;
      return;
    }
    curr=curr->next;
  }

 struct  hh *new=malloc(sizeof(struct hh));
  new->id=id;
  new->freq=1;
  add_nth_node(&ht->buckets[ind],0,new);


}
void put_influenced(struct Hashtable *ht, const int64_t id,const int64_t id_inf) {
    /* TODO */
    struct Node *curr;
   struct Node *curr1;
   unsigned long id_aux =id;
   int ind=ht->hash_function(&id_aux) % ht->hmax;
    curr=ht->buckets[ind].head;
     curr1=curr;

  curr=ht->buckets[ind].head;
  while(curr!=NULL){
    curr1=curr;
    if(((struct influenced *)curr->data)->id==id){
      if(((struct influenced *)curr->data)->list->head!=NULL){
      
      ((struct influenced *)curr->data)->tail->next=malloc(sizeof(struct Node));
      ((struct influenced *)curr->data)->tail->next->data=id_inf;
      ((struct influenced *)curr->data)->tail->next->next=NULL;
      ((struct influenced *)curr->data)->tail=((struct influenced *)curr->data)->tail->next;
      }else{
        if(id_inf!=0){
        ((struct influenced *)curr->data)->list->head=malloc(sizeof( struct Node));
  ((struct influenced *)curr->data)->list->head->data=id_inf;
  ((struct influenced *)curr->data)->list->head->next=NULL;
  ((struct influenced *)curr->data)->tail=((struct influenced *)curr->data)->list->head;
   }
      }
      

      
      return;
    }
    curr=curr->next;
  }


  struct  influenced *new=malloc(sizeof(struct influenced));
if(id_inf!=0){
 //struct  influenced *new=malloc(sizeof(struct influenced));
  new->id=id;
  new->list=malloc(sizeof(struct LinkedList));
  new->list->head=malloc(sizeof( struct Node));
  new->list->head->data=id_inf;
  new->list->head->next=NULL;
  new->tail=new->list->head;
  new->nr_ord=ht->size;
  ht->size++;

}else{
   //struct  influenced *new=malloc(sizeof(struct influenced));
  new->id=id;
  new->list=malloc(sizeof(struct LinkedList));
  new->list->head=NULL;
  new->tail=NULL;
  new->nr_ord=ht->size;
  ht->size++;


}
struct Node* new_node=malloc(sizeof(struct Node));
new_node->data=new;
new_node->next=NULL;
if(curr1==NULL){
  ht->buckets[ind].head=new_node;
  ht->buckets[ind].size++;
}else{
curr1->next=new_node;
  ht->buckets[ind].size++;

}

        ht->buckets[ind].tail = new_node;
    

  


}
struct venue* get_venue(struct Hashtable *ht, char* venue) {
    /* TODO */
 // printf("%d\n",key);
    struct Node *curr;
    // unsigned long int h=key;
     
  int hash_index = hash_function_string(venue) % ht->hmax;

  curr = ht->buckets[hash_index].head;
  if (curr != NULL) {
    while(curr != NULL) {
      
       //printf("%lld ",((struct influenced*)curr->data)->id);
       //printf("%lld\n",key);
      if (strcmp(((struct venue*)curr->data)->venue,venue)==0) {
        //printf("da");
        break;
      }
      curr = curr->next;
    }
    if (curr != NULL) {
      //printf("%d ",((struct hh*)curr->data)->freq);
      return (struct influenced*)curr->data;
    }
  }
   
    return NULL;
}
void* get_aux(struct Hashtable *ht, int key) {
    /* TODO */
 // printf("%d\n",key);
    struct Node *curr;
     unsigned long int h=key;
     
  int hash_index = ht->hash_function(&h) % ht->hmax;

  curr = ht->buckets[hash_index].head;
  if(curr==NULL){
    return NULL;
  }
  
    while(curr != NULL) {
      
      // printf("%d ",((struct hh*)curr->data)->id);
      if (((struct hh*)curr->data)->id==h) {
        //printf("da");
        return ((struct hh*)curr->data)->freq;
      }
      curr = curr->next;
    }
    return NULL;
  
   
   
}
 struct influenced* get_influenced(struct Hashtable *ht, int key) {
    /* TODO */
 // printf("%d\n",key);
    struct Node *curr;
     unsigned long int h=key;
     
  int hash_index = ht->hash_function(&h) % ht->hmax;

  curr = ht->buckets[hash_index].head;
  if (curr != NULL) {
    while(curr != NULL) {
      
       //printf("%lld ",((struct influenced*)curr->data)->id);
       //printf("%lld\n",key);
      if (((struct influenced*)curr->data)->id==key) {
        //printf("da");
        break;
      }
      curr = curr->next;
    }
    if (curr != NULL) {
      //printf("%d ",((struct hh*)curr->data)->freq);
      return (struct influenced*)curr->data;
    }
  }
   
    return NULL;
}
void free_paper(Paper* paper) {
  int i;

  free(paper->title);
  free(paper->venue);

  for (i = 0; i < paper->num_authors; i++) {
    free(paper->authors[i]->author_name);
    free(paper->authors[i]->institution);
    free(paper->authors[i]);
  }
  free(paper->authors);

  for (i = 0; i < paper->num_fields; i++) {
    free(paper->fields[i]);
  }
  free(paper->fields);
  free(paper->references);

  free(paper);
  paper = NULL;
}

void new_free_ht(struct Hashtable *ht) {
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
        free_paper(prev_info->value);
        free(prev_info->key);
        free(prev_info);
        free(prev);
      }
    }
  }
  free(ht->buckets);
  free(ht);
}
void new_free_ht_aux(struct Hashtable *ht) {
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
        //free_paper(prev_info->value);
        //free(prev_info->key);
        free(prev_info);
        free(prev);
      }
    }
  }
  free(ht->buckets);
  free(ht);
}
void new_free_ht_influenced(struct Hashtable *ht) {
  int i;
  struct LinkedList list;
  struct LinkedList* heh;
  struct Node *curr1,*curr, *prev,*prev1;
 
  struct influenced *prev_info;
  for(i = 0; i < ht->hmax; i++) {
    list = ht->buckets[i];
    curr = list.head;
    if(curr != NULL) {

      while (curr != NULL) {
        heh= ((struct influenced *)curr->data)->list;
        curr1=heh->head;
        while(curr1!=NULL){
          prev1=curr1;
          curr1=curr1->next;
          free(prev1);
        }
        free(heh);
        prev = curr;
        curr = curr->next;
        prev_info = (struct influenced*)(prev->data);
        //free_paper(prev_info->value);
        //free(prev_info->key);
        free(prev_info);
        free(prev);
      }
    }
  }
  free(ht->buckets);
  free(ht);
}
void destroy_publ_data(PublData* data) {
  new_free_ht(data->ht);
  new_free_ht_aux(data->ht_aux);
  new_free_ht_influenced(data->ht_influenced);
  free(data);
}

void add_paper(PublData* data, const char* title, const char* venue,
    const int year, const char** author_names, const int64_t* author_ids,
    const char** institutions, const int num_authors, const char** fields,
    const int num_fields, const int64_t id, const int64_t* references,
    const int num_refs) {
  /* TODO: implement add_paper */
 
  int i, my_id = id;
  Paper* my_paper, *cit_paper;;
  Author* aut;

  // alloc and initialize new paper
  my_paper = malloc(sizeof(Paper));

  //initialize id, year and num_sth + citations and paper number
  my_paper->citations = 0;
  my_paper->paper_number = data->ht->size+1;
  my_paper->id = id;
  my_paper->year = year;
  my_paper->num_authors = num_authors;
  my_paper->num_fields = num_fields;
  my_paper->num_refs = num_refs;
  my_paper->title = (char *) malloc((strlen(title) + 1) * sizeof(char));
  snprintf(my_paper->title, strlen(title) + 1, "%s", title);
 //printf("%s\n",my_paper->title);
  // initialize venue
  my_paper->venue = (char *) malloc((strlen(venue)+1) * sizeof(char));
  snprintf(my_paper->venue, strlen(venue)+1, "%s", venue);

  //initialize list of authors
  my_paper->authors = malloc(num_authors * sizeof(Author *));
  for (i = 0; i < num_authors; i++) {
    my_paper->authors[i] = malloc(sizeof(Author));
    aut = my_paper->authors[i];

    aut->author_id = author_ids[i];

    aut->author_name = (char *) malloc((strlen(author_names[i])+1) * sizeof(char));
    snprintf(aut->author_name, strlen(author_names[i])+1, "%s", author_names[i]);
    
    aut->institution = (char *) malloc((strlen(institutions[i])+1) * sizeof(char));
    snprintf(aut->institution, strlen(institutions[i])+1, "%s", institutions[i]);
  }

  // initialize fields
  my_paper->fields = (char **) malloc(num_fields * sizeof(char*));
  for (i = 0; i < num_fields; i++) {
    my_paper->fields[i] = (char *) malloc((strlen(fields[i])+1) * sizeof(char));
    snprintf(my_paper->fields[i], strlen(fields[i])+1, "%s", fields[i]);
  }
  char* s=venue;
  int64_t freq=id;
  put_venue(data->ht_venue,s,freq);
   put_influenced(data->ht_influenced,freq,0);
  my_paper->references = malloc(num_refs * sizeof(int64_t));
  for (i = 0; i < num_refs; i++) {
    my_paper->references[i] = references[i];
     freq=id;
    //printf("%d\n",my_paper->references[i]);
  put_influenced(data->ht_influenced,my_paper->references[i],freq);
   put_aux(data->ht_aux,my_paper->references[i]);
   //if(get_influenced(data->ht_influenced,references[i])->head->next!=NULL){
   //printf("%d\n",(int*)get_influenced(data->ht_influenced,references[i])->head->next->data);
//}
   }
  // put_aux(data->ht_aux,my_paper->id);
  

 
  // add paper
  put(data->ht, &my_id, sizeof(const int64_t), my_paper);
  
  //printf("%s\n",my_paper->title);
}

char* get_oldest_influence(PublData* data, const int64_t id_paper) {
  /* TODO: implement get_oldest_influence */
  int i, poped_id, num, size = 1, ID;
  int MIN_YEAR = 10000;
  int *visited = calloc(data->ht->size + 1, sizeof(int));
  char *oldest = NULL;
  Paper *paper, *cit_paper, *old_paper;
  Paper* poped_paper;
  
  struct Queue *q = (struct Queue *) malloc(sizeof(struct Queue));
  init_q(q);
  int64_t id_aux=id_paper;
  Paper *paper_aux=(Paper *)get(data->ht,&id_aux);
  enqueue(q, paper_aux);
  num = paper_aux->paper_number;
  visited[num]++;
  while (!is_empty_q(q)) {
    poped_paper = (Paper*)front(q);
    num=poped_paper->paper_number;
    visited[num]++;
    dequeue(q);
    

    if(poped_paper != NULL && poped_paper->references != NULL) {

      
      for (i = 0; i < poped_paper->num_refs; i++) {
        //printf("sex\n");
        cit_paper = (Paper *) get(data->ht, &poped_paper->references[i]);
        if (cit_paper != NULL) {
         //printf("sex\n");
          num = cit_paper->paper_number;
        /*  if (cit_paper->paper_number >= size) {
            visited = realloc(visited, (num + 1) * sizeof(int));
            memset(visited + size, 0, (num + 1 - size) * sizeof(int));
            size = num + 1;
          }    
          */       if (visited[num] == 0 && cit_paper->year > MIN_YEAR) {
          //printf("sex1\n");
          visited[num]++;
            enqueue(q, cit_paper);
          }
          
          if (visited[num] == 0 && cit_paper->year < MIN_YEAR) {
            MIN_YEAR = cit_paper->year;
            oldest = cit_paper->title;
            paper_aux = cit_paper;
            visited[num]++;
           //printf("sex2\n");
            enqueue(q, paper_aux);
          } 
          if( visited[num]==0 &&cit_paper->year == MIN_YEAR) {
             
                
                visited[num]++;
                 enqueue(q, cit_paper);
             
              if (get_aux(data->ht_aux,paper_aux->id) < get_aux(data->ht_aux,cit_paper->id)) {
                
                oldest = cit_paper->title;
                paper_aux = cit_paper;
                MIN_YEAR=cit_paper->year;

              
              } 
             
               if (get_aux(data->ht_aux,paper_aux->id) == get_aux(data->ht_aux,cit_paper->id)) {
                
                if (paper_aux->id > cit_paper->id ){
                oldest = cit_paper->title;
                paper_aux = cit_paper;
                MIN_YEAR=cit_paper->year;
                  
                   
                }
              }
              
        
            }
        }
      }
      
      }
      
    }
  

  purge_q(q);
  free(q);
  free(visited);
  if (oldest == NULL) {
    oldest = "None";
  }
  
  return oldest;
  return NULL;
}

float get_venue_impact_factor(PublData* data, const char* venue) {
  /* TODO: implement get_venue_impact_factor */
  int i, sum_cit = 0, nr_papers = 0;
  float impact_factor;
  struct LinkedList list;
  struct Node *curr;
  struct hh *inf;
  Paper *paper;
  char*s=venue;
  struct venue* venue_aux=get_venue(data->ht_venue,s);
if(venue_aux==NULL){
  return 0.f;
}
 curr=venue_aux->list->head;
 while(curr!=NULL){
  sum_cit=sum_cit+get_aux(data->ht_aux,curr->data);
  nr_papers++;
  curr=curr->next;
 }

  if(nr_papers>0){
     impact_factor = (float) sum_cit / nr_papers;
    return impact_factor;
  }
  return 0.f;
}

int get_number_of_influenced_papers(PublData* data, const int64_t id_paper,
    const int distance) {
  int i;
  int *visited=calloc(data->ht_influenced->size+1,sizeof(int));
  int *dist=calloc(data->ht_influenced->size+1,sizeof(int));
    struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
   struct Node *curr;
    int nr=0;
    init_q(q);
    int s=id_paper;
    struct influenced* p ;
    //printf("%lld\n",id_paper);

    struct influenced *first=get_influenced(data->ht_influenced,s);
    enqueue(q, first);
    if(first!=NULL){
  // printf("%d hahaha\n",first->nr_ord);

   
   visited[first->nr_ord] = 4;
   dist[first->nr_ord]=0;
   
    while(!is_empty_q(q)) {
       p = (struct influenced *)front(q);
        dequeue(q);
        //printf("%d hahaha\n",dist[p->nr_ord]);
        if(dist[p->nr_ord]>=distance){
          break;
        }
       
        if(p->list != NULL && p->list->head != NULL) {
        // printf("sex1\n");
            curr = p->list->head;
            while(curr!=NULL) {
             // printf("%d\n",curr->data);
                int64_t r = curr->data;
                
                struct influenced* aux=get_influenced(data->ht_influenced,r);

                
                //if(visited==NULL){
                  
                //}
                
               
                   if(aux!=NULL){
                  //printf("sex2\n");
                if(visited[aux->nr_ord] == 0) {
                  
                // printf("sex3\n");
                    visited[aux->nr_ord] = 4;
                    dist[aux->nr_ord] = dist[p->nr_ord]+1;
                  nr++;    
                    enqueue(q, aux);
                }
              }
                curr = curr->next;
            }
        }
        visited[p->nr_ord] = 1;      
    }
  }
  //for (i=0;i<data->ht_influenced->size+1;i++){
    //if (dist[i]!=0)
      //nr++;
  //}
    purge_q(q);
    free(q);
    free(visited);
    free(dist);
    //printf("%d\n",nr);
  return nr;
}
int get_erdos_distance(PublData* data, const int64_t id1, const int64_t id2) {
  /* TODO: implement get_erdos_distance */

  return -1;
}

char** get_most_cited_papers_by_field(PublData* data, const char* field,
    int* num_papers) {
  /* TODO: implement get_most_cited_papers_by_field */

  return NULL;
}

int get_number_of_papers_between_dates(PublData* data, const int early_date,
    const int late_date) {
   int i, sum_cit = 0, nr_papers = 0;
  float impact_factor;
  struct LinkedList list;
  struct Node *curr;
  struct hh *inf;
  Paper *paper;

  

  for (i = 0; i < data->ht->hmax; i++) {
    list = data->ht->buckets[i];
    if (list.size != 0) {
            curr = list.head;
      while (curr != NULL) {
        
       if(((Paper *)(( struct info *)curr->data)->value)->year >= early_date && ((Paper *)(( struct info *)curr->data)->value)->year<=late_date){
        nr_papers++;
      }
      curr=curr->next;
    }

  }
}
  return nr_papers;
}

int get_number_of_authors_with_field(PublData* data, const char* institution,
    const char* field) {
  /* TODO: implement get_number_of_authors_with_field */

  return 0;
}

int* get_histogram_of_citations(PublData* data, const int64_t id_author,
    int* num_years) {
  /* TODO: implement get_histogram_of_citations */

  *num_years = 0;

  return NULL;
}

char** get_reading_order(PublData* data, const int64_t id_paper,
    const int distance, int* num_papers) {
  /* TODO: implement get_reading_order */

  *num_papers = 0;

  return NULL;
}

char* find_best_coordinator(PublData* data, const int64_t id_author) {
  /* TODO: implement find_best_coordinator */

  return NULL;
}