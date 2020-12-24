#include <stdlib.h>
#include <stdio.h>
#include "scanner.h"

TokenType Token;
int		  Num;
char	  Id[MAX_IDENT_LEN + 1];

FILE * f;
char ch;
int line = 1, col = 1;
int lineErr = 1, colErr = 1;

// Khai bao ham expression
void expression();

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
	char c = fgetc(f);
	if(c == '\n') {
		++line;
		col = 1;
	} else {
		++col;
	}
	return toupper(c);
}

TokenType getToken(){
	
	//Ch chua ky tu doc duoc tu van ban nguon boi ham getCh()
	while(ch == ' ' || ch == '\n' || ch == '\t') ch = getCh();
	lineErr = line;
	colErr = col;

	if(isalpha(ch)) {
		int pos = 0;
		Id[pos] = ch;
		ch = getCh();
		while(isalpha(ch) || isdigit(ch)) {
			pos++;
			if(pos < MAX_IDENT_LEN) {
				Id[pos] = ch;
				ch = getCh();
			}
			else {
				while(isalpha(ch) || isdigit(ch)) ch = getCh();
			}
		}
		if(pos < MAX_IDENT_LEN) {
			pos++;
			Id[pos] = '\0';
		}
		else {
			Id[MAX_IDENT_LEN] = '\0';
		}

		return checkKeyword(Id);
	}
	else if(isdigit(ch)) {
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
	}
	else if(ch == '+') {
		ch = getCh();
		return PLUS;
	}
	else if(ch == '-') {
		ch = getCh();
		return MINUS;
	}
	else if(ch == '*') {
		ch = getCh();
		return TIMES;
	}
	else if(ch == '/') {
		ch = getCh();
		return SLASH;
	}
	else if(ch == '=') {
		ch = getCh();
		return EQU;
	}
	else if(ch == '<') {
		ch = getCh();
		if(ch == '=') {
			ch = getCh();
			return LEQ;
		}
		else if(ch == '>') {
			ch = getCh();
			return NEQ;
		}
		else {
			return LSS;
		}
	}
	else if(ch == '>') {
		ch = getCh();
		if(ch == '=') {
			ch = getCh();
			return GEQ;
		}
		else {
			return GTR;
		}
	}
	else if(ch == '(') {
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
			return getToken();
		} else {
			return LPARENT;
		}
	}
	else if(ch == ')') {
		ch = getCh();
		return RPARENT;
	}
	else if(ch == '[') {
		ch = getCh();
		return LBRACK;
	}
	else if(ch == ']') {
		ch = getCh();
		return RBRACK;
	}
	else if(ch == ':') {
		ch = getCh();
		if(ch == '=') {
			ch = getCh();
			return ASSIGN;
		} else {
			return NONE;
		}
	}
	else if(ch == ';') {
		ch = getCh();
		return SEMICOLON;
	}
	else if(ch == ',') {
		ch = getCh();
		return COMMA;
	}
	else if(ch == '.') {
		ch = getCh();
		return PERIOD;
	}
	else if(ch == '%') {
		ch = getCh();
		return PERCENT;
	}
	return NONE;
}

//Bao loi
void error(const char msg[]) {
	printf("%s\n", msg);
	printf("Line: %d, Col: %d", lineErr, colErr-1);
	exit(0);
}

//Phan tich nhan tu
void factor() {
	if(Token == IDENT) {
		Token = getToken();
		if(Token == LBRACK) {
			Token = getToken();
			expression();
			if(Token == RBRACK) {
				Token = getToken();
			}
			else error("Khong co dau ]");
		}
	}
	else if(Token == NUMBER) {
		Token = getToken();
	}
	else if(Token == LPARENT) {
		Token = getToken();
		expression();
		if(Token == RPARENT) {
			Token = getToken();
		}
		else error("Thieu dau )");
	}
	else {
		error("Factor: Syntax error");
	}
}

//Phan tich so hang
void term() {
	factor();
	while(Token == TIMES || Token == SLASH || Token == PERCENT) {
		Token = getToken();
		factor();
	}
}

//Phan tich bieu thuc
void expression() {
	if(Token == PLUS || Token == MINUS) {
		Token = getToken();
	}
	term();
	while(Token == PLUS || Token == MINUS) {
		Token = getToken();
		term();
	}
}

//Phan tich dieu kien
void condition() {
	if(Token == ODD) {
		Token = getToken();
		expression();
	}
	else {
		expression();
		if(Token == EQU || Token == NEQ || Token == LSS || Token == LEQ || Token == GTR || Token == GEQ) {
			Token = getToken();
			expression();
		}
		else error("Condition: Syntax error");
	}
}

//Phan tich cau lenh
void statement() {
	if(Token == IDENT) {
		Token = getToken();
		if(Token == LBRACK) {
			Token = getToken();
			expression();
			if(Token == RBRACK) {
				Token = getToken();
			}
			else error("Thieu dau ]");
		}

		if(Token == ASSIGN) {
			Token = getToken();
			expression();
		}
		else error("Thieu toan tu gan");
	}
	else if(Token == CALL) {
		Token = getToken();
		if(Token == IDENT) {
			Token = getToken();
			if(Token == LPARENT) {
				Token = getToken();
				expression();
				while(Token == COMMA) {
					Token = getToken();
					expression();
				}
				if(Token == RPARENT) Token = getToken();
				else error("Thieu dau )");
			}
		}
		else error("Thieu ten thu tuc ham");
	}
	else if(Token == BEGIN) {
		Token = getToken();
		statement();
		while (Token == SEMICOLON) {
			Token = getToken();
			statement();
		}
		if(Token == END) Token = getToken();
		else error("Thieu tu khoa End");
	}
	else if(Token == IF) {
		Token = getToken();
		condition();
		if(Token == THEN) {
			Token = getToken();
			statement();
			if(Token == ELSE) {
				Token = getToken();
				statement();
			}
		}
		else error("Thieu tu khoa Then");
	}
	else if(Token == WHILE) {
		Token = getToken();
		condition();
		if(Token == DO) {
			Token = getToken();
			statement();
		}
		else error("Thieu tu khoa Do");
	}
	else if(Token == FOR) {
		Token = getToken();
		if(Token == IDENT) {
			Token = getToken();
			if(Token == ASSIGN) {
				Token = getToken();
				expression();
				if(Token == TO) {
					Token = getToken();
					expression();
					if(Token == DO) {
						Token = getToken();
						statement();
					}
					else error("Thieu tu khoa Do");
				}
				else error("Thieu tu khoa To");
			}
			else error("Thieu lenh gan");
		}
		else error("Thieu ten bien");
	}
	else {

	}
}

//Phan tich cac khoi cau lenh
void block() {
	if(Token == CONST) {
		Token = getToken();
		while(1) {
			if(Token == IDENT) {
				Token = getToken();
				if(Token == EQU) {
					Token = getToken();
					if(Token == NUMBER) {
						Token = getToken();
						if(Token == COMMA) {
							Token = getToken();
						}
						else if(Token == SEMICOLON) {
							Token = getToken();
							break;
						}
						else {
							if(Token == IDENT) {
								error("Thieu dau ,");
							}
							else error("Thieu dau ;");
						}
					}
					else error("Thieu so");
				}
				else error("Thieu dau =");
			}
			else error("Thieu dinh danh cho bien");
		}
	}

	if(Token == VAR) {
		Token = getToken();
		while(1) {
			if(Token == IDENT) {
				Token = getToken();
				if(Token == LBRACK) {
					Token = getToken();
					if(Token == NUMBER) {
						Token = getToken();
						if(Token == RBRACK) {
							Token = getToken();
						}
						else error("Thieu dau ]");
					}
					else error("Thieu so luong phan tu cua mang");
				}

				if(Token == COMMA) {
					Token = getToken();
				}
				else if(Token == SEMICOLON) {
					Token = getToken();
					break;
				}
				else {
					if(Token == IDENT) {
						error("Thieu dau ,");
					}
					else error("Thieu dau ;");
				}
			}
			else error("Thieu dinh danh");
		}
	}

	while(Token == PROCEDURE) {
		Token = getToken();
		if(Token == IDENT) {
			Token = getToken();
			if(Token == LPARENT) {
				Token = getToken();
				while(1) {
					if(Token == VAR) {
						Token = getToken();
					}
					if(Token == IDENT) {
						Token = getToken();
						if(Token == SEMICOLON) {
							Token = getToken();
						}
						else if(Token == RPARENT) {
							Token = getToken();
							break;
						}
						else {
							if(Token == VAR || Token == IDENT) {
								error("Thieu dau ;");
							}
							else error("Thieu dau )");
						}
					}
					else error("Thieu ten bien");
				}
			}

			if(Token == SEMICOLON) {
				Token = getToken();
				block();
				if(Token == SEMICOLON) {
					Token = getToken();
				}
				else error("Thieu dau ;");
			}
			else error("Thieu dau ;");
		}
		else error("Thieu dinh danh cho procedure");
	}

	if(Token == BEGIN) {
		Token = getToken();
		statement();
		while(Token == SEMICOLON) {
			Token = getToken();
			statement();
		}
		if(Token == END) {
			Token = getToken();
		}
		else error("Thieu tu khoa End");
	}
	else error("Thieu tu khoa Begin");

}

//Phan tich chuong trinh
void program() {
	if(Token == PROGRAM) {
		Token = getToken();
		if(Token == IDENT) {
			Token = getToken();
			if(Token == SEMICOLON) {
				Token = getToken();
				block();
				if(Token == PERIOD) {
					printf("Thanh cong");
				} else error("Thieu dau .");
			} else error("Thieu dau ;");
		} else error("Thieu ten chuong trinh");
	} else error("Thieu tu khoa Program");
}

void compile(char * filename){
  	if((f = fopen(filename,"rt")) == NULL) printf("File %s not found",filename);
  	else{
		ch = ' ';
		Token = getToken();
		program();
	}
  fclose(f);
}
