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
		e utilizar a linha inteira de uma cache de 64bytes. 
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
				A função do laço é encontrar a posição em que se encontra o menor valor do subvetor, 
				e armazena-la na posição referente ao ID da thread atual no vetor min.
			*/
			#pragma omp for
			for (j = i+1; j < max; j++) {
				if (vetor[j] < vetor[min[id].n]) {
					min[id].n = j;
				}
			}

		}

		m = 0;
		
		/*
			Encontrando a posição em que o menor valor do array min,
			que armazena os menores valores obtidos por cada thread.
		*/
		for(j = 1; j < nt; j++){
			if(vetor[min[j].n] < vetor[min[m].n])
				m = j;
		}

		// Pegando o menor valor a partir da posição obtida
		m = min[m].n;

	//Move o valor da posição i do vetor para a variavel auxiliar
    aux = vetor[i];

	//Troca o valor da posição pelo menor valor encontrado
    vetor[i] = vetor[m];

	//atribui à posição do valor em que o menor valor foi encontrado ao valor da variável auxiliar
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
