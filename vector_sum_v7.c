#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

typedef struct soma
{
	int n;
	char lixo[60];
} Soma;

int sum(int *v, int n){
	int i, j, id, nt, sum = 0;
	Soma *somas;

  #pragma omp parallel private(i, id)
  {

    id = omp_get_thread_num();

		#pragma omp single
		{
			nt = omp_get_num_threads();
			somas = calloc(nt, sizeof(Soma));
		}

		somas[id].n = 0;

    for (i = id; i < n; i+=nt)
  		somas[id].n += v[i];

  }

	for(j = 0; j < nt; j++)
		sum += somas[j].n;

	return sum;

	/*
		Implementacão da tecnica de padding utilizando uma struct para preencher a
		memória com lixo. O problema que relaciona esta forma de programar é que
		quando os valores estão intercalados, as treads tendem a carregar em cache
		muitos dados que não vão ser acessados, desta forma, o ideal é fazer com que
		cada tread já tenha uma faixa do vetor para iterar.
	*/

}

int main(int argc, char **argv){
	static char *awnser[] = { "bad", "ok" };

	int i, sumv, n, id, nt;
	struct timeval time_start, time_end;
	double elapsed;

	if(argc == 2)
		n = atoi(argv[1]);
	else
		n = 1000000000;
	printf("number of elements: %d\n", n);

	int *vector = (int *) calloc(n, sizeof(int));
	assert(vector != NULL);

	for(i = 0; i < n; i++)
		vector[i] = 1;

	gettimeofday(&time_start, NULL);
	sumv = sum(vector, n);
  gettimeofday(&time_end, NULL);

	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

	printf("sum value is %u, which is %s\ntime: %.3f seconds\n", sumv, awnser[sumv == n], elapsed);

	return 0;
}
