#if !defined( PARSER_H )
# define PARSER_H

# ifndef FILE
#  include <stdio.h>
# endif

# include "main.h"
# include "slides.h"

// struct Slide
// {
//     char content[5000];
//     int index;
//     int x, y, r, g, b;
// };

struct Ptx
{
     char *title;
     int slds;
};


struct slide *parseTXT(FILE *inFile, int *slideCounter, char *presTitle);
#endif
