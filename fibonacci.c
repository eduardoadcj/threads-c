
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>
#include <stdint.h>

uint64_t fib_ (int n, int i)
{
  if (n < 2){
		return n;
	}else{

    if(i < 2){

      i++;
      uint64_t t1;
      uint64_t t2;

      #pragma omp task shared(t1)
      t1 = fib_(n-1, i);

      t2 = fib_(n-2, i);

      #pragma omp taskwait
      
      return t1 + t2;

    }else{
      return fib_(n-1, i) + fib_(n-2, i);
    }
  }

}
/*
  O problema se associa a quantidade de tasks que estão sendo criadas.
*/

uint64_t fib (int n)
{
  uint64_t r;
  #pragma omp parallel
  {
    #pragma omp single
    {
      r = fib_(n, 0);
    }
  }
  return r;
}

// 0 1 2 3 4 5 6
// 0 1 1 2 3 5 8 ...

uint64_t fib_check (int n)
{
	int i;
	uint64_t prev, fib, tmp;

	if (n < 2)
		return n;

	prev = 0;
	fib = 1;

	for (i=1; i<n; i++) {
		tmp = fib;
		fib += prev;
		prev = tmp;
	}

	return fib;
}

int main(int argc, char **argv)
{
	static char *awnser[] = { "bad", "ok" };

	int i, n, is_ok;
	uint64_t r;
	struct timeval time_start, time_end;
	double elapsed;

	assert(argc == 2);

	n = atoi(argv[1]);
	printf("fib seq: %i\n", n);

	gettimeofday(&time_start, NULL);
	r = fib(n);
	gettimeofday(&time_end, NULL);

	is_ok = (r == fib_check(n));

	elapsed = time_end.tv_sec - time_start.tv_sec + (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

	printf("fibonacci is %llu, which is %s\ntime: %.3f seconds\n", r, awnser[is_ok], elapsed);

	return 0;
}
