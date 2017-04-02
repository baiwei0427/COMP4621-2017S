#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define NUM_ITERS 5

void* my_thread_func(void *arg)
{
	int i;
	int index = *((int*)arg); /* thread index */
	free(arg);

	for (i = 0; i < NUM_ITERS; i++) {
		printf("Thread %d Iterations %d\n", index, i + 1);
		sleep(1);
	}

	return NULL;
}

int main()
{
	int i;
	pthread_t threads[NUM_THREADS];

	/* start threads */
	for (i = 0; i < NUM_THREADS; i++) {

		/* init the thread function argument */
		int *index_ptr = malloc(sizeof(int));
		*index_ptr = i + 1;
		printf("Start thread %d\n", i + 1);

		/* create a thread and start */
		if (pthread_create(&threads[i], NULL, my_thread_func, (void *)index_ptr) != 0) {
			printf("Error when creating thread %d\n", i + 1);
			return 0;
		}
	}

	/* wait for all threads to finish */
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
