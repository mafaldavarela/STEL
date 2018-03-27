#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "lista.h"
#include "lab2.h"


int main(int argc, char* argv[]){

  double dm=0; //media do tempo de serviço (segundos)
  int k=0; //potenciais clientes
  double l=0; //taxa de chegada de clientes (1/segundo)
  int total_samples = 1000000;
  double t=10; //delay
  int m=0; //numero de canais
  int max_buffer=0;


  if(strcmp(argv[1],"a")==0){
    printf("Computing a)...\n");
    dm=2*60;
    m=8;
    k=20000;
    l=0.009*k/(double)(60*60);
  }
  else if(strcmp(argv[1],"b")==0){
    printf("Computing b)...\n");
    max_buffer=1000;
    dm=2*60;
    m=8;
    k=20000;
    l=0.009*k/(double)(60*60);
  }
  else if(strcmp(argv[1],"c")==0){
    printf("Computing c)...\n");
    max_buffer=4;
    dm=2*60;
    m=8;
    k=20; //potenciais clientes
    l=(double)(12)/(double)(60*60); //taxa de chegada de clientes (1/segundo)
  }

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
	event_list = initialize(dm, l , m, k , argv[1]); //ocupação de recursos
	proccess(argv[1], dm, l, t, m, total_samples, max_buffer, k ,event_list);
	return 0;
}

lista* initialize(double dm, double l, int m, int k, char* arg){

	lista *init_list;
	init_list = NULL;
	for(int i=0; i<m; i++){
		//começam todos ao mesmo tempo
		init_list = adicionar(init_list, FIM, duration_of_call(dm));
	}
	if(strcmp("c",arg)==0)
		init_list = adicionar(init_list, INICIO, exponential((double)l*(k-m)));
	else
		init_list = adicionar(init_list, INICIO, exponential(l));
	return init_list;
}

void proccess(char* arg ,double dm, double l, double t, int m, int total_samples, int max_buffer, int k ,lista * event_list){

	servidor *server;
	server = (servidor *)malloc(sizeof (servidor));
	probabilidade *blocked;
	blocked = (probabilidade *)malloc(sizeof (probabilidade));
  blocked->samples=0;


  calc_average *client_delayed;
	client_delayed = (calc_average *)malloc(sizeof (calc_average));
  client_delayed->ammount=0;
  client_delayed->samples=0;

  probabilidade *less_than_t;
  less_than_t = (probabilidade *)malloc(sizeof (probabilidade));
  less_than_t->samples=0;

	server->clients_handled = m+1; //já lidamos com m+1 clientes no processo de inicialização
	server->occupied_channels = m; //canais ocupados após inicialização
	server->clock = 0; //manter controlo da clock do sistema
  server->waiting = 0;
  lista  * waiting_list = NULL;

  int histograma[40] = {0};
  int interval_nr = sizeof(histograma)/sizeof(histograma[0]);;
  double interval = 1 / (3*l);
  if(strcmp(arg,"c")==0)
	interval=1/(70*l);


	while(server->clients_handled <= total_samples || event_list!=NULL || (server->waiting) > 0){

    /*
    imprimir(event_list);
    getchar();
    */

		server -> clock = event_list -> tempo;

		if(event_list->tipo == FIM || (event_list==NULL && (server->waiting)>0)){
			(server->occupied_channels)--;
      if(((strcmp(arg,"b")==0 || strcmp(arg,"c")==0)) && (server->waiting)>0){
        (server->waiting)--;
        double aux = waiting_list -> tempo;
        waiting_list = remover(waiting_list);
        double wait_time = server->clock - aux;
        (client_delayed->ammount) += wait_time;
        if(wait_time < t){
          (less_than_t->samples)++;
        }
        int pos = (int)(wait_time/interval);
        if(pos < interval_nr)
          histograma[pos]++;
        (server->occupied_channels)++;
				event_list = adicionar(event_list, FIM, server -> clock + duration_of_call(dm));
      }
		}

		else if(event_list->tipo == INICIO){
			//recursos livres??

			//não há recursos livres
			if((server->occupied_channels) == m){
        //buffer=0; descartamos o cliente
        if(strcmp(arg,"a")==0){
          blocked->samples++;
          }
	else if((strcmp(arg,"b")==0 || strcmp(arg,"c")==0) && server->waiting >= max_buffer)
	  blocked->samples++;
        else if((strcmp(arg,"b")==0 || strcmp(arg,"c")==0) && server->waiting < max_buffer){
          (server->waiting)++;
          waiting_list = adicionar(waiting_list, WAITING , server -> clock);
          (client_delayed->samples)++;
        }
			}
			//há recursos livres
			else if((server->occupied_channels) < m){
				(server->occupied_channels)++;
				event_list = adicionar(event_list, FIM, server -> clock + duration_of_call(dm));
			}
      if(server->clients_handled <= total_samples){
        server->clients_handled++;
	if(strcmp("c",arg)==0){
		event_list = adicionar(event_list, INICIO, server -> clock + exponential((double)l*(k-m-server->waiting)));
  }
	else
		event_list = adicionar(event_list, INICIO, server -> clock + exponential((double)l));
      }
		}
		event_list = remover( event_list );
	}
	printf("FINISHED LOGGING\n");
	imprimir(event_list);
  if(strcmp(arg,"a")==0)
	 printf("Blocked user probability: %f\n", (double)(blocked->samples)/(double)total_samples);
  if(strcmp(arg,"b")==0 || strcmp(arg,"c")==0){
    //variáveis para histograma
    FILE *fptr;
    fptr = fopen("values.txt", "wb");
    for(int i=0; i< interval_nr; i++)
      fprintf(fptr, "%d\n", histograma[i]);
    FILE *f;
    f = fopen("settings.txt", "wb");
    //passar para o Matlab info sobre valores usados em simulação
    fprintf(f, " %f %d %f %f\n", interval, interval_nr ,client_delayed->samples, (double)(client_delayed->ammount)/(client_delayed->samples));
    fclose(f);
    printf("Blocked user probability: %f\n", (double)(blocked->samples)/(double)total_samples);
	double delay_prob = (double)(client_delayed->samples)/(double)total_samples;
    printf("Probability of delayed client: %f\n", delay_prob);

    printf("Average time of delayed service: %f\n", ((double)(client_delayed->ammount)/(client_delayed->samples))*delay_prob);
    printf("Probability of delay being less than %f: %f\n\n\n",t ,(double)(less_than_t->samples)/(client_delayed->samples));
  }
	free(client_delayed);
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
