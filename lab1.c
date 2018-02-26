#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

double get_poisson_proccess(double lambda);
int get_interval_value(double prob, int pos);

int main(void){

  double lambda = 8; //taxa de chegada de clientes (1/s)
  double interval = 1 / ( 8*lambda ); //intervalo usado em histograma
  int i = 0, total_samples = 1000000, vetor[30] = {0};
  int vetor_size = sizeof(vetor)/sizeof(vetor[0]);
  double total=0;

  char line;
  line = getchar();
  printf("Computing line %c)\n", line);

  srand( time(NULL) );

  printf("Storing values...\n");

  FILE *fptr;
  fptr = fopen("values.txt", "wb");

  if(line == 'a'){
    for(i=0; i < total_samples ; i++){
      double value = get_poisson_proccess(lambda);

      total+=value;

      int pos = (int)(value/interval);

      if( pos < vetor_size )
        vetor[pos]++;
    }
  }
  else if( line == 'b' ){
    double prob = lambda*interval;
    for(i = 0 ; i < total_samples ; i++){

      int value = get_interval_value( prob , 0 );

      if( value < vetor_size )
        vetor[value]++;
    }
  }

  for(i=0; i < vetor_size; i++){
    fprintf(fptr, "%d\n", vetor[i]);
  }

  fclose(fptr);
  printf("All values stored!\n");
  printf("Storing settings...\n");

  FILE *f;
  f = fopen("settings.txt", "wb");
  //passar para o Matlab info sobre valores usados em simulação
  fprintf(f, "%f %f %d %d %f\n", lambda, interval, vetor_size ,total_samples, total/total_samples);
  fclose(f);
  printf("Settings stored!\n");
  return 0;
}

double get_poisson_proccess(double lambda){
  double u, c;

  //geração de uma distribuição exponencial x a partir de uma distribuição uniforme u
  u = ((double) rand() + 1) / ((double) RAND_MAX + 1);

  //normalização para a média de intervalos entre chamadas
  c = -log(u)/lambda;

  return c;
}

int get_interval_value(double prob, int pos){

    //generate random number between 0 and 1
    double occurrence = ((double) rand() + 1) / ((double) RAND_MAX + 1);

    //if occurrence is smaller than probability of ocurrence, then the event happened in this interval position
    if(occurrence <= prob){
      return pos;
    }
    //if not, try the next interval recursively
    else
      return get_interval_value(prob, pos+1);
}
