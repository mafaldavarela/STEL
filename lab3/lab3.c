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
		event_list = adicionar( event_list, FIM_P , protecao_civil());
	}
	event_list = adicionar( event_list, INICIO_P, exponential( lambda , INICIO));

  imprimir(event_list);

  free(init_variables);

  return 0;
}

void proccess(lista *event_list, variables *init_variables) {

  servidor *server;
  server = (servidor *)malloc(sizeof (servidor));


  server->clients_handled = m+1; //limite da fila de espera NAO SEI QUANTO É
	server->occupied_channels = m; //canais ocupados após inicialização
	server->clock = 0; //manter controlo da clock do sistema
  lista  * waiting_list_P = NULL;
  lista  * waiting_list_I = NULL;

  int waiting_inem=0, waiting_pro=0, total_samples;
  double tempo;

  //As chamadas que encontram o sistema da Proteção Civil bloqueado são colocadas numa fila de espera
  //de comprimento finito, até ao limite da sua capacidade. Acima desse limite são perdidas.
  if((event_list->tipo ==INICIO_P) && (server->clients_handled <= waiting_pro) && (server->occupied_channels==init_variables->m) ){
    waiting_list_P=adicionar(waiting_list_P, WAITING , server -> clock);
    tempo=tempo+server->clock;
    total_samples++;
    printf("waiting time: %f\n",tempo/total_samples );
    waiting_pro++;
  }
  // a chamada da Proteção Civil só é libertada quando for atendida pelo INEM.
  else if ( (event_list->tipo==FIM_P) && (waiting_list_P!=NULL) && waiting_inem==0) {
    waiting_list_P = remover(waiting_list_P);
    event_list=adicionar(event_list, FIM_P, protecao_civil());
    waiting_pro--;
  }
  //Se o sistema estiver bloqueado, são colocadas numa fila de espera sem limite de capacidade.
  else if( (event_list->tipo==INICIO_I) && (server->occupied_channels==init_variables->m)){
    waiting_list_I=adicionar(waiting_list_I, WAITING, server->clock);
    waiting_inem++;
  }
  else if ( ( (event_list->tipo==FIM_I) || (event_list->tipo==FIM_P)) && waiting_list_I!=NULL){
    waiting_list_I=remover(waiting_list_I);
    event_list=adicionar(event_list, FIM_I, protecao_civil());
    waiting_inem--;
  }

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
