#if !defined( PARSER_H )
#define PARSER_H

struct Slide
{
    char content[5000];
    int number;
    int x, y, r, g, b;
};

Slide* parseTXT(FILE *inFile, int* slideCounter, char *presTitle);

#endif
