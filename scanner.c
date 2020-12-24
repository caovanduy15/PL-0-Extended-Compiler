#include <stdio.h>
#include "scanner.h"

TokenType Token;
int		  Num;
char	  Id[MAX_IDENT_LEN + 1];

FILE * f;
char ch;

TokenType checkKeyword(char * str){
	//TODO: Kiem tra tham so truyen vao co phai la mot Tu khoa khong
	//................
	int i;
	for(i= 0; i < KEYWORDS_COUNT; i++) {
		if(!strcmp(str, keywords[i].string)) {
			return keywords[i].Token;
		}
	}
	return IDENT;
}

char getCh(){
  return toupper(fgetc(f));
}

TokenType getToken(){
//TODO:
//..............
	//Ch chua ky tu doc duoc tu van ban nguon boi ham getCh()
	while(ch == ' ' || ch == '\n' || ch == '\t') ch = getCh();

	if(isalpha(ch)) {
		int pos = 0;
		Id[pos] = ch;
		ch = getCh();
		while(isalpha(ch) || isdigit(ch)) {
			pos++;
			if(pos < MAX_IDENT_LEN) {
				Id[pos] = ch;
				ch = getCh();
			} else {
				while(isalpha(ch) || isdigit(ch)) ch = getCh();
			}
		}
		if(pos < MAX_IDENT_LEN) {
			pos++;
			Id[pos] = '\0';
		} else {
			Id[MAX_IDENT_LEN] = '\0';
		}
		
		return checkKeyword(Id);
	} else if(isdigit(ch)) {
		int length = 1;
		Num = ch - '0';
		ch = getCh();
		while(isdigit(ch)) {
			length++;
			if(length > MAX_NUMBER_LEN) return NONE;
			Num = Num*10 + (ch - '0');
			ch = getCh();
		}
		return NUMBER;
	} else if(ch == '+') {
		ch = getCh();
		return PLUS;
	} else if(ch == '-') {
		ch = getCh();
		return MINUS;
	} else if(ch == '*') {
		ch = getCh();
		return TIMES;
	} else if(ch == '/') {
		ch = getCh();
		return SLASH;
	} else if(ch == '=') {
		ch = getCh();
		return EQU;
	} else if(ch == '<') {
		ch = getCh();
		if(ch == '=') {
			ch = getCh();
			return LEQ;
		} else if(ch == '>') {
			ch = getCh();
			return NEQ;
		} else {
			return LSS;
		}
	} else if(ch == '>') {
		ch = getCh();
		if(ch == '=') {
			ch = getCh();
			return GEQ;
		} else {
			return GTR;
		}
	} else if(ch == '(') {
		ch = getCh();
		if(ch == '*') {
			ch = getCh();
			do {
				while(ch != '*') {
					ch = getCh();
					if (feof(f)) return NONE;
				}
				ch = getCh();
				if (feof(f)) return NONE;
			} while(ch != ')');
			
			ch = getCh();
			return COMMENT;
		} else {
			return LPARENT;
		}
	} else if(ch == ')') {
		ch = getCh();
		return RPARENT;
	} else if(ch == '[') {
		ch = getCh();
		return LBRACK;
	} else if(ch == ']') {
		ch = getCh();
		return RBRACK;
	} else if(ch == ':') {
		ch = getCh();
		if(ch == '=') {
			ch = getCh();
			return ASSIGN;
		}
	} else if(ch == ';') {
		ch = getCh();
		return SEMICOLON;
	} else if(ch == ',') {
		ch = getCh();
		return COMMA;
	} else if(ch == '.') {
		ch = getCh();
		return PERIOD;
	} else if(ch == '%') {
		ch = getCh();
		return PERCENT;
	} else {
		return NONE;	
	}
}


void compile(char * filename){
  	if((f = fopen(filename,"rt")) == NULL) printf("File %s not found",filename);
  	else{
		ch =' ';
		do{
		 	Token = getToken();
			printf(" %s",TabToken[Token]);
			if(Token==IDENT) printf("(%s) \n",Id);
			else if(Token==NUMBER) printf("(%d) \n",Num);
			else printf("\n");
		}while(Token != NONE);
	}
  fclose(f);
}
