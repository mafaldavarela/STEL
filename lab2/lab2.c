#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define OCUPADO 1
#define LIVRE 0

typedef struct{
  int canais_ocupados;
  int* canais;
}servidor;

double exponential(double l);
double duration_of_call(double dm);

int main(void){
  servidor* server;
  server = malloc(sizeof(server));
  double l=0.009*60*60; //taxa de chegada de clientes (1/hora)
  double dm=2*60; //media do tempo de serviço
  int m=8; //numero de canais
  int k=20000; //potenciais clientes
  server->canais = malloc(sizeof(int)*m);
  srand( time(NULL) );
  for(int i=0;i<m;i++){
     server->canais[i]=LIVRE;
  }
   for(int i=0;i<10;i++){
     printf("time to arrive- %f\n", exponential(l));
     printf("time of call- %f\n", duration_of_call(dm));

  }
  free(server); 	
}
double exponential(double l){
  double u, c;

  //geração de uma distribuição exponencial x a partir de uma distribuição uniforme u
  u = ((double) rand() + 1) / ((double) RAND_MAX + 1);

  //normalização para a média de intervalos entre chamadas
  c = -log(u)/l;

  return c;
}
double duration_of_call(double dm){
  double u, c;

  //geração de uma distribuição exponencial x a partir de uma distribuição uniforme u
  u = ((double) rand() + 1) / ((double) RAND_MAX + 1);

  //normalização para a média de intervalos entre chamadas
  c = -log(u)*dm;

  return c;
}
