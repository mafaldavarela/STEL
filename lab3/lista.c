#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

lista * remover (lista * apontador)
{
	lista * lap = (lista *)apontador -> proximo;
	free(apontador);
	return lap;
}

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

void imprimir (lista * apontador)
{
	if(apontador == NULL)
		printf("Lista vazia!\n");
	else
	{
		while(apontador != NULL)
		{
			if(apontador -> tipo == 0)
				printf("Tipo=INICIO_P\tTempo=%lf\n", apontador -> tempo);
			else if(apontador -> tipo == 1)
					printf("Tipo=INICIO_I\tTempo=%lf\n", apontador -> tempo);
			else if(apontador -> tipo == 2)
					printf("Tipo=FIM_P\tTempo=%lf\n", apontador -> tempo);
			else if(apontador -> tipo == 3)
					printf("Tipo=FIM_I\tTempo=%lf\n", apontador -> tempo);
			else if(apontador -> tipo == 4)
					printf("Tipo=INICIO_P_I\tTempo=%lf\n", apontador -> tempo);
			else if(apontador -> tipo == 5)
					printf("Tipo=FIM_P_I\tTempo=%lf\n", apontador -> tempo);
			else if(apontador -> tipo == 8)
					printf("Tipo=WAITING_P\tTempo=%lf\n", apontador -> tempo);
			else if(apontador -> tipo == 9)
					printf("Tipo=WAITING_P_I\tTempo=%lf\n", apontador -> tempo);
			else if(apontador -> tipo == 10)
					printf("Tipo=WAITING_I\tTempo=%lf\n", apontador -> tempo);
					
			apontador = (lista *)apontador -> proximo;
		}
	}
}
