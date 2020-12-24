#include "TokenTypedef.h"

#define	MAX_NUMBER_LEN   6
#define MAX_IDENT_LEN   10
#define KEYWORDS_COUNT  15

struct {
  	char string[MAX_IDENT_LEN + 1];
  	TokenType Token;
} keywords[KEYWORDS_COUNT] = {
  		{"BEGIN", BEGIN},
  		{"CALL", CALL},
  		{"CONST", CONST},
  		{"DO", DO}, 
    	{"ELSE", ELSE},
      	{"END", END},
        {"FOR", FOR},
   		{"IF", IF},
     	{"ODD",ODD},
  		{"PROCEDURE", PROCEDURE},
		{"PROGRAM", PROGRAM},
    	{"THEN", THEN},
    	{"TO", TO},
		{"VAR", VAR},
  		{"WHILE", WHILE}
};
