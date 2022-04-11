// Copyright 2020 <Oporanu Ioan Nicolae>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Node {
  char titlu[30];
  char artist[30];
  char album[30];
  int year;
  struct Node *next;
  struct Node *prev;
};
struct list {
  struct Node *head;
  struct Node *tail;
  struct Node *cursor;
};
struct buffer {
  char titlu[30];
  char artist[30];
  char album[30];
  char year1;
  int year;
};
void MOVE_NEXT(struct list *song, FILE *m) {
  if (song->cursor == NULL) {
    fprintf(m, "Error: no track playing\n");
    return;
  }
  if (song->cursor == song->tail) {
    return;
  }
  song->cursor = song->cursor->next;
}
void MOVE_PREV(struct list *song, FILE *m) {
  if (song->cursor == NULL) {
    fprintf(m, "Error: no track playing\n");
    return;
  }
  if (song->cursor == song->head) {
    return;
  }
  song->cursor = song->cursor->prev;
}
void DEL_FIRST(struct list *song, FILE *f) {
  if (song->head == NULL) {
    fprintf(f, "Error: delete from empty playlist\n");
    return;
  }
  if (song->head->next == NULL) {
    free(song->head);
    song->head = NULL;
    song->tail = NULL;
    song->cursor = NULL;
    return;
  }
  struct Node *aux = song->head;

  if (song->cursor == song->head) {
    song->cursor = song->head->next;
  }
  song->head = song->head->next;
  song->head->prev = NULL;
  free(aux);
}
void DEL_LAST(struct list *song, FILE *f) {
  if (song->tail == NULL) {
    fprintf(f, "Error: delete from empty playlist\n");
    return;
  }
  if (song->tail->prev == NULL) {
    free(song->tail);
    song->tail = NULL;
    song->head = NULL;
    song->cursor = NULL;
    return;
  }
  struct Node *aux = song->tail;
  if (song->cursor == song->tail) {
    song->cursor = song->tail->prev;
  }
  song->tail = song->tail->prev;
  song->tail->next = NULL;
  free(aux);
}
void DEL_CURR(struct list *song, FILE *f) {
  if (song->cursor == NULL) {
    fprintf(f, "Error: no track playing\n");
    return;
  }
  if (song->cursor == song->tail) {
    DEL_LAST(song, f);
    return;
  }
  if (song->cursor == song->head) {
    DEL_FIRST(song, f);
    return;
  }
  struct Node *aux = song->cursor;
  song->cursor->prev->next = song->cursor->next;
  song->cursor->next->prev = song->cursor->prev;
  song->cursor = aux->next;
  free(aux);
}
void DEL_SONG(struct list *song, char nume[100], FILE *m) {
  char fisier[100];
  struct buffer buff;
  memset(fisier, '\0', 100 * sizeof(char));
  memset(&buff, '\0', 98 * sizeof(char));
  snprintf(fisier, sizeof(fisier), "%s%s", "./songs/", nume);

  FILE *f = fopen(fisier, "rb");
  if (f == NULL) {
    return;
  } else {
    fseek(f, -94, SEEK_END);
    fread(&buff, sizeof(struct buffer), 1, f);
    buff.year = atoi(&buff.year1);
    fclose(f);
    struct Node *curr = song->head;
    while (curr != NULL) {
      if (strcmp(curr->titlu, buff.titlu) == 0) {
        if (curr == song->head) {
          DEL_FIRST(song, m);
          break;
        }
        if (curr == song->tail) {
          DEL_LAST(song, m);
          break;
        }
        struct Node *tmp = curr;
        if (song->cursor == curr) {
          song->cursor = song->cursor->next;
        }
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        free(tmp);
        break;
      }
      curr = curr->next;
    }
    if (curr == NULL) {
      fprintf(m, "Error: no song found to delete\n");
    }
  }
}
void ADD_FIRST(struct list *song, char nume[100], FILE *m) {
  char file[100];
  struct buffer buffer;
  memset(file, '\0', 100 * sizeof(char));
  memset(&buffer, '\0', 98 * sizeof(char));
  snprintf(file, sizeof(file), "%s%s", "./songs/", nume);
  FILE *fp = fopen(file, "rb");
  if (fp == NULL) {
    return;
  } else {
    fseek(fp, -94, SEEK_END);
    fread(&buffer, sizeof(struct buffer), 1, fp);
    buffer.year = atoi(&buffer.year1);
    fclose(fp);
    struct Node *curr = song->head;
    while (curr != NULL) {
      if (strcmp(curr->titlu, buffer.titlu) == 0) {
        break;
      }
      curr = curr->next;
    }
    if (curr != NULL) {
      DEL_SONG(song, nume, m);
    }
  }
  char fisier[100];
  struct buffer buff;
  memset(fisier, '\0', 100 * sizeof(char));
  memset(&buff, '\0', 98 * sizeof(char));
  snprintf(fisier, sizeof(fisier), "%s%s", "./songs/", nume);
  FILE *f = fopen(fisier, "rb");
  if (f == NULL) {
    return;
  } else {
    fseek(f, -94, SEEK_END);
    fread(&buff, sizeof(struct buffer), 1, f);
    buff.year = atoi(&buff.year1);
    // printf("%d",buff.year);
    fclose(f);
    if (song->head == NULL) {
      song->head = malloc(sizeof(struct Node));
      memset(song->head, '\0', 94 * sizeof(char));
      song->head->next = NULL;
      song->head->prev = NULL;
      snprintf(song->head->titlu, sizeof(song->head->titlu) + 1, "%s",
               buff.titlu);
      snprintf(song->head->artist, sizeof(song->head->artist) + 1, "%s",
               buff.artist);
      snprintf(song->head->album, sizeof(song->head->album) + 1, "%s",
               buff.album);
      song->head->year = buff.year;
      song->tail = song->head;
      song->cursor = song->head;
    } else {
      struct Node *new;
      new = malloc(sizeof(struct Node));
      memset(new, '\0', 94 * sizeof(char));
      song->head->prev = new;
      new->next = song->head;
      new->prev = NULL;
      snprintf(song->head->prev->titlu, sizeof(song->head->prev->titlu) + 1,
               "%s", buff.titlu);
      snprintf(song->head->prev->artist, sizeof(song->head->prev->artist) + 1,
               "%s", buff.artist);
      snprintf(song->head->prev->album, sizeof(song->head->prev->album) + 1,
               "%s", buff.album);
      song->head->prev->year = buff.year;
      song->head = new;
    }
  }
}
void ADD_LAST(struct list *song, char nume[100], FILE *m) {
  char file[100];
  struct buffer buffer;
  memset(file, '\0', 100 * sizeof(char));
  memset(&buffer, '\0', 98 * sizeof(char));
  snprintf(file, sizeof(file), "%s%s", "./songs/", nume);
  FILE *fp = fopen(file, "rb");
  if (fp == NULL) {
    return;
  } else {
    fseek(fp, -94, SEEK_END);
    fread(&buffer, sizeof(struct buffer), 1, fp);
    buffer.year = atoi(&buffer.year1);
    fclose(fp);
    struct Node *curr = song->head;
    while (curr != NULL) {
      if (strcmp(curr->titlu, buffer.titlu) == 0) {
        break;
      }
      curr = curr->next;
    }
    if (curr != NULL) {
      DEL_SONG(song, nume, m);
    }
  }
  char fisier[100];
  struct buffer buff;
  memset(fisier, '\0', 100 * sizeof(char));
  memset(&buff, '\0', 98 * sizeof(char));
  snprintf(fisier, sizeof(fisier), "%s%s", "./songs/", nume);
  FILE *f = fopen(fisier, "rb");
  if (f == NULL) {
    return;
  } else {
    fseek(f, -94, SEEK_END);
    fread(&buff, sizeof(struct buffer), 1, f);
    buff.year = atoi(&buff.year1);
    // printf("%d",buff.year);
    fclose(f);
    if (song->tail == NULL) {
      song->tail = malloc(sizeof(struct Node));
      memset(song->tail, '\0', 94 * sizeof(char));
      song->tail->next = NULL;
      song->tail->prev = NULL;
      snprintf(song->tail->titlu, sizeof(song->tail->titlu) + 1, "%s",
               buff.titlu);
      snprintf(song->tail->artist, sizeof(song->tail->artist) + 1, "%s",
               buff.artist);
      snprintf(song->tail->album, sizeof(song->tail->album) + 1, "%s",
               buff.album);
      song->tail->year = buff.year;
      song->head = song->tail;
      song->cursor = song->tail;
    } else {
      struct Node *new;
      new = malloc(sizeof(struct Node));
      memset(new, '\0', 94 * sizeof(char));
      song->tail->next = new;
      new->prev = song->tail;
      new->next = NULL;
      snprintf(new->titlu, sizeof(new->titlu) + 1, "%s", buff.titlu);
      snprintf(new->artist, sizeof(new->artist) + 1, "%s", buff.artist);
      snprintf(new->album, sizeof(new->album) + 1, "%s", buff.album);
      new->year = buff.year;
      song->tail = new;
    }
  }
}
void ADD_AFTER(struct list *song, char nume[100], FILE *m) {
  if (song->cursor == NULL) {
    return;
  }
  char file[100];
  struct buffer buffer;
  memset(file, '\0', 100 * sizeof(char));
  memset(&buffer, '\0', 98 * sizeof(char));
  snprintf(file, sizeof(file), "%s%s", "./songs/", nume);
  FILE *fp = fopen(file, "rb");
  if (fp == NULL) {
    return;
  } else {
    fseek(fp, -94, SEEK_END);
    fread(&buffer, sizeof(struct buffer), 1, fp);
    buffer.year = atoi(&buffer.year1);
    // printf("%d",buff.year);
    fclose(fp);
    struct Node *curr = song->head;
    while (curr != NULL) {
      if (strcmp(curr->titlu, buffer.titlu) == 0) {
        break;
      }
      curr = curr->next;
    }
    if (strcmp(song->cursor->titlu, buffer.titlu) == 0) {
      return;
    }
    int ok = 0;
    if (curr != NULL) {
      DEL_SONG(song, nume, m);
      ok = 1;
    }
    if (song->cursor == song->tail) {
      ADD_LAST(song, nume, m);
      return;
    }
    if (ok == 1) {
      struct Node *new;
      new = malloc(sizeof(struct Node));
      memset(new, '\0', 94 * sizeof(char));
      snprintf(new->titlu, sizeof(new->titlu) + 1, "%s", buffer.titlu);
      snprintf(new->artist, sizeof(new->artist) + 1, "%s", buffer.artist);
      snprintf(new->album, sizeof(new->album) + 1, "%s", buffer.album);
      new->year = buffer.year;
      new->prev = song->cursor;
      new->next = song->cursor->next;
      song->cursor->next->prev = new;
      song->cursor->next = new;
    } else {
      struct Node *new;
      new = malloc(sizeof(struct Node));
      memset(new, '\0', 94 * sizeof(char));
      snprintf(new->titlu, sizeof(new->titlu) + 1, "%s", buffer.titlu);
      snprintf(new->artist, sizeof(new->artist) + 1, "%s", buffer.artist);
      snprintf(new->album, sizeof(new->album) + 1, "%s", buffer.album);
      new->year = buffer.year;
      new->next = song->cursor->next;
      new->prev = song->cursor;
      song->cursor->next->prev = new;
      song->cursor->next = new;
    }
  }
}
void SHOW_FIRST(struct list *song, FILE *m) {
  if (song->head == NULL) {
    fprintf(m, "Error: show empty playlist\n");
    return;
  }
  fprintf(m, "Title: %.30s\nArtist: %.30s\nAlbum: %.30s\nYear: %d\n",
          song->head->titlu, song->head->artist, song->head->album,
          song->head->year);
}
void SHOW_LAST(struct list *song, FILE *m) {
  if (song->tail == NULL) {
    fprintf(m, "Error: show empty playlist\n");
    return;
  }
  fprintf(m, "Title: %.30s\nArtist: %.30s\nAlbum: %.30s\nYear: %d\n",
          song->tail->titlu, song->tail->artist, song->tail->album,
          song->tail->year);
}
void SHOW_CURR(struct list *song, FILE *m) {
  if (song->cursor == NULL) {
    fprintf(m, "Error: show empty playlist\n");
    return;
  }
  fprintf(m, "Title: %.30s\nArtist: %.30s\nAlbum: %.30s\nYear: %d\n",
          song->cursor->titlu, song->cursor->artist, song->cursor->album,
          song->cursor->year);
}
void SHOW_PLAYLIST(struct list *song, FILE *m) {
  if (song == NULL || song->head == NULL || song->tail == NULL) {
    fprintf(m, "[]\n");
    return;
  }
  struct Node *curr = song->head;
  fprintf(m, "[");
  while (curr->next != NULL) {
    fprintf(m, "%.30s; ", curr->titlu);
    curr = curr->next;
  }
  fprintf(m, "%.30s]\n", curr->titlu);
}
int main(int argc, char *argv[]) {
  struct list *playlist;
  playlist = malloc(sizeof(struct list));
  playlist->head = NULL;
  playlist->tail = NULL;
  playlist->cursor = NULL;
  FILE *f = fopen(argv[1], "r");
  FILE *m = fopen(argv[2], "w");
  if (f == NULL || m == NULL) {
    if (f == NULL && m != NULL) {
      fclose(m);
    }
    if (m == NULL && f != NULL) {
      fclose(f);
    }

  } else {
    int nr;
    fscanf(f, "%d", &nr);
    for (int i = 0; i < nr; i++) {
      char titlu[200];
      char comanda[100];
      fscanf(f, "%s", comanda);
      fgets(titlu, 200, f);
      titlu[strlen(titlu) - 1] = '\0';
      snprintf(titlu, sizeof(titlu), "%s", titlu + 1);
      if (strcmp(comanda, "MOVE_NEXT") == 0) {
        MOVE_NEXT(playlist, m);
      }
      if (strcmp(comanda, "MOVE_PREV") == 0) {
        MOVE_PREV(playlist, m);
      }
      if (strcmp(comanda, "DEL_FIRST") == 0) {
        DEL_FIRST(playlist, m);
      }
      if (strcmp(comanda, "DEL_LAST") == 0) {
        DEL_LAST(playlist, m);
      }
      if (strcmp(comanda, "DEL_CURR") == 0) {
        DEL_CURR(playlist, m);
      }
      if (strcmp(comanda, "DEL_SONG") == 0) {
        DEL_SONG(playlist, titlu, m);
      }
      if (strcmp(comanda, "ADD_FIRST") == 0) {
        ADD_FIRST(playlist, titlu, m);
      }
      if (strcmp(comanda, "ADD_LAST") == 0) {
        ADD_LAST(playlist, titlu, m);
      }
      if (strcmp(comanda, "ADD_AFTER") == 0) {
        ADD_AFTER(playlist, titlu, m);
      }
      if (strcmp(comanda, "SHOW_FIRST") == 0) {
        SHOW_FIRST(playlist, m);
      }
      if (strcmp(comanda, "SHOW_LAST") == 0) {
        SHOW_LAST(playlist, m);
      }
      if (strcmp(comanda, "SHOW_CURR") == 0) {
        SHOW_CURR(playlist, m);
      }
      if (strcmp(comanda, "SHOW_PLAYLIST") == 0) {
        SHOW_PLAYLIST(playlist, m);
      }
    }
    fclose(f);
    fclose(m);
  }
  struct Node *start = playlist->head;
  struct Node *temp = start;
  while (start != NULL) {
    temp = start;
    start = start->next;
    free(temp);
  }
  free(playlist);
  return 0;
}
