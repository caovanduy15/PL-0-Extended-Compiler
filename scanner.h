#define	MAX_NUMBER_LEN   6
#define MAX_IDENT_LEN   10
#define KEYWORDS_COUNT  15

typedef enum {NONE=0, IDENT, NUMBER,
		BEGIN, CALL, CONST, DO,  ELSE, END, FOR, IF, ODD,
		PROCEDURE, PROGRAM, THEN, TO, VAR, WHILE,
		
		PLUS, MINUS, TIMES, SLASH, EQU, NEQ, LSS, LEQ,GTR, GEQ, PERCENT,
		LPARENT, RPARENT, LBRACK,RBRACK,PERIOD,COMMA,SEMICOLON, ASSIGN
} TokenType;

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

char TabToken[][10] = {	"NONE", "IDENT", "NUMBER",		
		"BEGIN", "CALL", "CONST", "DO",  "ELSE", "END", "FOR", "IF", 
		"ODD", "PROCEDURE", "PROGRAM", "THEN", "TO", "VAR", "WHILE",
	
		"PLUS", "MINUS", "TIMES", "SLASH", "EQU", "NEQ", "LSS", 
		"LEQ", "GTR", "GEQ", "PERCENT", "LPARENT", "RPARENT", 
		"LBRACK", "RBRACK", "PERIOD", "COMMA", "SEMICOLON", "ASSIGN"
	};
