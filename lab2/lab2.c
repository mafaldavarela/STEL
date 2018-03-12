#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "lista.h"
#include "lab2.h"


int main(void){

  double l=0.009/(60*60); //taxa de chegada de clientes (1/segundo)
  double dm=2*60; //media do tempo de serviço (segundos)
  int m=8; //numero de canais
  int k=20000; //potenciais clientes
  srand( time(NULL) );

	lista  * event_list = NULL;
	event_list = initialize(dm, l , m); //ocupação de recursos
	proccess(dm, l , m, k, event_list);
	return 0;
}

lista* initialize(double dm, double l, int m){

	lista *init_list;
	init_list = NULL;
	for(int i=0; i<m; i++){
		//começam todos ao mesmo tempo
		init_list = adicionar(init_list, FIM, duration_of_call(dm));
	}
	init_list = adicionar(init_list, INICIO, exponential(l));
	return init_list;
}

void proccess(double dm, double l, int m, int k, lista * event_list){

	servidor *server;
	server = (servidor *)malloc(sizeof (servidor));
	probabilidade *blocked;
	blocked = (probabilidade *)malloc(sizeof (probabilidade));
	probabilidade *client_delayed;
	client_delayed = (probabilidade *)malloc(sizeof (probabilidade));

	blocked->samples++;
	server->clients_handled = m+1; //já lidamos com m clientes no processo de inicialização
	server->occupied_channels = m; //canais ocupados após inicialização
	server->clock = 0; //manter controlo da clock do sistema

	while(server->clients_handled <= k || event_list!=NULL){

		server -> clock = event_list -> tempo;

		if(event_list->tipo == FIM){
			(server->occupied_channels)--;
		}

		else if(event_list->tipo == INICIO){
			//recursos livres??

			//não há recursos livres
			if((server->occupied_channels) == m){
				blocked->samples++;
				//buffer=0; descartamos o cliente
				if(server->clients_handled <= k){
					server->clients_handled++;
					event_list = adicionar(event_list, INICIO, server -> clock + exponential(l));
				}
			}
			//há recursos livres
			else if((server->occupied_channels) < m){
				(server->occupied_channels)++;
				event_list = adicionar(event_list, FIM, server -> clock + duration_of_call(dm));
				if(server->clients_handled <= k){
					server->clients_handled++;
					event_list = adicionar(event_list, INICIO, server -> clock + exponential(l));
				}
			}
		}
		event_list = remover( event_list );
	}
	printf("FINISHED LOGGING\n");
	imprimir(event_list);
	printf("Blocked user probability: %f\n", (double)(blocked->samples)/k);
	free(client_delayed);
	free(blocked);
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
