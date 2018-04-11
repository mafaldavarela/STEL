#ifndef LAB3_H_   /* Include guard */
#define LAB3_H_

#define INICIO_P 0
#define INICIO_I 1
#define FIM_P 2
#define FIM_I 3
#define INICIO_P_I 4
#define FIM_P_I 5
#define INICIO 6
#define FIM 7
#define WAITING_P 8
#define WAITING_P_I 9
#define WAITING_I 10

typedef struct{
  lista * event_list;
	lista * waiting_list;
	int waiting_clients;
	int occupied_channels;
} sistema;

typedef struct{
	sistema * inem;
	sistema * protecao;
  int clients_handled;
	double clock;
} servidor;


typedef struct{
	int m;
	int L;
	int k;
  int waiting_buffer_pro;
  int mi;
} variables;

double exponential(double aux, int type);

lista * add_init_event(lista *event_list, int mode, double current_time, double lambda);

lista * add_end_event(lista *event_list, int mode, double current_time);

double time_inem();

double box_muller (double sigma, double mu, double max, double min);

void proccess(lista * protecao_event_list, variables * init_variables, double lambda);


#endif // LAB3_H_
