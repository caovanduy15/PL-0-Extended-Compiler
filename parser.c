#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TokenTypedef.h"

#define MAX_IDENT_LEN   10
#define N   400

typedef enum {
		constant, type, variable, procedure, parameter, array
} Object;

typedef struct {
  	char Id[MAX_IDENT_LEN + 1];
  	Object Type;
  	int numOfParam;
} Stack;

extern int lineErr, colErr;
extern TokenType Token;
extern FILE * f;
extern int Num;
extern char ch;
extern char Id[MAX_IDENT_LEN + 1];

extern TokenType getToken();

Stack stack[N];

int top = 0;

void enter(char* Id, Object Type) {
	if(top >= N-1) {
		printf("Stack Overflow");
	} else {
		top++;
		strcpy(stack[top].Id, Id);
		stack[top].Type = Type;
	}
}

int location(char* Id) {
	int i;
	for(i = top; i > 0; i--) {
		if(!strcmp(Id, stack[i].Id)) {
			return i;
		}
	}
	return 0;
}

int checkIdent(char* Id, int Tx) {
	int i;
	for(i = top; i > Tx; i--) {
		if(!strcmp(Id, stack[i].Id)) {
			return i;
		}
	}
	return 0;
}

Object getKind(int idx) {
	return stack[idx].Type;
}


// Khai bao ham expression
void expression();

//Bao loi
void error(const char msg[]) {
	printf("%s\n", msg);
	printf("Line: %d, Col: %d", lineErr, colErr);
	exit(0);
}

//Bao loi
void errorTenChuaKhaiBao(const char msg[]) {
	printf("Ten chua khai bao: %s\n", msg);
	printf("Line: %d, Col: %d", lineErr, colErr);
	exit(0);
}

//Bao loi
void errorKhongPhaiLaBien(const char msg[]) {
	printf("%s khong phai la bien\n", msg);
	printf("Line: %d, Col: %d", lineErr, colErr);
	exit(0);
}

//Bao loi
void errorKhongPhaiLaMang(const char msg[]) {
	printf("%s khong phai la mang\n", msg);
	printf("Line: %d, Col: %d", lineErr, colErr);
	exit(0);
}

//Bao loi
void errorThieuPhanTuCuaMang(const char msg[]) {
	printf("%s la mang. Can chi ro phan tu cua mang can truy cap\n", msg);
	printf("Line: %d, Col: %d", lineErr, colErr);
	exit(0);
}

//Phan tich nhan tu
void factor() {
	int p;
	if(Token == IDENT) {
		p = location(Id);
		if(p <= 0) errorTenChuaKhaiBao(Id);
		if(getKind(p) != variable && getKind(p) != array && getKind(p) != constant) {
			error("Phai la 1 bien hoac constant");
		}
		Token = getToken();
		if(getKind(p) == array && Token != LBRACK) errorThieuPhanTuCuaMang(Id);
		if(getKind(p) != array && Token == LBRACK) errorKhongPhaiLaMang(Id);
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
	int p;
	if(Token == IDENT) {
		p = location(Id);
		if(p <= 0) errorTenChuaKhaiBao(Id);
		if(getKind(p) != variable && getKind(p) != array) {
			errorKhongPhaiLaBien(Id);
		}
		Token = getToken();
		if(getKind(p) == array && Token != LBRACK) errorThieuPhanTuCuaMang(Id);
		if(getKind(p) != array && Token == LBRACK) errorKhongPhaiLaMang(Id);
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
			p = location(Id);
			if(p <= 0) errorTenChuaKhaiBao(Id);
			if(getKind(p) != procedure) {
				error("Dinh danh phai la cua procedure");
			}
			int numOfParam = 0;
			Token = getToken();
			if(Token == LPARENT) {
				Token = getToken();
				expression();
				numOfParam++;
				while(Token == COMMA) {
					Token = getToken();
					expression();
					numOfParam++;
				}
				if(Token == RPARENT) Token = getToken();
				else error("Thieu dau )");
			}
			if(stack[p].numOfParam != numOfParam) error("Sai so luong tham so cua procedure");
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
			p = location(Id);
			if(p <= 0) errorTenChuaKhaiBao(Id);
			if(getKind(p) != variable) {
				errorKhongPhaiLaBien(Id);
			}
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
void block(int Tx) {
	if(Token == CONST) {
		Token = getToken();
		while(1) {
			if(Token == IDENT) {
				if(checkIdent(Id, Tx) == 0) {
					enter(Id, constant);
				} else {
					error("Constant duoc khai bao bi trung ten");
				}
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
				if(checkIdent(Id, Tx) == 0) {
					enter(Id, variable);
				} else {
					error("Bien duoc khai bao bi trung ten");
				}
				Token = getToken();
				if(Token == LBRACK) {
					Token = getToken();
					if(Token == NUMBER) {
						stack[top].Type = array;
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
			if(checkIdent(Id, Tx) == 0) {
				enter(Id, procedure);
			} else {
				error("Trung dinh danh cho procedure");
			}
			Token = getToken();
			int t = top;
			int numOfParam = 0;
			if(Token == LPARENT) {
				Token = getToken();
				while(1) {
					if(Token == VAR) {
						Token = getToken();
					}
					if(Token == IDENT) {
						if(checkIdent(Id, t) == 0) {
							enter(Id, variable);
						} else {
							error("Parameter duoc khai bao bi trung ten");
						}
						numOfParam++;
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
			stack[t].numOfParam = numOfParam;

			if(Token == SEMICOLON) {
				Token = getToken();
				block(t);
				top = t;
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
				block(top);
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
