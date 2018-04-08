#ifndef LAB3_H_   /* Include guard */
#define LAB3_H_

#define INICIO_P 0
#define INICIO_I 1
#define FIM_P 2
#define FIM_I 3

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

double protecao_civil(lista *event_list, variables *init_variables);

void proccess();

#endif // LAB3_H_
