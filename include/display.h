#if !defined( DISPLAY_H )
#define DISPLAY_H
#include "parser.h"

struct slides {
     slide *cur;
     int cnt;
};

void initDisplay();
int displayLoop(struct slides ss, char* title, char* fileName);

#endif
