#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "parser.h"

// defaults
int x = 100;
int y = 30;
int s = 15;
char *globalTitle;

/* Removes sub from str */
char *delsub(char *str, const char *sub)
{
    size_t n = strlen(str);

    char *token, *newp;
    token = newp = NULL;

    if ((token = strtok(str, sub)) != NULL) {
        newp = malloc(n * sizeof(*newp));
        strcat(newp, token);

        while ((token = strtok(NULL, sub)) != NULL) {
            strcat(newp, token);
        }
        memmove(str, newp, n);
        free(newp);
    }
    return str;
}

/* int scanr(FILE *ptx, struct Ptx *data, struct Slide **slds) */
/* { */
/*     char *line = NULL; */
/*     size_t len = 0; */
/*     size_t read = 0; */
/*  */
/* // XXX we're not actually using a regular buffer, since */
/* // we're reading the file by line; not block by block */
/*  */
/* #<{(| #if _DEFAULT_SOURCE || ! (_POSIX_C_SOURCE >= 200112L) |)}># */
/* #<{(| # if DSS_DEBUG |)}># */
/* #<{(|     fprintf(stderr, "%s\n", "we're using getpagesize\n"); |)}># */
/* #<{(| # endif |)}># */
/* #<{(|     #<{(| gives us a good starting point for buffer size. |)}># |)}># */
/* #<{(|     size_t pgsz = getpagesize(); |)}># */
/* #<{(| #else |)}># */
/* #<{(|     size_t pgsz = 4096; |)}># */
/* #<{(| #endif |)}># */
/* #<{(|  |)}># */
/* #<{(|     char *buf = malloc(pgsz * sizeof(*buf)); |)}># */
/*  */
/*     #<{(| if we have getline, let's use that |)}># */
/* #if _POSIX_C_SOURCE >= 200809L */
/* # if DSS_DEBUG */
/*         fprintf(stderr, "%s\n", "we're using getline\n"); */
/* # endif */
/*         while ((read = getline(&line, &len, ptx)) != EOF) { */
/*             #<{(| process the file |)}># */
/*  */
/*             #<{(| get title from file */
/*              * FIXME: we should do something else other than title= */
/*              * like @title or something */
/*              |)}># */
/*             if (strstr(line, "title=") != NULL) { */
/*  */
/*  */
/* #endif */
/*  */
/*     while (fgets()); */
/* } */

struct Slide* parseTXT(FILE *inFile, int* slideCounter, char *presTitle)
{
    char buf[1000];
    while(fgets(buf, 1000, inFile)!=NULL) {
        if (strstr(buf, "title=")!=NULL) { // finds title line
            char quoted[128];
            if (sscanf(buf, "%*[^\"]\"%127[^\"]\"", quoted) == 1) {
                globalTitle = quoted;
            } else {
                fprintf(stderr, "improper title\n");
            }
        } else if (strstr(buf, "areaX=")!=NULL) {
            if (sscanf(buf, "%*[^\"]\"%d[^\"]\"", &x) == 1) {
                continue;
            } else {
                fprintf(stderr, "improper area\n");
            }
        } else if (strstr(buf, "areaY=")!=NULL) {
            if (sscanf(buf, "%*[^\"]\"%d[^\"]\"", &y) == 1) {
                continue;
            } else {
                fprintf(stderr, "improper area\n");
            }
        } else if (strstr(buf, "slides=")!=NULL) {
            if (sscanf(buf, "%*[^\"]\"%d[^\"]\"", &s) == 1) {
                continue;
            } else {
                fprintf(stderr, "improper slide declaration\n");
            }
        } else if (strstr(buf, "{STARTSLIDES}")!=NULL) {
            break;
        }
    }
    *slideCounter = s;
    strcat(presTitle, globalTitle);

    // allocate memory to the heap for storing our array of slides
    // an array is used here to enable jumping to slides by number
    struct Slide *slides = (struct Slide *)malloc(s * sizeof(*slides));
    slides[0].content[0] = '\0'; // erases junk characters
    int i = 0;
    while(fgets(buf, 1000, inFile)!=NULL) {
        if (strstr(buf, "_____")!=NULL || strstr(buf, "|")!=NULL) { // finds line of slide
            strcat(slides[i].content, buf);
        } else if (strstr(buf, "{ENDSLIDE}")!=NULL) { // iterate to the next slide
            slides[i].index = i+1;
            slides[i].x = x;
            slides[i].y = y;
            slides[i].r = 0;
            slides[i].g = 0;
            slides[i].b = 0;
            i++;
            if (i>=s)
                break;
            slides[i].content[0] = '\0';
        }
    }
    return slides;
}
// vim: set ts=4 sts=4 sw=4:
