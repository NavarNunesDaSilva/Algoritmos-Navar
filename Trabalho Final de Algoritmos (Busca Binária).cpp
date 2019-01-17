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

int conversorint(string aconverter)  //Estrutura feita apenas para a convers�o
{
    int n;
    stringstream numeroInt(aconverter);
    numeroInt >> n;
    return n;
}

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

int falhar()
{
	switch(estado)
	{
	    case 0: partida = 9; break;
        case 9: partida = 12; break;
        case 12: partida = 20; break;
        case 20: partida = 25; break;
        case 25: partida = 32; break;
        case 32: partida = 33; break;
        case 31:
            //retornar msg de erro, porque danou-se tudo
            cout << "Linha " << linhas << " : Erro -> Fim de arquivo inesperado\n";
            estado = 0;
            cont_sim_lido++;
            break;
        case 33:
            //retornar msg de erro, porque danou-se tudo
            cout << "Linha " << linhas << ": Erro encontrado no codigo\n";
            cont_sim_lido++;
            estado = 0;
            break;

        default: printf("Erro do compilador");
	}
	return(partida);
}


Token proximo_token()
{
	Token token;


	while((code[cont_sim_lido] != EOF) && (c != '\0'))
        {
		switch(estado)
		{

			case 0:                          //Case inicial
				c = code[cont_sim_lido];
				if (c == '\n')
                {
                    linhas++;
                    estado = 0;
					cont_sim_lido++;
                }
				else if((c == ' ') || (c == '\t'))
                {
					estado = 0;
					cont_sim_lido++;
                }
				else if(c == '<') estado = 1;
				else if(c == '=') estado = 5;
				else if(c == '>') estado = 6;
				else if(c == '/') estado = 28;
				else if(c == '{') estado = 31;
				else if(c == '}')
                {
                    cout << "Linha " << linhas << " : Erro -> Esperava o sinal de abertuda de comentario antes" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    break;
                }
				else
                {
					 estado = falhar();
                }
				break;

			case 1:                         //Case ap�s o "<"
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if(c == '=') estado = 2;
				else if(c == '>') estado = 3;
				else
                {
                    estado = 4;
                }
				break;

			case 2:                        //Case do "<="
				cont_sim_lido++;
				printf("<relop, '<'>\n");
				token.nome_token = RELOP;
				token.atributo = LE;
				estado = 0;
				return(token);
				break;

			case 3:                        //Case do "<>"
				cont_sim_lido++;
				printf("<relop, '='>\n");
				token.nome_token = RELOP;
				token.atributo = NE;
				estado = 0;
				return(token);
				break;

			case 4:                        //Caso do "<" sozinho ou com algo diferente depois
				cont_sim_lido++;  //precisa?
				printf("<relop, '>'>\n");
				token.nome_token = RELOP;
				token.atributo = LT;
				estado = 0;
				return(token);
				break;

			case 5:                        //Caso do "="
				cont_sim_lido++;
				printf("<relop, '\'>\n");
				token.nome_token = RELOP;
				token.atributo = EQ;
				estado = 0;
				return(token);
				break;

			case 6:                       //Caso do ">"
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c == '=') estado = 7;
				else estado = 8;
				break;

			case 7:                      //Caso do ">="
				cont_sim_lido++;
				printf("<relop, '{'>\n");
				token.nome_token = RELOP;
				token.atributo = GE;
				estado = 0;
				return(token);
				break;

			case 8:                      //Caso do "=" com algo diferente na frente
				cont_sim_lido++;  //Precisa?
				printf("<relop, '}'>\n");
				token.nome_token = RELOP;
				token.atributo = GT;
				estado = 0;
				return(token);
				break;

			case 9:                      //Estava no estado zero, mas n�o achou qualquer (< > =) ou espa�amento
			    c = code[cont_sim_lido];
			    c2 = code[(cont_sim_lido+2)];
                if (c == '.')
                {
                    if (c2 == '\0')
                    {
                        cout<< "<simbolo especial, " << c << ">" << endl;
                        token.nome_token = PONTO_OP;
                        token.atributo = NULL;
                        exit(EXIT_SUCCESS);
                        cont_sim_lido++;
                        estado = 0;
                        return(token);
                        break;
                    }
                }
			    else if (c == '\n')
                {
                    linhas++;
                    estado = 9;
					cont_sim_lido++;
                }
				else if((c == ' ')||(c == '\t'))
                {
					estado = 9;
					cont_sim_lido++;
				}
				else if(isalpha(c) || (c == '_'))
				{
                        contlimite++;
                        if (contlimite > limite)  //Verifica se o limite de caracteres foi atingido
                        {
                            while(fimdocomentario != 1)
                            {
                                if (c == '\n')
                                {
                                    fimdocomentario = 1;
                                    linhas++;
                                }
                                else if((c == ' ')||(c == '\t'))
                                {
                                    fimdocomentario = 1;
                                }
                                else if((isalpha(c))||(isdigit(c)))
                                {
                                    cont_sim_lido++;
                                    c = code[cont_sim_lido];
                                }
                                else
                                {
                                    fimdocomentario = 1;
                                }
                            }
                            cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no indentificador)" << endl;
                            fimdocomentario = 0;
                            contlimite = 0;
                            estado = 0;
                            break;
                        }
                        else
                        {
                            c_string = c;
                            palavra.push_back(c_string);
                            estado = 10;
                        }
                }
				else
                {
                    estado = falhar();
                }
				break;

            case 10:                    //Caso ache letras,e segue at� a palavra terminar
                cont_sim_lido++;
				c = code[cont_sim_lido];
				c2 = code[(cont_sim_lido+2)];
                if (c == '.')
                {
                    if (c2 == '\0')
                    {
                        estado = 11;
                        break;
                    }
                }
				else if((isalpha(c)) || (isdigit(c)) || (c == '_'))
                {
                    contlimite++;
                    if (contlimite > limite)  //Verifica se o limite de caracteres foi atingido
                    {
                        while(fimdocomentario != 1)
                        {
                            if (c == '\n')
                            {
                                linhas++;
                                fimdocomentario = 1;
                            }
                            else if((c == ' ') || (c == '\t'))
                            {
                                fimdocomentario = 1;
                            }
                            else if((isalpha(c))||(isdigit(c)))
                            {
                                cont_sim_lido++;
                                c = code[cont_sim_lido];
                            }
                            else
                            {
                                fimdocomentario = 1;
                            }
                        }
                        cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no indentificador)" << endl;
                        fimdocomentario = 0;
                        contlimite = 0;
                        estado = 0;
                        break;
                    }
                    else
                    {
                        c_string = c;
                        palavra.push_back(c_string);
                        estado = 10;
                    }
                }
				else
                    estado = 11;
				break;

            case 11:                   //Est� pegando a palavra, mas acha algo diferente de uma letra
                achou = 0;
                //cont_sim_lido++;   //Precisa?

                for(i=0; i < palavra.size(); i++) //concatena uma sequencia de caracteres em uma unica string
                {
                    palavracompleta.insert(i,palavra[i]);
                }
                for(i=0; i < tabela.size(); i++) //verifica se uma palavra est� na tabela, se n�o, insere nova palavra e sua posicao, se sim, sua posicao
                {
                    if(palavracompleta == tabela[i])
                    {
                        posicao = i;
                        achou = 1;
                        break;
                    }
                }
                if (achou == 0)
                {
                    cout << "A palavra NAO esta na tabela. Sera acrescentada agora\n";
                    tabela.push_back(palavracompleta);
                    posicao = (i);
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


				cout<< "<" << palavracompleta << " ," << posicao << ">" << endl;
				contlimite = 0;
				estado = 0;
				palavra.clear();
				palavracompleta.clear();

				c2 = code[(cont_sim_lido+2)];
                if (c == '.')
                {
                    if (c2 == '\0')
                    {
                        cout<< "<simbolo especial, " << c << ">" << endl;
                        token.nome_token = PONTO_OP;
                        token.atributo = NULL;
                        exit(EXIT_SUCCESS);
                        cont_sim_lido++;
                        estado = 0;
                        return(token);
                        break;
                    }
                }

				return(token);
				break;

			case 12:                    //Estava no estado 9, mas n�o achou espa�amento ou uma letra
				c = code[cont_sim_lido];
				if (c == '\n')
                {
                    linhas++;
                    estado = 12;
					cont_sim_lido++;
                }
				else if((c == ' ')||(c == '\t'))
                {
					estado = 12;
					cont_sim_lido++;
				}
				else if(isdigit(c))
				{

				    contlimite++;
				    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else if(c == '\0')  //caso a cadeia acabe
                    {
                        estado = 19;
                        break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 13;
                    }
                }
				else
					estado = falhar();
				break;

            case 13:              //Achou um n�mero
                cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c == '.')
                {
                    estado = 14;
                    break;
                }
				else if(isdigit(c))
                {

                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else if(c == '\0')  //caso a cadeia acabe
                    {
                        estado = 19;
                        break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 13;
                    }
                }
				else if(c == 'E')
				{
				    c_string = c;
				    numero.push_back(" ");
				    numero.push_back(c_string);
				    estado = 16;
				}
				else
                {
                    estado = 19;  //Sem essa condi��o n�o era poss�vel chegar ao resultado desejado. Caso esse estado esteja lendo um n�mero e veja algo diferente, o estado n�o alterava
                }
				break;

            case 14:              //Estava lendo um n�mero, e acabou achando um '.'
                c2 = code[(cont_sim_lido+2)];
                if (c == '.')
                {
                    if (c2 == '\0')
                    {
                        cout<< "<simbolo especial, " << c << ">" << endl;
                        token.nome_token = PONTO_OP;
                        token.atributo = NULL;
                        exit(EXIT_SUCCESS);
                        cont_sim_lido++;
                        estado = 0;
                        return(token);
                        break;
                    }
                    else
                    {
                        estado = 19;
                        break;
                    }
                }
                else
                {
                    c = code[cont_sim_lido];
                    c_string = c;
                    numero.push_back(c_string);
                    cont_sim_lido++;
                }
				if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 15;
                    }
                }

                //Cabe uma condi��o de erro aqui

				break;

            case 15:              //Leu um n�mero ap�s ver o ponto
                cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 15;
                    }
                }
				else if((c == 'E'))
				{
				    c_string = c;
				    numero.push_back(" ");
				    numero.push_back(c_string);
				    estado = 16;
				}
				else
				{
				    estado = 19;
				}
				break;

            case 16:           //Achou o 'E' enquanto lia um n�mero
                cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c == ' ')
                {
                    contlimite++;
                    estado = 16;
                }
				if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 18;
                    }
                }
                else if((c == '+')||(c == '-'))
                {
                    c_string = c;
				    numero.push_back(c_string);
                    estado = 17;
                }
                else
                {
                    cout << "Linha " << linhas <<" : Erro -> (Esperava um sinal ou numero no expoente)" << endl;
                    estado = 0;
                }
				break;

            case 17:             //Leu um + ou - ap�s ler um n�mero junto de um 'E'
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 18;
                    }
                }
				break;

            case 18:             //Leu um n�mero ap�s ver um +, - ou E no estado 16
                //cout << "\n1?";
                cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    numero.clear();
                    numeroCompleto.clear();
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 18;
                    }
                }
				else
                    estado = 19;
				break;

            case 19:                //Leu algo diferente ap�s estar lendo um n�mero
                //cont_sim_lido++;  //Precisa?

                if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    numero.clear();
                    numeroCompleto.clear();
                    break;
                }
                else
                {
                    contlimite = 0;
                }

                for(i=0; i < numero.size(); i++) //concatena uma sequencia de caracteres em uma unica string
                {
                    numeroCompleto.insert(i,numero[i]);
                }

                cout << "<num, " << numeroCompleto << ">" << endl;

				token.nome_token = NUM;
				token.atributo = conversorint(numeroCompleto); //Chamando a funcao que converte string pra inteiros

				estado = 0;
				numero.clear();
				numeroCompleto.clear();
				if (c == '.')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    token.nome_token = PONTO_OP;
                    token.atributo = NULL;
                    exit(EXIT_SUCCESS);
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
				return(token);
				break;

			case 20:                   //Estava no estado 12, e n�o achou um n�mero
				c = code[cont_sim_lido];
				if (c == '\n')
                {
                    linhas++;
                    estado = 20;
					cont_sim_lido++;
                }
				else if((c == ' ')||(c == '\t'))
                {
					estado = 20;
					cont_sim_lido++;
				}
				else if(isdigit(c))
				{
				    contlimite++;
				    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    numero.clear();
                    numeroCompleto.clear();
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 21;
                    }
                }
				else
				{
				    estado = falhar();
				}
				break;

            case 21:              //Saiu do estado 20 ap�s finalmente ler um n�mero
                cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 21;
                    }
                }
                else if(c == '.')
                {
                    c_string = c;
                    numero.push_back(c_string);
                    estado = 22;
                }
				break;

            case 22:            //Estava lendo um n�mero no estado 21, e viu um '.'
				cont_sim_lido++;
				c = code[cont_sim_lido];
				c2 = code[(cont_sim_lido+2)];
                if (c2 == '\0')
                {
                    estado = 19;
                    break;
                }
				else if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 23;
                    }
                }
				break;

            case 23:            //Viu um n�mero ap�s ler o '.' do estadon 22
                cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 23;
                    }
                }
				else
				{
				    estado = 24;
				}
				break;

            case 24:               //Viu outra coisa ao ler o '.' no estado 23

                for(i=0; i < numero.size(); i++) //concatena uma sequencia de caracteres em uma unica string
                {
                    numeroCompleto.insert(i,numero[i]);
                }

				token.nome_token = NUM;
				token.atributo = conversorint(numeroCompleto); //Chamando a funcao que converte string pra inteiros

				cout << "<num, " << numeroCompleto << ">" << endl;
				estado = 0;
				numero.clear();
				numeroCompleto.clear();
				return(token);
				break;

			case 25:                    //Estava no estado 20, e n�o achou um n�mero ainda
				c = code[cont_sim_lido];
				if (c == '\n')
                {
                    linhas++;
                    estado = 25;
					cont_sim_lido++;
                }
				else if((c == ' ')||(c == '\t'))
                {
					estado = 25;
					cont_sim_lido++;
				}
				else if(isdigit(c))
				{
				    contlimite++;
				    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                        cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                        estado = 0;
                        contlimite = 0;
                        break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 26;
                    }
                }
				else
				{
				    estado = falhar();
				}
				break;

            case 26:             //Achou uma letra no estado 25
                cont_sim_lido++;
				c = code[cont_sim_lido];
				if(isdigit(c))
                {
                    contlimite++;
                    if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                    {
                        cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                        estado = 0;
                        contlimite = 0;
                        break;
                    }
                    else
                    {
                        c_string = c;
                        numero.push_back(c_string);
                        estado = 26;
                    }
                }
                else
                {
                    estado = 27;
                }

				break;

            case 27:              //Leu outra coisa enquanto lia a palavra no estado 26

                for(i=0; i < numero.size(); i++) //concatena uma sequencia de caracteres em uma unica string
                {
                    numeroCompleto.insert(i,numero[i]);
                }
				token.nome_token = NUM;
				token.atributo = conversorint(numeroCompleto); //Chamando a funcao que converte string pra inteiros

				if (contlimite>limite)  //Verifica se o limite de caracteres foi atingido
                {
                    cout << "Linha " << linhas <<" : Erro -> (Limite de digitos alcancado no numero)" << endl;
                    estado = 0;
                    contlimite = 0;
                    break;
                }
                else
                {
                    contlimite = 0;
                }

				cout<< "<num, " << numeroCompleto << ">" << endl;
				estado = 0;
				numero.clear();
				numeroCompleto.clear();
				return(token);
				break;

            case 28:  //caso veja uma barra no estado 0
                cont_sim_lido++;
                c = code[cont_sim_lido];
				if((c == '/'))
                {
					while(1)
                    {
                        cont_sim_lido++;
                        c = code[cont_sim_lido];

                        if(c == '\n')
                        {
                            linhas++;
                            break;
                        }
                    }
                }

                printf("<coment, >\n");
				estado = 0;
				return(token);

				break;

            case 31:
                while(fimdocomentario != 1)
                {
                    cont_sim_lido++;
                    c = code[cont_sim_lido];

                    if(c == '}')
                    {
                        cont_sim_lido++;
                        printf("<coment, >\n");
                        estado = 0;
                        return(token);
                        fimdocomentario = 1;
                    }
                    if(c == '{')
                    {
                        estado = falhar();
                        fimdocomentario = 1;
                    }
                    if(c == '\0')
                    {
                        estado = falhar();
                        fimdocomentario = 1;
                    }
                }

                fimdocomentario = 0;
				break;

            case 32:
                c = code[cont_sim_lido];
                if (c == '@')
                {
                    cout << "Linha " << linhas <<" : Erro -> caracter invalido " << c << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '#')
                {
                    cout << "Linha " << linhas <<" : Erro -> caracter invalido  " << c << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '$')
                {
                    cout << "Linha " << linhas <<" : Erro -> caracter invalido " << c << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '%')
                {
                    cout << "Linha " << linhas <<" : Erro -> caracter invalido " << c << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '�')
                {
                    cout << "Linha " << linhas <<" : Erro -> caracter invalido " << c << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '&')
                {
                    cout << "Linha " << linhas <<" : Erro -> caracter invalido " << c << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '?')
                {
                    cout << "Linha " << linhas <<" : Erro -> caracter invalido " << c << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '!')
                {
                    cout << "Linha " << linhas <<" : Erro -> caracter invalido " << c << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else
				{
				    estado = falhar();
				    break;
				}

            case 33:
                c = code[cont_sim_lido];
                if (c == '+')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '-')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == ',')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '*')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '(')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == ')')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '.')
                {
                    c2 = code[(cont_sim_lido+2)];
                    if (c2 == '\0')
                    {
                        cout<< "<simbolo especial, " << c << ">" << endl;
                        exit(EXIT_SUCCESS);
                        cont_sim_lido++;
                        estado = 0;
                        return(token);
                        break;
                    }
                }
                else if (c == ';')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '\'')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == '"')
                {
                    cout<< "<simbolo especial, " << c << ">" << endl;
                    cont_sim_lido++;
                    estado = 0;
                    return(token);
                    break;
                }
                else if (c == ':')
                {
                    cont_sim_lido++;
                    c = code[cont_sim_lido];
                    if (c == '=')
                    {
                        cout<< "<simbolo especial, " << c << ">" << endl;
                        cont_sim_lido++;
                        estado = 0;
                        return(token);
                        break;
                    }
                    else
                    {
                        cout<< "<simbolo especial, :>" << endl;
                        estado = 0;
                        return(token);
                        break;
                    }

                }
                else
                {
                    contlimite = 0;
                    estado = falhar();
				    break;
                }

		}

	}
	token.nome_token = EOF;
	token.atributo = -1;
	return(token);
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
