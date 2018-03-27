#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include "lista.h"
#include "lab3.h"

int main( void ){

  srand( time(NULL) );

  variables  * init_variables = malloc( sizeof(variables));

  init_variables -> m = 8;
  init_variables -> L = 4;
  init_variables -> K = 20;

  double lambda = (double) (600) / (double) 3600;

  //initialize
  lista  * event_list = NULL;
  for(int i = 0 ; i < init_variables -> m; i++){
		event_list = adicionar( event_list, FIM , protecao_civil());
	}
	event_list = adicionar( event_list, INICIO, exponential( lambda , INICIO));

  imprimir(event_list);

  free(init_variables);

  return 0;
}

void proccess() {

}

double protecao_civil(){

  double dm = 0;
  int min = 0, max = 0;
  double time_to_inem = 0;
  double call_direction = ((double) rand() + 1) / ((double) RAND_MAX + 1);

  if( call_direction <= 0.4 ) {
    dm = 1.5*60;
    min = 60;
    max = 4*60;
  }
  else {
    dm = 45;
    min = 15;
    max = 75;

    double time_to_inem = exponential(dm , FIM);
    while( time_to_inem < min && time_to_inem > max)
      time_to_inem = exponential(dm , FIM);

    dm = 45;
    min = 15;
    max = INT_MAX;
  }

  double to_return = exponential(dm , FIM);

  while( to_return < min && to_return > max)
    to_return = exponential(dm , FIM);

  return to_return + time_to_inem;
}


double exponential(double aux , int type){
  double u, c;

  //geração de uma distribuição exponencial x a partir de uma distribuição uniforme u
  u = ((double) rand() + 1) / ((double) RAND_MAX + 1);

  c = 0;
  //normalização para a média de intervalos entre chamadas
  if( type == INICIO ) {
    c = -log(u)/aux;
  }
  else if ( type == FIM ){
    c = -log(u)*aux;
  }

  return c;
}
