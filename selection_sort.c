#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

//struct para consumir a linha de cache por inteiro.
//tecnica de padding
typedef struct line
{
	int n;
	char lixo[60];
} Line;

void selection_sort (int *vetor,int max)
{
	//iniciando variaveis
  int i, j, m, aux, div, mod, id, nt;
	
	/*
		Struct que serve para aplicar a tecnica de padding
		e utilizar a linha inteira de uma cache de 64bits. 
		Pode ser substituida por um int normal.
	*/
	Line *min = NULL;

	//For para percorrer o vetor inteiro
  for (i = 0; i < (max - 1); i++) {

		//Iniciando contexto paralelo
		#pragma omp parallel private (id, j)
		{

			//Obtendo identificador da thread atual
			id = omp_get_thread_num();

			/*
				Disparando instrução para ser utilizada na primeira thread.
				Esta instrução será executada apenas uma vez.
			*/
			#pragma omp single
			{
				//Obtendo o numero de threads disponíveis
				nt = omp_get_num_threads();
				//Caso o valor min ainda não esteja inicializado. Inicia-se com o calloc
				if(min == NULL)
					//aloca-se memória para um vetor de nt posições
					min = calloc(nt, sizeof(Line));

			}

			//Atribuindo o valor na posição referente a thread atual
			min[id].n = i;

			/*
				Essa notação divide os iteradores entre as threads automaticamente.
			*/
			#pragma omp for
			for (j = i+1; j < max; j++) {
				if (vetor[j] < vetor[min[id].n]) {
					min[id].n = j;
				}
			}

		}

		m = 0;
		
		for(j = 1; j < nt; j++){
			if(vetor[min[j].n] < vetor[min[m].n])
				m = j;
		}

		m = min[m].n;

    aux = vetor[i];
    vetor[i] = vetor[m];
    vetor[m] = aux;

  }

  /* Imprime o vetor ordenado */
  for (i = 0; i < max; i++) {
    printf ("%d ",vetor[i]);
  }
  printf ("\n");

}

int main()
{
  int max, i;
	
	printf("Informe o tamanho do vetor\n");
	scanf ("%d",&max);

  int  vetor[max];
  for (i = 0; i < max; i++) {
    scanf ("%d",&vetor[i]);
  }

	printf("Iniciando ordenação...\n");
  selection_sort (vetor, max);

	return 0;

}
