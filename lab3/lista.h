#ifndef LISTA_H_   /* Include guard */
#define LISTA_H_

// Definição da estrutura da lista
typedef struct{
	int tipo;
	double tempo;
	struct lista * proximo;
	double previsao;
	double inicio;
} lista;

// Função que remove o primeiro elemento da lista
lista * remover (lista * apontador);

// Função que adiciona novo elemento à lista, ordenando a mesma por tempo
lista * adicionar (lista * apontador, int n_tipo, double n_tempo, double n_previsao, double n_inicio);

// Função que imprime no ecra todos os elementos da lista
void imprimir (lista * apontador);

#endif // LISTA_H_
