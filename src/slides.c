#include <stdio.h>
#include <stdlib.h>
#include "slides.h"

struct slide* createSlideArray(int s) {
    struct slide* slides = (struct slide*)malloc(s * sizeof(struct slide));
    return slides;
}

struct line *nextLine(struct line *prev) {
     struct line *n = malloc(sizeof(struct line));
    n->prev = prev;
    prev->next = n;
    return n;
}

void freeSlides(struct slide *slide[], int s) {
    int i;
    for (i=0;i<s;i++) {
        free(slide[i]);
    }
}

void freeLines(struct line *l) {
    struct line *current;
    while(l) {
        current = l;
        l = l->next;
        free(current);
    }
}
