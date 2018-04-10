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
  init_variables -> mi = 1;

  double lambda = (double) (600) / (double) 3600;

  //initialize
  lista  * protecao_event_list = NULL;
  for(int i = 0 ; i < init_variables -> m; i++){
		protecao_event_list = add_init_event(protecao_event_list, FIM, 0, lambda);
	}
	protecao_event_list = add_init_event(protecao_event_list, INICIO, 0, lambda);

  proccess(protecao_event_list, init_variables, lambda);

  free(init_variables);

  return 0;
}

void proccess(lista * protecao_event_list, variables * init_variables, double lambda) {

  servidor * server;
  server = (servidor *)malloc(sizeof (servidor));
  server -> protecao = (sistema *)malloc(sizeof (sistema));
  server -> inem = (sistema *)malloc(sizeof (sistema));

  server -> clients_handled = init_variables -> m + 1; //lidamos com m+1 clientes após inicialização
	server -> clock = 0; //manter controlo da clock do sistema

  server -> protecao -> event_list = protecao_event_list;
  server -> protecao -> waiting_list = NULL;
  server -> protecao -> occupied_channels = init_variables -> m; //canais ocupados após inicialização
  server -> protecao -> waiting_clients = 0; //clientes à espera na waiting list

  server -> inem -> event_list = NULL;
  server -> inem -> waiting_list = NULL;
  server -> inem -> occupied_channels = 0; //canais ocupados após inicialização
  server -> inem -> waiting_clients = 0;  //clientes à espera na waiting list

  int total_samples = 1000;

  while(server -> clients_handled < total_samples || server -> protecao -> waiting_clients > 0 || server -> protecao -> event_list != NULL){


    getchar();
    imprimir( server -> protecao -> event_list);
    printf("%d\n\n", server -> clients_handled);
    server -> clock = server -> protecao -> event_list -> tempo;
    //printf("CLOCK: %f\n", server -> clock );

    //As chamadas que encontram o sistema da Proteção Civil bloqueado são colocadas numa fila de espera
    //de comprimento finito, até ao limie da sua capacidade. Acima desse limite são perdidas.
    if( server -> protecao -> event_list -> tipo == INICIO_P ){
      //printf("Found a INICIO_P event!\n");
      if(server -> protecao -> occupied_channels == init_variables -> m && server -> protecao -> waiting_clients <  init_variables -> L){
        server -> protecao -> waiting_list = adicionar( server -> protecao-> waiting_list , WAITING_P , server -> clock);
        (server -> protecao -> waiting_clients)++;
        //printf("STORED IN WAITING LIST!\n");
      }
      else if(server -> protecao -> occupied_channels < init_variables -> m){
        (server -> protecao -> occupied_channels)++;
        server -> protecao -> event_list = add_end_event(server -> protecao -> event_list, INICIO_P, server -> clock);
        //printf("ADDED END EVENT!\n");
      }
      if(server -> clients_handled < total_samples){
        server -> protecao -> event_list = add_init_event( server -> protecao -> event_list , INICIO, server -> clock , lambda);
        server -> clients_handled ++;
      }
      //printf("ADDED INIT EVENT!\n");
    }

    else if( server -> protecao -> event_list -> tipo == INICIO_P_I ){
      //printf("Found a INICIO_P_I event!\n");
      if( server -> protecao -> occupied_channels == init_variables -> m && server -> protecao -> waiting_clients <  init_variables -> L){
        server -> protecao -> waiting_list = adicionar( server -> protecao -> waiting_list , WAITING_P_I , server -> clock);
        //printf("STORED IN WAITING LIST!\n");
        (server -> protecao -> waiting_clients)++;
      }
      else if(server -> protecao -> occupied_channels < init_variables -> m){
        //printf("ADDED END AND INIT EVENT!\n");
        (server -> protecao -> occupied_channels)++;
        server -> protecao -> event_list = add_end_event( server -> protecao -> event_list , INICIO_P, server -> clock);
      }
      if(server -> clients_handled < total_samples){
        server -> protecao -> event_list = add_init_event( server -> protecao -> event_list , INICIO, server -> clock, lambda);
        server -> clients_handled ++;
    }
      //printf("ADDED INIT EVENT!\n");
    }

    // a chamada da Proteção Civil só é libertada quando for atendida pelo INEM.
    else if ((server -> protecao -> event_list -> tipo == FIM_P) || (server -> protecao -> event_list -> tipo == FIM_P_I)) {
      //printf("Found a FIM event!\n");
      (server -> protecao -> occupied_channels)--;
      /*call goes to inem*/
      if ( server -> protecao -> event_list -> tipo == FIM_P_I && (server -> inem -> occupied_channels) < (init_variables -> mi)  ){
        server -> inem -> event_list = adicionar (server -> inem -> event_list, FIM_I, time_inem());
        (server -> inem -> occupied_channels)++;
      }
      else if(server -> protecao -> event_list -> tipo == FIM_P_I && (server -> inem -> occupied_channels) >= (init_variables -> mi)){
        lista * aux = server -> inem -> event_list;
        for (int i=0; i < server -> inem -> waiting_clients; i++) {
          aux = server-> inem -> event_list -> proximo;
        }
        server -> protecao -> event_list = adicionar(server-> protecao->event_list, WAITING_I, aux-> tempo);
        printf(" aux->tempo %f\n", aux ->tempo );
        server -> inem -> waiting_clients++;
      }
      if((server -> protecao -> waiting_clients > 0 ) && ( server -> protecao -> occupied_channels < init_variables -> m )){
        server -> protecao -> event_list = add_end_event(server -> protecao -> event_list, server -> protecao -> waiting_list -> tipo, server -> clock);
        server -> protecao -> waiting_list = remover( server -> protecao -> waiting_list );
        server -> protecao -> waiting_clients--;
        (server -> protecao -> occupied_channels)++;
      }
    /*else if (server -> inem -> event_list -> tipo == FIM_I){
      (server->inem -> occupied_channels)--;
      int a=0;

      if (server -> inem -> waiting_clients > 0){
        for (int i=0 ;  a=1 ; i++) {

          server -> protecao -> event_list = (lista *) server-> inem -> event_list -> proximo;

          if (server -> protecao -> event_list -> tipo == WAITING_I){

            server -> inem -> event_list = adicionar (server -> inem -> event_list, INICIO_I, time_inem());
            server -> inem -> waiting_clients--;
            (server->inem -> occupied_channels)++;
            a=1;
          }
        }
      }
    }*/
    }
      printf("waitinglist inem %d\n", server -> inem -> waiting_clients );
    imprimir(server -> protecao -> waiting_list);
    imprimir(server -> inem -> event_list);
    printf("\n\n");
    server -> protecao -> event_list = remover( server -> protecao -> event_list );
  }
  printf("END!\n");
  free(server -> inem);
  free(server -> protecao);
  free(server);
}


lista * add_init_event(lista * event_list, int mode, double current_time, double lambda){

  double dm = 0;
  int min = 0, max = 0, type = 0;

  //para encher precisamos de definir o evento
  double call_direction = ((double) rand() + 1) / ((double) RAND_MAX + 1);

  if( call_direction <= 0.4 ) {
    dm = 1.5*60;
    min = 60;
    max = 4*60;
    //só passa pela Proteção
    if(mode == FIM)
      type = FIM_P;
    else if(mode == INICIO)
      type = INICIO_P;
  }
  else if( call_direction > 0.4 && call_direction <= 1) {
    dm = 45;
    min = 30;
    max = 75;
    //depois da Proteção segue para o INEM
    if(mode == FIM)
      type = FIM_P_I;
    else if(mode == INICIO)
      type = INICIO_P_I;
  }
  else {
    printf("Error in function add_init_event()");
    exit(-1);
    return NULL;
  }
  if(mode == FIM){
    double tempo = exponential(dm , type);

    while( tempo < min && tempo > max)
      tempo = exponential(dm , type);

    return adicionar(event_list, type, tempo + current_time );
  }
  else
    return adicionar(event_list, type, exponential(lambda , type) + current_time);
}

lista * add_end_event(lista * event_list, int mode, double current_time){

  double dm = 0;
  int min = 0, max = 0, type = 0;
  int aux = 0;

  if( mode == INICIO_P || mode == WAITING_P ) {
    dm = 1.5*60;
    min = 60;
    max = 4*60;
    //só passa pela Proteção
    type = FIM_P;
  }
  else if( mode == INICIO_P_I || mode == WAITING_P_I) {
    dm = 45;
    min = 30;
    max = 75;
    //depois da Proteção segue para o INEM
    type = FIM_P_I;
  }
  else {
    printf("Error in function add_end_event()");
    exit(-1);
    return NULL;
  }

  double tempo = exponential(dm , type);

  while( tempo < min && tempo > max)
    tempo = exponential(dm , type);

  return adicionar(event_list, type, tempo + current_time);
}

double time_inem()
{
  double min=60;
  double dm=90;

  double tempo = exponential(dm , FIM_I);

  while( tempo < min)
    tempo = exponential(dm , FIM_I);

  return tempo;

}

double exponential(double aux , int type){
  double u, c;

  //geração de uma distribuição exponencial x a partir de uma distribuição uniforme u
  u = ((double) rand() + 1) / ((double) RAND_MAX + 1);

  c = 0;
  //normalização para a média de intervalos entre chamadas
  if( type == INICIO_P || type == INICIO_I || type == INICIO_P_I ) {
    c = -log(u)/aux;
  }
  else if ( type == FIM_P || type == FIM_I || type == FIM_P_I ){
    c = -log(u)*aux;
  }

  if( c < 0){
    printf("Error in function exponential()");
    exit(-1);
    return -1;
  }
  return c;
}
