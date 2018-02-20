#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
// Definição da estrutura da lista
typedef struct{
	int tipo;
	double tempo;
	double tempo_final;
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
//lista * adicionar (lista * apontador, int n_tipo, double n_tempo, double n_tempo_final)
lista * adicionar (lista * apontador, int n_tipo, double n_tempo, double n_tempo_final) // falta adicionar aqui double n_tempo_final
{
	lista * lap = apontador;
	lista * ap_aux, * ap_next;
	if(apontador == NULL)
	{
		apontador = (lista *) malloc(sizeof (lista));
		apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> tempo = n_tempo;
		apontador -> tempo_final = n_tempo_final;
		return apontador;
	}
	else
	{
		if (apontador->tempo > n_tempo) {
	        ap_aux = (lista *) malloc(sizeof (lista));
	        ap_aux -> tipo = n_tipo;
            ap_aux -> tempo = n_tempo;
	    ap_aux -> tempo_final = n_tempo_final;
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
		apontador -> tempo_final = n_tempo_final;
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

double get_c( double lambda){
	double value;
	double c;
	value =rand();
	if(value ==0)
		value++;
	c=(1.0/(double)lambda) * (- log(value / (double)RAND_MAX));
	return c;
}

// Pequeno exemplo de utilização

int main(void)
{

	lista  * lista_eventos;
	int tipo_ev; 
	double tempo_ev;
	lista_eventos = NULL;
	double lambda=1.2;

	double c=0.0;
	int total=0;
	double timet=0;
	double estimator=0;
	double d=0;
	int block_count=0;
	int calls_count=0;
	long int Rc=1711500;
	long int Rt=0; 
	int bit_qeue=0;
	long int F=100000;
	int talking_bitrate = 17200;
	int silent_bitrate = 1104;
	double tempo_final_ev;
	double dm_c=0;
	double dm_p=0;
	double dm=0;
	int tipo_chamada=0;
	int number_calls=0;
	int number_calls_end=0;
	double time_calling=0;
	double time_talking=0;
	double time_silent=0;
	long int lost_bits=0;
	long int total_bitrate=0;
	long int silent_bits=0;
	long int talking_bits=0;
	long int average_req_bitrate=0;
	long int bits_generated=0;
	int ntalking=0;
	int nsilent=0;


        srand(time(NULL));
	c=get_c(lambda);
	lista_eventos=adicionar(lista_eventos,0,c,0); // tipo 0 - inicio de chamada ; tipo 1 - talking ; tipo 2 - silence

	for(total =0; number_calls_end<100000; total++){
		if ( lista_eventos != NULL)
		{
			tipo_ev = lista_eventos -> tipo;
			tempo_ev = lista_eventos -> tempo;
			tempo_final_ev = lista_eventos -> tempo_final;
			lista_eventos = remover(lista_eventos);
			bits_generated+=Rt*(tempo_ev-timet);
			talking_bits+=ntalking*talking_bitrate*(tempo_ev-timet);
			silent_bits+=nsilent*silent_bitrate*(tempo_ev-timet);

			if(Rt < Rc){
				if(bit_qeue==0)
					total_bitrate+=Rt;
				else	total_bitrate+=Rc;
			}
			else total_bitrate+=Rc;

					
			if(Rt > Rc || bit_qeue !=0){
				bit_qeue+=(tempo_ev-timet) * (Rt-Rc);
				if(bit_qeue <0)
					bit_qeue=0;
				if(bit_qeue > F){
					lost_bits+= (bit_qeue-F);
					bit_qeue=F;
				}
				
				
			}

			if(tipo_ev == 1){

				ntalking--;
				
				if(tempo_ev == tempo_final_ev){
					Rt=Rt-talking_bitrate;
					calls_count--;	
					number_calls_end++;
				}
				else{
					dm_p = tempo_ev + get_c(1/3.0);
					
					if(dm_p > tempo_final_ev)  
						dm_p= tempo_final_ev;
					
					lista_eventos=adicionar(lista_eventos,2,dm_p, tempo_final_ev);
					Rt=Rt-talking_bitrate+silent_bitrate;
					time_silent+=dm_p-tempo_ev;
					nsilent++;
				}
				
			}
			
			else if(tipo_ev == 2){
				
				nsilent--;
				if(tempo_ev == tempo_final_ev){
					Rt=Rt-silent_bitrate;
					calls_count--;
					number_calls_end++;
					
				}
				else{
					dm_c = tempo_ev + get_c(1/2.0);
					
					if(dm_c > tempo_final_ev)
						dm_c= tempo_final_ev;
					lista_eventos=adicionar(lista_eventos,1,dm_c, tempo_final_ev);
					Rt=Rt+talking_bitrate-silent_bitrate;
					time_talking+=dm_c-tempo_ev;
					ntalking++;
				}
			
			}
			
			else if(tipo_ev == 0){
				if(1){
				
					d=tempo_ev+get_c(1.0/180.0); //aqui devemos gerar um evento de fim de chamada e meter na lista. Quando recebermos um evento de fim decrementamos calls_count;
			
					if ( rand() < RAND_MAX /2)
						tipo_chamada = 1;
					else tipo_chamada =2;
 					
					if(tipo_chamada == 1){
						dm = tempo_ev + get_c(1/2.0);
						
						Rt=Rt+talking_bitrate;
						time_talking+=dm-tempo_ev;
						ntalking++;
					}

					else{
						dm = tempo_ev + get_c(1/3.0);
						Rt=Rt+silent_bitrate;
						time_silent+=dm-tempo_ev;
						nsilent++;
					}
					if(dm > d)
						dm= d;

					lista_eventos=adicionar(lista_eventos,tipo_chamada,dm, d);
					number_calls++;
					calls_count++;
				}
				else block_count++;
				
				c=get_c(lambda);

				lista_eventos=adicionar(lista_eventos,0,c+tempo_ev, 0);
			}
			timet=tempo_ev;
		
		}
		else {
			break;
		}
	}

	estimator=tempo_ev/(number_calls);
	time_calling=time_talking+time_silent;
	long int bits=average_req_bitrate*tempo_ev;
	double prop_talking=(double) (talking_bits)/bits_generated;
	double prop_silent=(double) (silent_bits)/bits_generated;
	long int voice_pack= (talking_bits/688.0);
	long int lost_voice_pack =((lost_bits * prop_talking) / 688.0) ;
	
	printf("\nLISTA ACTUAL\n");
	printf("estimator: %lf\n", estimator);
	printf("Time simulated %lf \n", tempo_ev);
	printf("Number bits generated %ld \n", bits_generated);
	printf("Number talking bits generated %ld, ( %ld packets) \n", talking_bits, (long int)(talking_bits/688.0));
	printf("Number silent bits generated %ld, (%ld packets) \n", silent_bits, (long int)(silent_bits/368.0));
	printf("average request rate %ld \n",(long int) (bits_generated/tempo_ev));
	printf("Proportion of talking bits %lf \n",prop_talking);
	printf("Proportion of silent bits %lf \n",prop_silent);
	printf("Number of bits lost: %ld %lf \n", lost_bits, (double) (lost_bits)/bits_generated);
	printf("Bits transfered %ld %lf\n",bits_generated-lost_bits,(double) (bits_generated-lost_bits)/bits_generated);
	printf("Number of talking bits lost: %ld, (%ld packets) \n", (long int)(lost_bits * prop_talking), (long int)((lost_bits * prop_talking) / 688.0) );
	printf("Number of silent bits lost : %ld, (%ld packets) \n", (long int) (lost_bits * prop_silent), (long int)((lost_bits * prop_silent) / 368.0) );
	printf("Rate of lost packets: %lf  \n ",(double) lost_voice_pack/voice_pack); 

	while(lista_eventos !=NULL){
	lista_eventos = remover(lista_eventos);
	}

}

