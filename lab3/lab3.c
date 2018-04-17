#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lista.h"
#include "lab3.h"


int main(void) {

  srand(time(NULL));

  variables *init_variables = malloc(sizeof(variables));

  init_variables->m = 1;
  init_variables->L = 1;
  init_variables->k = 20000; //infinite
  init_variables->mi = 1;

  double lambda = (double)(600) / (double)3600;

  // initialize
  int found = 0;
  resultados * results;
  FILE *f;
  f = fopen("results.txt", "a+");
  printf("Trying with {m = %d ; L = %d ; mi = %d}\n", init_variables->m , init_variables->L , init_variables->mi);
  while(!found){
    double epsilon = 0.02;
    double epsilon1 = 0.01;
    double epsilon2 = 2;
    double epsilon3 = 2;
    lista * protecao_event_list = NULL;
    int counter = 0;
    for (int i = 0 ; i < init_variables -> m ; i++ ) {
      protecao_event_list = add_init_event( protecao_event_list , FIM , 0 , lambda );
    }
    protecao_event_list = add_init_event( protecao_event_list , INICIO, 0 , (double)lambda);
    results = proccess(protecao_event_list, init_variables, lambda);
    if((fabs(results -> delay_prob - 0.2) <= epsilon) && (fabs(results -> lost_prob - 0.01) <= epsilon1) && (fabs(results -> avg_delay - 30) <= epsilon2) && (fabs(results -> total_delay - 60)) <= epsilon3){
      printf("\n\nFOUND!!!\n");
      printf("With {m = %d ; L = %d ; mi = %d}\n", init_variables->m , init_variables->L , init_variables->mi);
      printf("Delay calls prob. at prot.: %f\n", results -> delay_prob);
      printf("Lost calls prob: %f\n", results -> lost_prob);
      printf("Avg. delay Prot: %f\n", results -> avg_delay);
      printf("Avg. delay Total: %f\n", results -> total_delay);
      fprintf(f, "\n\nFOUND!!!\n");
      fprintf(f,"With {m = %d ; L = %d ; mi = %d}\n", init_variables->m , init_variables->L , init_variables->mi);
      fprintf(f,"Delay calls prob. at prot.: %f\n", results -> delay_prob);
      fprintf(f,"Lost calls prob: %f\n", results -> lost_prob);
      fprintf(f,"Avg. delay Prot: %f\n", results -> avg_delay);
      fprintf(f,"Avg. delay Total: %f\n", results -> total_delay);
      fprintf(f,"------------------------\n\n");
      //found = 1;
    }
    if((fabs(results -> delay_prob - 0.2) <= epsilon)) counter++;
    if(fabs(results -> lost_prob - 0.01) <= epsilon1) counter++;
    if(fabs(results -> avg_delay - 30) <= epsilon2) counter++;
    if(fabs(results -> total_delay - 60) <= epsilon3) counter++;
    if(counter>=3){
      printf("With {m = %d ; L = %d ; mi = %d}\n", init_variables->m , init_variables->L , init_variables->mi);
      printf("Delay calls prob. at prot.: %f\n", results -> delay_prob);
      printf("Lost calls prob: %f\n", results -> lost_prob);
      printf("Avg. delay Prot: %f\n", results -> avg_delay);
      printf("Avg. delay Total: %f\n", results -> total_delay);
      fprintf(f,"With {m = %d ; L = %d ; mi = %d}\n", init_variables->m , init_variables->L , init_variables->mi);
      fprintf(f,"Delay calls prob. at prot.: %f\n", results -> delay_prob);
      fprintf(f,"Lost calls prob: %f\n", results -> lost_prob);
      fprintf(f,"Avg. delay Prot: %f\n", results -> avg_delay);
      fprintf(f,"Avg. delay Total: %f\n", results -> total_delay);
      fprintf(f,"------------------------\n\n");

    }
      if(init_variables -> mi < 40){
        init_variables -> mi++;
      }
      else{
        printf("Ended at {m = %d ; L = %d ; mi = %d}\n", init_variables->m , init_variables->L , init_variables->mi);
        init_variables -> L++;
        init_variables -> mi = 1;
        if(init_variables -> L > 40){
          init_variables -> m ++;
          init_variables -> L = 1;
          init_variables -> mi = 1;
        }
        printf("Trying with {m = %d ; L = %d ; mi = %d}\n", init_variables->m , init_variables->L , init_variables->mi);
      }

    if(init_variables -> m > 30)
      break;
    free(results);
  }
  fclose(f);
  free(init_variables);
  return 0;
}

resultados * proccess(lista * protecao_event_list, variables * init_variables, double lambda) {

  servidor * server = (servidor *)malloc(sizeof(servidor));
  server -> protecao = (sistema *)malloc(sizeof(sistema));
  server -> inem = (sistema *)malloc(sizeof(sistema));

  server -> clients_handled = init_variables -> m + 1; // lidamos com m+1 clientes após inicialização
  server -> clock = 0;         // manter controlo da clock do sistema

  server -> protecao -> event_list = protecao_event_list;
  server -> protecao -> waiting_list = NULL;
  server -> protecao -> occupied_channels = init_variables -> m; // canais ocupados após inicialização
  server -> protecao -> waiting_clients = 0; // clientes à espera na waiting list

  server -> inem -> event_list = NULL;
  server -> inem -> waiting_list = NULL;
  server -> inem -> occupied_channels = 0; // canais ocupados após inicialização
  server -> inem -> waiting_clients = 0;   // clientes à espera na waiting list


  probability * lost_calls = (probability *)malloc(sizeof(probability));
  lost_calls -> samples = 0;

  probability * average_delay = (probability *)malloc(sizeof(probability));
  average_delay -> samples = 0;
  average_delay -> ammount = 0;

  probability * average_delay_total = (probability *)malloc(sizeof(probability));
  average_delay_total -> samples = 0;
  average_delay_total -> ammount = 0;

  probability * average_absolute_error = (probability *)malloc(sizeof(probability));
  average_absolute_error -> samples = 0;
  average_absolute_error -> ammount = 0;

  int vetor[40] = {0};
  double interval = 0.1;
  int vetor_size = sizeof(vetor)/sizeof(vetor[0]);
  int total_samples = 1000000;
  while (server -> clients_handled < total_samples || server -> protecao -> waiting_clients > 0 || server-> protecao -> event_list != NULL || server -> inem -> event_list != NULL ){

    if(server -> inem -> waiting_list == 0 && server -> protecao -> occupied_channels == 0 && server -> clients_handled >= total_samples){
      while( server -> inem -> occupied_channels > 0){
        server-> inem-> event_list = remover(server -> inem -> event_list);
        server -> inem -> occupied_channels--;
      }
      break;
    }
    /*getchar();
    printf("EVENT LIST PROTECAO\n");
    imprimir(server -> protecao -> event_list);
    printf("WAITING: %d\n", server -> protecao -> waiting_clients);
    printf(">>>+1\n");*/
    // As chamadas que encontram o sistema da Proteção Civil bloqueado são
    // colocadas numa fila de espera de comprimento finito, até ao limite da sua
    // capacidade. Acima desse limite são perdidas.
    if (server -> inem -> event_list == NULL || (server -> protecao != NULL && server -> protecao -> event_list -> tempo < server -> inem -> event_list -> tempo)) {
      server -> clock = server -> protecao -> event_list -> tempo;
      if (server -> protecao -> event_list -> tipo == INICIO_P) {
      //  printf("Found a INICIO_P event!\n");
        if (server -> protecao -> occupied_channels == init_variables -> m && server -> protecao -> waiting_clients < init_variables -> L) {
          server -> protecao -> waiting_list = adicionar(server -> protecao -> waiting_list, WAITING_P, server -> clock, 0, server->clock);
          (server -> protecao -> waiting_clients)++;
        //  printf("STORED IN WAITING LIST!\n");
        } else if ( server -> protecao -> occupied_channels < init_variables -> m) {
          (server -> protecao -> occupied_channels)++;
          server -> protecao -> event_list = add_end_event(server -> protecao -> event_list, INICIO_P, server -> clock);
        //  printf("ADDED END EVENT!\n");
        } else if(server -> protecao -> occupied_channels == init_variables -> m && server -> protecao -> waiting_clients == init_variables -> L){
          (lost_calls -> samples) ++;
        //  printf("EVENT LOST!\n");
        }
        if (server -> clients_handled < total_samples) {
          server -> protecao -> event_list = add_init_event(server -> protecao -> event_list, INICIO, server -> clock, (double)lambda);
          server -> clients_handled++;
        //  printf("ADDED INIT EVENT!\n");
        }

      }

      else if (server -> protecao -> event_list -> tipo == INICIO_P_I) {
        //printf("Found a INICIO_P_I event!\n");
        if (server -> protecao -> occupied_channels == init_variables -> m && server -> protecao -> waiting_clients < init_variables -> L) {
          server -> protecao -> waiting_list = adicionar(server -> protecao -> waiting_list, WAITING_P_I, server -> clock, 0, server->clock);
        //  printf("STORED IN WAITING LIST!\n");
          (server -> protecao -> waiting_clients)++;
        } else if (server -> protecao -> occupied_channels < init_variables -> m) {
        //  printf("ADDED END EVENT!\n");
          (server -> protecao -> occupied_channels)++;
          server -> protecao -> event_list = add_end_event(server -> protecao -> event_list, INICIO_P, server -> clock);
        } else if(server -> protecao -> occupied_channels == init_variables -> m && server -> protecao -> waiting_clients == init_variables -> L){
          (lost_calls -> samples) ++;
        //  printf("EVENT LOST!\n");
        }
        if (server -> clients_handled < total_samples) {
          server -> protecao -> event_list = add_init_event(server -> protecao -> event_list, INICIO, server -> clock, (double)lambda);
          server -> clients_handled++;
        //  printf("ADDED INIT EVENT!\n");
        }
      }

      // a chamada da Proteção Civil só é libertada quando for atendida pelo
      // INEM.
      else if ((server -> protecao -> event_list -> tipo == FIM_P) || (server -> protecao -> event_list -> tipo == FIM_P_I)) {
        //printf("Found a FIM event!\n");
        (server -> protecao -> occupied_channels)--;
        /*call goes to inem*/
        if (server -> protecao -> event_list -> tipo == FIM_P_I && (server -> inem -> occupied_channels) < (init_variables -> mi)) {
          server -> inem -> event_list = adicionar(server -> inem -> event_list, FIM_I, server -> clock + time_inem(), 0, server->clock);
          (server -> inem -> occupied_channels)++;
          //printf("ADDED EVENT TO INEM!\n");
        } else if (server -> protecao -> event_list -> tipo == FIM_P_I && (server -> inem -> occupied_channels) >= (init_variables -> mi)) {
          //HERE
            if(average_absolute_error -> samples == 0){
              server -> inem -> waiting_list = adicionar(server -> inem -> waiting_list, WAITING_I, server -> clock, 0, server-> protecao -> event_list -> inicio);
              server -> protecao -> event_list = adicionar(server -> protecao -> event_list, WAITING_I, server -> inem -> event_list -> tempo, 0, server->clock);
            }
            else{
              double prediction = (double) (average_absolute_error -> ammount) /  (double)(average_absolute_error -> samples);
              server -> inem -> waiting_list = adicionar(server -> inem -> waiting_list, WAITING_I, server -> clock, prediction, server-> protecao -> event_list -> inicio);
              server -> protecao -> event_list = adicionar(server -> protecao -> event_list, WAITING_I, server -> inem -> event_list -> tempo, 0, server->clock);
            }
          (server -> protecao -> occupied_channels)++;
          server -> inem -> waiting_clients++;
        //  printf("INEM EVENT WAITING IN PROTECAO!\n");
        }
        if ((server -> protecao -> waiting_clients > 0) && (server -> protecao -> occupied_channels < init_variables -> m)) {
          server -> protecao -> event_list = add_end_event(server -> protecao -> event_list,server -> protecao -> waiting_list -> tipo, server -> clock);
          average_delay -> samples++;
          average_delay -> ammount += (server -> clock - server -> protecao -> waiting_list -> tempo);
          server -> protecao -> waiting_list = remover(server -> protecao -> waiting_list);
          server -> protecao -> waiting_clients--;
          (server -> protecao -> occupied_channels)++;
        }
      }
      if(server -> protecao -> event_list != NULL )
        server -> protecao -> event_list = remover(server -> protecao -> event_list);
    }
  else if(server -> inem -> event_list != NULL){
  //  printf("Found a FIM inem event!\n");
    server -> clock = server -> inem -> event_list -> tempo;
    server -> inem -> event_list = remover(server -> inem -> event_list);
    (server -> inem -> occupied_channels)--;
    if (server -> protecao -> event_list -> tipo == WAITING_I) {
    //  printf("INSERTED TO INEM FROM WAITING!\n");
      server -> inem -> event_list = adicionar(server -> inem -> event_list, FIM_I, server -> clock + time_inem(), 0, server->clock);
      (server -> inem -> occupied_channels)++;
      server -> protecao -> event_list = remover(server -> protecao -> event_list);
      (server -> protecao -> occupied_channels)--;
      if( server -> inem -> waiting_clients > 0 ){
        average_delay_total -> samples ++;
        average_delay_total -> ammount += (server -> clock - server -> inem -> waiting_list -> inicio);
        if(average_absolute_error -> samples == 0){
          average_absolute_error -> samples++;
          average_absolute_error -> ammount = (server -> clock - server -> inem -> waiting_list -> tempo);
          vetor[20]++;
        }
        else {
          average_absolute_error -> samples++;
          average_absolute_error -> ammount += (double)(server -> clock - server -> inem -> waiting_list -> tempo - server -> inem -> waiting_list -> previsao);
          double value = (server -> clock - server -> inem -> waiting_list -> tempo - server -> inem -> waiting_list -> previsao)/(server -> inem -> waiting_list -> previsao);
          double pos = (value/interval);
          printf("From x=%f x'=%f\n",server -> clock - server -> inem -> waiting_list -> tempo,server -> inem -> waiting_list -> previsao );
          printf("Got pos %f\n",pos );
          if(pos <= 20 && pos >=-20)
            vetor[(int)(pos+20)]++;
        }
        if (server -> protecao -> event_list != NULL) {
          while (server -> protecao -> event_list -> tipo == WAITING_I && server -> protecao -> event_list -> tempo == server -> clock) {
            server -> protecao -> event_list = adicionar(server -> protecao -> event_list, WAITING_I, server -> inem -> event_list -> tempo, 0, server->clock);
            server -> protecao -> event_list = remover(server -> protecao -> event_list);
          }
        }
        server -> inem -> waiting_clients--;
        server -> inem -> waiting_list = remover(server -> inem -> waiting_list);;
      }
      if (server -> protecao -> waiting_clients > 0) {
      //  printf("INSERTED TO PROTECAO FROM WAITING!\n");
        server -> protecao -> event_list = add_end_event(server -> protecao -> event_list, server -> protecao -> waiting_list -> tipo, server -> clock);
        server -> protecao -> waiting_list = remover(server -> protecao -> waiting_list);
        server -> protecao -> waiting_clients--;
        (server -> protecao -> occupied_channels)++;
      }
    }
  }
  /*printf("CLOCK: %f\n", server -> clock );
  printf("WAITING LIST PROTECAO\n");
  imprimir(server -> protecao -> waiting_list);
  printf("---------\n");
  printf("EVENT LIST INEM\n");
  imprimir(server -> inem -> event_list);
  printf("WAITING LIST INEM\n");
  imprimir(server -> inem -> waiting_list);
  printf("\n\n");*/
}
resultados * results = (resultados *)malloc(sizeof(resultados));
results -> delay_prob = (double) (average_delay -> samples) /  (double)(total_samples);
results -> lost_prob = (double) (lost_calls -> samples) /  (double)(total_samples);
results -> avg_delay = (double) (average_delay -> ammount) /  (double)(average_delay -> samples);
results -> total_delay = (double) (average_delay_total -> ammount) /  (double)(average_delay_total -> samples);

FILE *fptr;
fptr = fopen("histogram.txt", "wb");
for(int i=0; i < vetor_size; i++){
    fprintf(fptr, "%d\n", vetor[i]);
  }

if(average_delay_total -> samples == 0)
  results -> total_delay = 0;
//printf("Avg. error: %f\n", (double) (average_absolute_error -> ammount) /  (double)(average_absolute_error -> samples));
free(server -> inem);
free(server -> protecao);
free(server);
free(lost_calls);
free(average_delay);
free(average_delay_total);
free(average_absolute_error);
return results;
}

lista *add_init_event(lista *event_list, int mode, double current_time, double lambda) {

  double dm = 0, sigma, min = 0, max = 0;
  int type = 0;

  // para encher precisamos de definir o evento
  double call_direction = ((double)rand() + 1) / ((double)RAND_MAX + 1);

  if (call_direction <= 0.4) {
    dm = 1.5 * 60;
    min = 60;
    max = 4 * 60;
    // só passa pela Proteção
    if (mode == FIM)
      type = FIM_P;
    else if (mode == INICIO)
      type = INICIO_P;
  } else if (call_direction > 0.4 && call_direction <= 1) {
    dm = 45;
    sigma=15;
    min = 30;
    max = 75;
    // depois da Proteção segue para o INEM
    if (mode == FIM){
        type = FIM_P_I;
        return adicionar(event_list, type, box_muller(sigma, dm, max, min)+current_time, 0, current_time);
    }
    else if (mode == INICIO)
      type = INICIO_P_I;
  } else {
    printf("Error in function add_init_event()");
    exit(-1);
    return NULL;
  }
  if (mode == FIM) {
    double tempo = exponential(dm, type);

    while (tempo < min && tempo > max)
      tempo = exponential(dm, type);

    return adicionar(event_list, type, tempo + current_time, 0, current_time);
  }
  else
    return adicionar(event_list, type, exponential(lambda, type) + current_time, 0, current_time);
}

lista *add_end_event(lista *event_list, int mode, double current_time) {

  double dm = 0, sigma=0, min = 0, max = 0;
  int type = 0;

  if (mode == INICIO_P || mode == WAITING_P) {
    dm = 1.5 * 60;
    min = 60;
    max = 4 * 60;
    // só passa pela Proteção
    type = FIM_P;

    double tempo = exponential(dm, type);

    while (tempo < min && tempo > max)
      tempo = exponential(dm, type);

    return adicionar(event_list, type, tempo + current_time, 0, current_time);
    }

  else if (mode == INICIO_P_I || mode == WAITING_P_I) {
    dm = 45;
    sigma=15;
    min = 30;
    max = 75;
    // depois da Proteção segue para o INEM
    type = FIM_P_I;
    return adicionar(event_list, type, box_muller(sigma, dm, max, min)+current_time, 0, current_time);
  }

  else {
    printf("Error in function add_end_event()");
    exit(-1);
    return NULL;
  }

}

double time_inem() {
  double min = 60;
  double dm = 90;

  double tempo = exponential(dm, FIM_I);

  while (tempo < min)
    tempo = exponential(dm, FIM_I);

  return tempo;
}

//returns a value that respects a gaussian distribution
double box_muller (double sigma, double mu, double max, double min){
  double u=0, v=0, s=0, z0, tempo;
  static double z1;
  static int turn=0;
  if (turn==1){
    turn=0;
     z0=z1;
   }
  else{
    do{

      u =-1+2*((double)rand())/(double)RAND_MAX;
      v =-1+2*((double)rand())/(double)RAND_MAX;
      s=u*u+v*v;
    } while(s>=1);
    z0=u*sqrt((double)(-2*(double)(log(s))/s));
    z1=v*sqrt((double)(-2*(double)(log(s))/s));

    turn=1;

  }

  tempo = (double)(z0 * sigma + mu);

    if (tempo < min && tempo > max)
        return box_muller(sigma, mu, max, min);
    //printf(" %f\n", tempo);
  return tempo;
}
double exponential(double aux, int type) {
  double u, c;

  // geração de uma distribuição exponencial x a partir de uma distribuição
  // uniforme u
  u = ((double)rand() + 1) / ((double)RAND_MAX + 1);

  c = 0;
  // normalização para a média de intervalos entre chamadas
  if (type == INICIO_P || type == INICIO_I || type == INICIO_P_I) {
    c = -log(u) / aux;
  } else if (type == FIM_P || type == FIM_I || type == FIM_P_I) {
    c = -log(u) * aux;
  }

  if (c < 0) {
    printf("Error in function exponential()");
    exit(-1);
    return -1;
  }
  return c;
}
