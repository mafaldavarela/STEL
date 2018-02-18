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
double poisson(double lambda, int k)
{
	double fat=1, func; 
	for(int fat=1; k>1; k--)
			fat=fat*(double)k;
	func=((exp(-lambda)*(pow(lambda, (double)k))))/fat;
	return func;
}

//HISTOGRAMA
void histograma(double n[], int tamanho)
{

//int n[tamanho] ;
/*for(int cont=0; cont<tamanho; cont++)
{
	n[cont]=apontador -> tempo;
	apontador = (lista *)apontador -> proximo;
}*/

	int i, j;
 
	printf("%s%13s%17s\n","Element/index", "Value", "Histogram");

	for(i=0; i <= (tamanho-1); i++)

	{

	printf("%9d%15d ", i, n[i]);

	for(j = 1; j<= n[i]; j++)
			printf("*");
	printf("\n");
	}
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
	

		lista_eventos=adicionar(lista_eventos,0,c);

	//GERAR CADA ELEMENTO TIPO E TEMPO
	for(i=0; i<tamanho; i++)
		{
			//TIPO DA CHAMADA
			tipochamada=rand() % 3;
			
			//TEMPO DA CHAMADA
         		tempochamada=(double)rand()/RAND_MAX*(60);
    
			lista_eventos=adicionar(lista_eventos, tipochamada, tempochamada);
			
		}
	imprimir(lista_eventos);
	printf("--------------------------------------------------------------------------------");

	//ELIMINAR O PRIMEIRO EVENTO
	lista_eventos = remover(lista_eventos);
	imprimir(lista_eventos);
	printf("--------------------------------------------------------------------------------");
	//CALCULAR  u
	u=((double)rand()+1)/((double)RAND_MAX+1);
	printf("u-%f\n", u);

	//taxa de chegada	
	c=(- (double)log(u) / lambda);
	printf(" c-%f\n", c);
	printf("tamanho-%d\n\n", tamanho);
	double funct[tamanho];
	for(j=0; j<tamanho; j++)
		{
			funct[j]=poisson(lambda, j);
			printf(" %f\n", funct[j]);
		}
	histograma(funct, tamanho);
}
# STEL
