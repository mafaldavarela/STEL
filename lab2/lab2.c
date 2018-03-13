#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "lista.h"
#include "lab2.h"


int main(int argc, char* argv[]){

  double dm=2*60; //media do tempo de serviço (segundos)
  int m=8; //numero de canais
  int k=20000; //potenciais clientes
  double l=0.009/(60*60)*k; //taxa de chegada de clientes (1/segundo)
  int total_samples = 1000000;
  double t=120; //delay
  if(argc!=2){
    printf("Number of arguments invalid!\n");
    return 0;
  }
  if(strcmp(argv[1],"a")!=0 && strcmp(argv[1],"b")!=0 && strcmp(argv[1],"c")!=0){
    printf("Argument field invalid!\n");
    return 0;
  }
  srand( time(NULL) );

	lista  * event_list = NULL;
	event_list = initialize(dm, l , m); //ocupação de recursos
	proccess(argv[1], dm, l, t, m, total_samples, event_list);
	return 0;
}

lista* initialize(double dm, double l, int m){

	lista *init_list;
	init_list = NULL;
	for(int i=0; i<m; i++){
		//começam todos ao mesmo tempo
		init_list = adicionar(init_list, FIM, duration_of_call(dm));
    //init_list = adicionar(init_list, INICIO, exponential(l));
	}
	init_list = adicionar(init_list, INICIO, exponential(l));
	return init_list;
}

void proccess(char* arg ,double dm, double l, double t, int m, int total_samples, lista * event_list){

	servidor *server;
	server = (servidor *)malloc(sizeof (servidor));
	probabilidade *blocked;
	blocked = (probabilidade *)malloc(sizeof (probabilidade));
  blocked->samples=0;

	probabilidade *client_delayed;
	client_delayed = (probabilidade *)malloc(sizeof (probabilidade));
  client_delayed->samples=0;

  calc_average *service_delayed;
	service_delayed = (calc_average *)malloc(sizeof (calc_average));
  service_delayed->ammount=0;
  service_delayed->samples=0;

  probabilidade *less_than_t;
  less_than_t = (probabilidade *)malloc(sizeof (probabilidade));
  less_than_t->samples=0;

  int total_waited;
	server->clients_handled = m+1; //já lidamos com m+1 clientes no processo de inicialização
	server->occupied_channels = m; //canais ocupados após inicialização
	server->clock = 0; //manter controlo da clock do sistema
  server->waiting = 0;
	while(server->clients_handled <= total_samples || event_list!=NULL){

    /*
    imprimir(event_list);
    getchar();
    */
		server -> clock = event_list -> tempo;

		if(event_list->tipo == FIM){
			(server->occupied_channels)--;
      if(strcmp(arg,"b")==0 && (server->waiting)>0){
        (server->waiting)--;
        (server->occupied_channels)++;
        double aux = duration_of_call(dm);
        if( aux > dm){
          if((aux-dm) < t)
            less_than_t->samples++;
          (service_delayed -> ammount)+= aux - dm;
          (service_delayed -> samples)++;
        }
				event_list = adicionar(event_list, FIM, server -> clock + aux);
      }
		}

		else if(event_list->tipo == INICIO){
			//recursos livres??

			//não há recursos livres
			if((server->occupied_channels) == m){
        if(strcmp(arg,"a")==0)
				    blocked->samples++;
				//buffer=0; descartamos o cliente
        else if(strcmp(arg,"b")==0)
          (server->waiting)++;
          total_waited++;
			}
			//há recursos livres
			else if((server->occupied_channels) < m){
				(server->occupied_channels)++;
        double aux = duration_of_call(dm);
        if( aux > dm){
          if((aux-dm) < t)
            less_than_t->samples++;
          (service_delayed -> ammount)+= aux - dm;
          (service_delayed -> samples)++;
        }
				event_list = adicionar(event_list, FIM, server -> clock + aux);
			}
      if(server->clients_handled <= total_samples){
        server->clients_handled++;
        double aux = exponential(l);
        if( aux > 1/l)
          (client_delayed -> samples)++;
        event_list = adicionar(event_list, INICIO, server -> clock + aux);
      }
		}
		event_list = remover( event_list );
	}
	printf("FINISHED LOGGING\n");
	imprimir(event_list);
	printf("Blocked user probability: %f\n", (double)(blocked->samples)/(double)total_samples);
  if(strcmp(arg,"b")==0){
    printf("Probability of delayed client: %f\n", (double)(client_delayed->samples)/(double)total_samples);
    printf("Average of delayed service: %f\n", (double)(service_delayed->ammount)/(service_delayed->samples));
    printf("Probability of delayed client time less than %f: %f\n", t,(double)(less_than_t->samples)/(double)total_samples);
    printf("Total waited: %d\n", total_waited);
  }
	free(client_delayed);
  free(service_delayed);
	free(blocked);
  free(less_than_t);
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
