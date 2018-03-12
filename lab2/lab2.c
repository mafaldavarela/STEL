#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define OCUPADO 1
#define LIVRE 0
typedef struct{
	int tipo;
	double tempo;
	struct lista * proximo;
} lista;


// Função que remove o primeiro elemento da lista
lista * remover (lista * apontador)
{
	lista * lap = (lista *)apontador -> proximo;
	free(apontador);
	return lap;
}

// Função que adiciona novo elemento à lista, ordenando a mesma por tempo
lista * adicionar (lista * apontador, int n_tipo, double n_tempo)
{
	lista * lap = apontador;
	lista * ap_aux, * ap_next;
	if(apontador == NULL)
	{
		apontador = (lista *) malloc(sizeof (lista));
		apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> tempo = n_tempo;
		return apontador;
	}
	else
	{
		if (apontador->tempo > n_tempo) {
	        ap_aux = (lista *) malloc(sizeof (lista));
	        ap_aux -> tipo = n_tipo;
            ap_aux -> tempo = n_tempo;
            ap_aux -> proximo = (struct lista *) apontador;
            return ap_aux;
	    }

		ap_next = (lista *)apontador -> proximo;
		while(apontador != NULL)
		{
			if((ap_next == NULL) || ((ap_next -> tempo) > n_tempo))
				break;
			apontador = (lista *)apontador -> proximo;
			ap_next = (lista *)apontador -> proximo;
		}
		ap_aux = (lista *)apontador -> proximo;
		apontador -> proximo = (struct lista *) malloc(sizeof (lista));
		apontador = (lista *)apontador -> proximo;
		if(ap_aux != NULL)
			apontador -> proximo = (struct lista *)ap_aux;
		else
			apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> tempo = n_tempo;
		return lap;
	}
}

// Função que imprime no ecra todos os elementos da lista
void imprimir (lista * apontador)
{
	if(apontador == NULL)
		printf("Lista vazia!\n");
	else
	{
		while(apontador != NULL)
		{
			printf("Tipo=%d\tTempo=%lf\n", apontador -> tipo, apontador -> tempo);
			apontador = (lista *)apontador -> proximo;
		}
	}
}
double exponential(double l);
double duration_of_call(double dm);

int main(void){

  lista  * lista_eventos;
	lista_eventos = NULL;

  double l=0.009*60*60; //taxa de chegada de clientes (1/hora)
  double dm=2*60; //media do tempo de serviço
  int m=8; //numero de canais
  int k=20000; //potenciais clientes
  double ltime=0, time_atual, c;
  int ltipo;
  srand( time(NULL) );
	lista_eventos = adicionar(lista_eventos, LIVRE, exponential(l));
  int m_ocupados=1; //canais ocupados
  int clientes=1;//numero de clientes
  while(m_ocupados<m){ // inicialização
      m_ocupados ++;
      clientes++;
      lista_eventos = adicionar(lista_eventos, m_ocupados, duration_of_call(dm));
  }
  while(clientes <k){

            ltime = lista_eventos -> tempo;
            ltipo = lista_eventos -> tipo;
            time_atual = ltime;
            lista_eventos = remover(lista_eventos);

            if(ltipo == LIVRE){ //Se a chamada é do tipo LIVRE
                lista_eventos = adicionar(lista_eventos, LIVRE, time_atual+exponential(l));
                clientes++;

		 }


        }
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
