#ifndef LAB2_H_   /* Include guard */
#define LAB2_H_

#define INICIO 0
#define FIM 1
#define WAITING 2

typedef struct{
	int occupied_channels;
  int clients_handled;
	double clock;
	int waiting;
} servidor;

typedef struct{
  double samples;
} probabilidade;

typedef struct{
  double samples;
	double ammount;
} calc_average;

lista* initialize(double dm, double l, int m, int k, char* arg);

void proccess(char* arg,double dm, double l, double t, int m, int total_samples,int max_buffer, int k, lista * event_list);

double exponential(double l);

double duration_of_call(double dm);


#endif // LAB2_H_
