#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <bits/stdc++.h>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>

#define IF  256
#define THEN  257
#define ELSE  258
#define RELOP  259
#define ID  260
#define NUM  261
#define COMENT 268
#define SIMBOLO_INVALIDO 269
#define PALAVRA_RESERVADA_PROGRAM 271
#define ADD_OP 272
#define SUB_OP 273
#define DIV_OP 274
#define VIRGULA 275
#define PONTO_VIRG 276
#define MULT_OP 277
#define LEFT_PAREN 278
#define RIGHT_PAREN 279
#define PONTO_OP 280
#define ATRIB 281
#define INT 282
#define REAL 283
#define BEGIN 284
#define END 285
#define AND 286
#define OR 287
#define WHILE 288
#define DO 289
#define VAR 290
#define PROCEDURE 291
#define PROGRAM 292
#define INICIO 296
#define FIM 297
#define WRITE 298
#define READ 307
#define BOOLEAN 299
#define TRUE 300
#define FALSE 301
#define FUNCAO 302
#define NOT 303
#define DOIS_PONTOS 304

#define LT 262
#define LE 263
#define EQ 264
#define NE 265
#define GT 266
#define GE 267
#define IDEN 270

using namespace std;

//infelizmente, ap�s 2 semanas tentando, n�o consegui instalar o google tests no computador, al�m do fato de que o computador � dividido com o trabalho de outra pessoa, e n�o posso particionar um linux.

struct Token
{
 int nome_token;
 int atributo;
};



Token token;

int i;
int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
int valor_lexico;
int achou;

int limite = 20;
int contlimite = 0;
int fimdocomentario = 0;
int linhas = 0;

char c = 'a';  //Cria esse char para, al�m de ler o texto, tamb�m serve para comparar com o for que chama o Token
char c2 = 'b';
char *code;

vector <string> tabela;
vector <string> palavra;
string palavracompleta;
vector <string> numero;
string numeroCompleto;
string c_string;
int posicao; //indica a posicao da palavra na tabela

char *readFile(char *fileName)
{
	FILE *file = fopen(fileName, "r");
	char *code;
	int nmp = 0;
	int cvb;

	if(file == NULL) return NULL;

	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	code = new char (f_size);

	while ((c = fgetc(file))!= EOF)
    {
        code[nmp++] = (char) c;
	}
	code[nmp] = ' ';  //Como o c�digo pega palavras completas, � necess�rio adicionar um espa�o antes do sinal de final de cadeia

	nmp++;

	code[nmp] = '\0';

	return code;
}


Token proximo_token()
{
	Token token;


	while((code[cont_sim_lido] != EOF) && (c != '\0'))
        {
            c = code[cont_sim_lido];
            cont_sim_lido++;

            c_string = c;
            palavra.push_back(c_string);

				if (c == '\n')
                {
					break;
                }
				else if((c == ' ') || (c == '\t'))
                {
					cont_sim_lido++;
					c = code[cont_sim_lido];
                }
                achou = 0;


                palavracompleta.insert(i,palavra[i]);

                for(i=0; i < tabela.size(); i++) //verifica se uma palavra est� na tabela, se n�o, insere nova palavra e sua posicao, se sim, sua posicao
                {
                    if(palavracompleta == tabela[i])
                    {
                        posicao = i;
                        achou = 1;
                        break;
                    }
                }
                if (achou == 1)
                {
                    posicao = (i);
                    cout<< "<" << palavracompleta << " ," << posicao << ">" << endl;

                    palavra.clear();
                }

                if(palavracompleta == "if")
                {
                    token.nome_token = IF;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "else")
                {
                    token.nome_token = ELSE;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "then")
                {
                    token.nome_token = THEN;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "int")
                {
                    token.nome_token = INT;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "real")
                {
                    token.nome_token = REAL;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "begin")
                {
                    token.nome_token = BEGIN;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "end")
                {
                    token.nome_token = END;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "and")
                {
                    token.nome_token = AND;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "or")
                {
                    token.nome_token = OR;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "while")
                {
                    token.nome_token = WHILE;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "do")
                {
                    token.nome_token = DO;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "var")
                {
                    token.nome_token = VAR;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "procedure")
                {
                    token.nome_token = PROCEDURE;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "program")
                {
                    token.nome_token = PROGRAM;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "inicio")
                {
                    token.nome_token = INICIO;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "fim")
                {
                    token.nome_token = FIM;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "write")
                {
                    token.nome_token = WRITE;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "read")
                {
                    token.nome_token = READ;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "boolean")
                {
                    token.nome_token = BOOLEAN;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "true")
                {
                    token.nome_token = TRUE;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "false")
                {
                    token.nome_token = FALSE;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "funcao")
                {
                    token.nome_token = FUNCAO;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "not")
                {
                    token.nome_token = NOT;
                    token.atributo = NULL;
                }
                else if(palavracompleta == "div")
                {
                    token.nome_token = DIV_OP;
                    token.atributo = NULL;
                }
                else
                {
                    token.nome_token = ID;
                    token.atributo = posicao;
                }

				return(token);

	}
	token.nome_token = EOF;
	token.atributo = -1;
	return(token);

	palavracompleta.clear();
}


int main ()
{
    tabela.push_back("if");
    tabela.push_back("then");
    tabela.push_back("else");
    tabela.push_back("int");
    tabela.push_back("real");
    tabela.push_back("begin");
    tabela.push_back("end");
    tabela.push_back("and");
    tabela.push_back("or");
    tabela.push_back("while");
    tabela.push_back("do");
    tabela.push_back("var");
    tabela.push_back("procedure");
    tabela.push_back("program");
    tabela.push_back("inicio");
    tabela.push_back("fim");
    tabela.push_back("write");
    tabela.push_back("read");
    tabela.push_back("boolean");
    tabela.push_back("true");
    tabela.push_back("false");
    tabela.push_back("funcao");
    tabela.push_back("not");
    tabela.push_back("div");

    code = readFile("entrada.txt");

    while(c != '\0') //Toda string termina no caractere "\o". Por isso, o token ser� chamado at� que a string tenha sido completamente lida
    {
        token = proximo_token();
    }

    cout << "\n\n";

    system ("pause");
}

// Por Navar Nunes da Silva
