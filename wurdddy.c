#include <regex.h>
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

// char *estrdup(char *s);
char *cleanrrr(char *s);
unsigned int hash(char *wurd);
void build(FILE *fp);
void dump();
WurdCount* lookup(char *wurd, int create);

regex_t regex;
int status;
regmatch_t pmatch[2];

unsigned int hash(char *wurd) {
    unsigned int h;
    unsigned char *p;
    h = 0;
    // printf("HASHY!\n");
    for (p = (unsigned char *) wurd; *p != '\0'; p++)
        h = MULTIPLIER * h + *p;
    return h % NHASH;
}

WurdCount* lookup(char *wurd, int create) {
    unsigned int h;
    WurdCount *w;
    h = hash(wurd);
    // printf("HAHS %d\n", h);
    for (w = wurds[h]; w != NULL; w = w->next) {
        if (strcmp(wurd, w->wurd) != 0) {
            break;
        } else {
            // printf("FOUND IT!\n");
            return w;
        }
    }
    if (create) {
        w = (WurdCount *) malloc(sizeof(WurdCount));
        w->wurd = wurd;
        w->count = 0;
        w->next = wurds[h];
        wurds[h] = w;
    }
    return w;
}

char *cleanrrr(char *s)
{
    char *cleanwurd;

    if ((cleanwurd = malloc(100 * sizeof(char))) == NULL) {
        printf("MEMORYD IED...!!!\n");
        exit(-1);
    }

    // do i really need to do this? it seems to fix a bug with random chars sometimes appearing
    int j;
    for (j = 0; j < 99; j++)
        cleanwurd[j] = 0;

    int cli; // cleanword array Index
    int i;
    int len = strlen(s);
    for (i = 0, cli = 0; i < len; i++) {
        status = regexec(&regex, &s[i], 1, pmatch, 0);
        if (status == 1)
            cleanwurd[cli++] = s[i];
    }
    return cleanwurd;
}

void build(FILE *fp) {
    // build a hash array containing lists of wurd count structs 
    
    char buf[100], fmt[10];
    sprintf(fmt, "%%%lus", sizeof(buf)-1); // now fmt = "%99s"

    if (regcomp(&regex, "[^a-z]", REG_ICASE|REG_EXTENDED) != 0) {
        perror("Oooft, fucked that regex up");
        exit(-1);
    } // for use in cleanrrr 

    while (fscanf(fp, fmt, buf) != EOF) {
        char *cleanwurd;
        cleanwurd = cleanrrr(buf);
        if (strlen(cleanwurd) > 0) {
            WurdCount *w;
            w = lookup(cleanwurd, 1);
            w->count++;
        }
    }
    regfree(&regex);
}

void printCount(WurdCount *w) {
    printf("%3d: %s\n", w->count, w->wurd);
}

void dump()
{
    int i;
    WurdCount *w;
    for (i = 0; i < NHASH; i++) {
        if ((w = wurds[i]) != NULL) {
            printCount(w);
            for ( w = w->next ; w != NULL; w = w->next) {
                printCount(w);
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("JUST WAN ARGUMENT PLEASE\n");
        exit(-1);
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)  {
        perror("FUCK THAT! ");
        exit(-1);
    }

    build(fp); 
    dump();
}
