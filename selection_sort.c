#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

typedef struct line
{
	int n;
	char lixo[60];
} Line;

void selection_sort (int *vetor,int max)
{
  int i, j, m, aux, div, mod, id, nt;
	Line *min = NULL;

  for (i = 0; i < (max - 1); i++) {

		#pragma omp parallel private (id, j)
		{

			id = omp_get_thread_num();

			#pragma omp single
			{
				nt = omp_get_num_threads();
				if(min == NULL)
					min = calloc(nt, sizeof(Line));
			}

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
