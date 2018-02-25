#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double get_poisson_value(double lambda);

int main(void){

  double lambda = 8; //taxa de chegada de clientes (1/s)
  double interval = 1 / ( 8*lambda );
  int i = 0, total_samples = 10000000, vetor[30] = {0};
  int vetor_size = sizeof(vetor)/sizeof(vetor[0]);
  double total=0;

  srand( time(NULL) );

  FILE *fptr;
  fptr = fopen("values.txt", "wb");


  for(i=0; i < total_samples ; i++){
    double value = get_poisson_value(lambda);

    total+=value;

    int pos = (int)(value/interval);

    if(pos < vetor_size)
      vetor[pos]++;
  }

  for(i=0; i < vetor_size; i++){
    fprintf(fptr, "%d\n", vetor[i]);
  }

  fclose(fptr);
  printf("All values stored!\n");
  printf("Avg. of interval between calls: %f\n", total/total_samples);
  return 0;
}

double get_poisson_value(double lambda){
  double u, c;

  //geração de uma distribuição exponencial x a partir de uma distribuição uniforme u
  u = ((double) rand() + 1) / ((double) RAND_MAX + 1);

  //normalização para a média de intervalos entre chamadas
  c = -log(u)/lambda;
  return c;
}
