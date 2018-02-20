#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

// Definição da estrutura da lista
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
//novo código

//HISTOGRAMA
void histograma(lista * apontador, int tamanho, float lambda)
{
			int i, vect[tamanho]={0}, cont;
			float delta;
			delta=1/(8*lambda);
			for(i=0;i<tamanho; i++)
				{
						x=apontador->tempo;
						if(x>(delta*cont) && x<(delta*(cont+1))
								vect[cont]++;	
						else 
						{
								cont++;
								if(x>(delta*cont) && x<(delta*(cont+1))
									vect[cont]++;	
		
						}				
					apontador = (lista *)apontador -> proximo;
						
				}
//IMPRIMIR O VETOR EM HISTOGRAMA
			
	
}

int main(void)
{
	lista *lista_eventos;
	int tipo_ev; double tempo_ev;
	lista_eventos = NULL;
	int  i=0, j, tamanho, tipochamada=0;
	double  tempochamada, u, c;
	float lambda=1.2;
	
	srand(time(NULL));

	//TAMANHO DA LISTA
	tamanho = 1 + ( rand() % 100 );
	
	double vect[tamanho];
	vect[0]=0;

		lista_eventos=adicionar(lista_eventos,0,c);

	//GERAR CADA ELEMENTO TIPO E TEMPO
	for(i=1; i<=tamanho; i++)
		{
			u=((double)rand()+1)/((double)RAND_MAX+1);
			
			//TIPO DA CHAMADA
			tipochamada=rand() % 3;

			//taxa de chegada	
			c=(- (double)log(u) / lambda);
			printf("%f\n", c);

			vect[i]=c;
			printf(" vect[%d]=%f\n", i, vect[i]);
			lista_eventos=adicionar(lista_eventos, tipochamada, c+vect[i-1]);

		}

	imprimir(lista_eventos);
	printf("--------------------------------------------------------------------------------");

	//ELIMINAR O PRIMEIRO EVENTO
	lista_eventos = remover(lista_eventos);
	imprimir(lista_eventos);
	printf("--------------------------------------------------------------------------------");
	

	printf("tamanho-%d\n\n", tamanho);

	histograma(lista_eventos, tamanho, lambda);
}