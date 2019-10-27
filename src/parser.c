#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "main.h"
#include "parser.h"
#include "slides.h"

// defaults
int x = -1;
int y = -1;
int s = -1;
char *globalTitle = NULL;

int scanr(FILE *fp, struct Ptx *ptx)
{
    char *lnptr, *tmp = lnptr = NULL;

#if HAVE_GETLINE
    /* TODO if we don't have getline, we'll need to use
     * fgets
     */
    char *line = NULL;
    size_t size, len, read = len = size = 0;

    while ((read = getline(&line, &len, fp)) != EOF) {
        lnptr = line;
        size = strlen(line) + 1;
        tmp = malloc(size);
#else
    char *buf = NULL;
    size_t size = 4096;
    buf = malloc(size * sizeof(*buf));
    while(fgets(buf, size, fp) != NULL) {
        lnptr = buf;
        tmp = malloc(size);
#endif

        if (strstr(lnptr, "title=") != NULL) {
            /* get title from file
             * FIXME: we should do something else other than title=
             * like @title or something
             */

            if (sscanf(lnptr, "%*[^\"]\"%127[^\"]\"", tmp) == 1) {
                ptx->title = strdup(tmp);
                free(tmp);
            } else {
                fprintf(stderr, "%s: %s\n", PROGNAME, "malformed title field");
                return 1;
            }
        /* } else if (strstr(lnptr, "areaX=") != NULL) { */
        /*     if (sscanf(lnptr, "%*[^\"]\"%d[^\"]\"", &x) != 1) { */
        /*         fprintf(stderr, "%s: %s\n", PROGNAME, "malformed area X field"); */
        /*         return 2; */
        /*     } */
        /* } else if (strstr(lnptr, "areaY=") != NULL) { */
        /*     if (sscanf(lnptr, "%*[^\"]\"%d[^\"]\"", &y) != 1) { */
        /*         fprintf(stderr, "%s: %s\n", PROGNAME, "malformed area Y field"); */
        /*         return 3; */
        /*     } */
            /* FIXME: replace this with a slide counter */
        /* } else if ((regexec(&rgx, line, 0, NULL, 0)) == 0) { */
        /*     ++data->sld_cnt; */
        } else if (strstr(lnptr, "{STARTSLIDES}") != NULL) {
            if (y == -1 && strstr(lnptr, ".start{}") != NULL) {
                while (fgets(lnptr, size, fp) != NULL) {
                    if (strstr(lnptr, ".end{}") == NULL) {
                        x = x == -1 ? (int)strlen(lnptr) : x;
                        ++y;
                    } else {
                        break;
                    }
                }
            }
            rewind(fp);
            break;
        }
    }
    return 0;
}

struct slide* parseTXT(FILE *inFile, int* slideCounter, char *presTitle)
{
    size_t sz = 4096;
    char *buf = malloc(sz * sizeof(*buf));
    while(fgets(buf, sz, inFile)!=NULL) {
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
# if 0
    // Slide* slides = (Slide*)malloc(s * sizeof(Slide)); original line
    struct slide* slides = createSlideArray(s);
    //slides[0].content[0] = '\0'; // erases junk characters
    struct line *l = malloc(sizeof(struct line));
    struct line *first = l;
    l->content[0] = '\0';
    int curMaxLen = 0;
    int indx = 0;
# endif
    struct slide* slides = createSlideArray(s);

    // create a line pointer, l is for iterating while first is
    // the pointer used in each slide struct
    struct line *l = malloc(sizeof(struct line));
    struct line *first = l;
    l->content[0] = '\0';

    // curMaxX and curY are used to set x/y values to slides
    int curMaxX = 0;
    int curY = 0;

    // continue itteration over file
    int indx = 0;
    while(fgets(buf, 1000, inFile)!=NULL) {
        if (strstr(buf, "{ENDSLIDE}")!=NULL) { // iterate to the next slide
            slides[indx].first = first;
            l = malloc(sizeof(struct line));
            first = l;
            /* slides[indx].maxLen = curMaxLen; */
            /* curMaxLen = 0; */
	        /* slides[indx].x = x; */
	        /* slides[indx].y = y; */
            /* slides[indx].r = 0; */
            /* slides[indx].g = 0; */
            /* slides[indx].b = 0; */
            /* ++indx; */
            /* slides[indx].index = indx; */
            /* if (indx >= s) */
            slides[indx].index = indx+1;
            // assign curMaxX to x, reset variable
            slides[indx].maxX = curMaxX;
            curMaxX = 0;
            // assign curY to y, reset variable
	        slides[indx].y = curY;
            curY = 0;
            slides[indx].r = 0;
            slides[indx].g = 0;
            slides[indx].b = 0;
            indx++;
            if (indx>=s)
                break;
        } else {
            // add buf to current line and itterate to the next
            strcat(l->content, buf);
            l = nextLine(l);

            // update curMaxX only if line is longer that previous lines
            char end = '\n';
            char *ptr = strchr(buf, end);
            if (ptr) {
                int n = ptr - buf;
                if (n>curMaxX) {
                    curMaxX = n;
                }
                curY++;
            }
        }
    }
    return slides;
}
// vim: set ts=4 sts=4 sw=4:
