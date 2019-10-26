#if !defined( PARSER_H )
# define PARSER_H

# include "main.h"

struct Slide
{
    char content[5000];
    int index;
    int x, y, r, g, b;
};

struct Ptx
{
     char *title;
     int slds;
};


struct Slide *parseTXT(FILE *inFile, int *slideCounter, char *presTitle);

#endif
