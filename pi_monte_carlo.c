#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#define BASE 10000000

uint64_t pi_monte_carlo (uint64_t nit)
{
	uint64_t i, count, result;
	double x, y;
	int id;

	result = 0;

	#pragma omp parallel private(x, y, i, count, id)
	{

		count = 0;
		unsigned short rbuffer[3];
		id = omp_get_thread_num();

		for(int j = 0; j < 2; j++)
			rbuffer[j] = id+j;

		#pragma omp for
		for (i=0; i<nit; i++) {

			x = erand48(rbuffer)*2.0 - 1.0; // creates floats between
			y = erand48(rbuffer)*2.0 - 1.0; // 1 and -1

			if ((x*x + y*y) < 1.0)
				count++; // stone hit the pond

		}

		#pragma omp atomic
		result += count;

	}

	return result;
}

int main (int argc, char **argv)
{
	double pi, elapsed;
	uint64_t total_it, count;
	struct timeval timer_begin_app, timer_end_app;

	if (argc == 2)
		total_it = strtoull(argv[1], NULL, 10);
	else if (argc == 1)
		total_it = BASE;
	else {
		printf("usage: %s <number_of_stones>\n", argv[0]);
		exit(1);
	}

	gettimeofday(&timer_begin_app, NULL);
	count = pi_monte_carlo(total_it);
	gettimeofday(&timer_end_app, NULL);

	elapsed = timer_end_app.tv_sec - timer_begin_app.tv_sec + (timer_end_app.tv_usec - timer_begin_app.tv_usec) / 1000000.0;

	pi = 4.0 * (double)count/(double)total_it;

	printf("pi: %f\nExecution time: %.3f seconds\n", pi, elapsed);

	return 0;
}
