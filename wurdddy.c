#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
  NHASH = 4093,
  MULTIPLIER = 31
};

typedef struct WurdCount WurdCount;
struct WurdCount {
  char *wurd;
  int count;
  WurdCount *next;
};
WurdCount *wurds[NHASH];

char *estrdup(char *s);
void build(FILE *fp);
void inc(char *wurd);
unsigned int hash(char *wurd);
WurdCount* lookup(char *wurd, int create);

void inc(char *wurd) {
  WurdCount *w;
  w = lookup(wurd, 1);
  printf("called!\n");
  w->count++;
}

unsigned int hash(char *wurd) {
  unsigned int h;
  unsigned char *p;
  h = 0;
  printf("HASHY!\n");
  for (p = (unsigned char *) wurd; *p != '\0'; p++)
    h = MULTIPLIER * h + *p;
  return h % NHASH;
}

WurdCount* lookup(char *wurd, int create) {
  unsigned int h;
  WurdCount *w;
  h = hash(wurd);
  printf("HAHS %d", h);
  for (w = wurds[h]; w != NULL; w = w->next) {
    if (strcmp(wurd, w->wurd) != 0) {
      break;
    } else {
      printf("FOUND IT!\n");
      return w;
    }
  }
  if (create) {
    w = (WurdCount *) malloc(sizeof(WurdCount));
    w->next = wurds[h];
    wurds[h] = w;
  }
  return w;
}

char *estrdup(char *s)
{
    char *t;
      t = (char *) malloc(strlen(s)+1);
        if (t == NULL)
              printf("EstrdumpFuckkkaa!!\n");
          strcpy(t, s);
            return t;
}

void build(FILE *fp) {
  char buf[100], fmt[10];
  sprintf(fmt, "%%%lus", sizeof(buf)-1);
  while (fscanf(fp, fmt, buf) != EOF) {
    printf("%s\n", buf);
    //inc(sprintf("%s", buf));
    inc(estrdup(buf));
  }
}


int main(int argc, char **argv)
{
  if (argc != 2) {
    printf("JUST WAN ARGUMENT PLEASE\n");
    exit(-1);
  }

  FILE *fp = fopen(argv[1], "r");
  // TODO: test for error
  build(fp);
}
