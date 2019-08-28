#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

int sum(int *v, int n){
	int i, j, id, nt, sum = 0;
	int *t;

  #pragma omp parallel private(i, id)
  {

    id = omp_get_thread_num();

		// Vai exetar apenas uma vez por uma thread qualquer elegida
		#pragma omp single
		{
			nt = omp_get_num_threads();
			t = (int *) calloc(nt, sizeof(int) );
		}

		t[id] = 0;

    for (i = id; i < n; i+=nt)
  		t[id] += v[i];

		/*
			Fica mais lento por causa da questão do compartilhamento de cache.
			Cada linha de cache possui 64bytes, cada int possui 4bytes. Necessáriamente as variáveis
			gravam uma após a outra na mesma linha de cache. Então, mesmo sendo visivelmente variaveis em lugares
			diferentes, elas preenchem a mesma linha da cache, portanto, o recurso passa a ter um problema de compartilhamento.
			Esse problema é conhecido como falso compartilhamento de cache. Uma forma de resolver seria a atulização da tecnica pedding
		*/

  }

	for(j = 0; j < nt; j++)
		sum += t[j];

	return sum;

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
