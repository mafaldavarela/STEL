#ifndef LAB3_H_   /* Include guard */
#define LAB3_H_

#define PROTECAO 0
#define INEM 1
#define INICIO 0
#define FIM 1

typedef struct{
	int occupied_channels;
  int clients_handled;
	double clock;
	int waiting;
} servidor;

typedef struct{
	int m;
	int L;
	int K;
} variables;

double exponential(double aux, int type);

double protecao_civil();

void proccess();

#endif // LAB3_H_
