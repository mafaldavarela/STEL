#ifndef LAB2_H_   /* Include guard */
#define LAB2_H_

#define INICIO 0
#define FIM 1

typedef struct{
	int occupied_channels;
  int clients_handled;
	double clock;
} servidor;

typedef struct{
  double samples;
} probabilidade;

lista* initialize(double dm, double l, int m);

void proccess(double dm, double l, int m, int k, lista * event_list);

double exponential(double l);

double duration_of_call(double dm);


#endif // LAB2_H_
