#if !defined( SLIDES_H )
# define SLIDES_H

struct slide
{
    int index;
    int maxLen;
    int x, y, r, g, b;
    struct line *first;
};

struct line {
    char content[256]; // number subject to change
    int r, g, b;
    struct line *prev;
    struct line *next;
};

struct slide* createSlideArray(int s);
// not currently in use
// line *newLine();
struct line *nextLine(struct line *prev);
void freeSlides(struct slide *slide[], int s);
void freeLines(struct line *first);

#endif
