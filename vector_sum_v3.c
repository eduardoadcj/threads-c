#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

int sum(int *v, int n){
	int i, sum = 0, id, nt;

  #pragma omp parallel private(i, id, nt)
  {
    id = omp_get_thread_num();
    nt = omp_get_num_threads();
    for (i = id; i < n; i+=nt){
			#pragma omp atomic
  		sum += v[i];
  	}
  }
	//funciona corretamente mas demora muito
	// critical - funciona com o controle do sistema operacional
	// atomic - controle de hardware
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
