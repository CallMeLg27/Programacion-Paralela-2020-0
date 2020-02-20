#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int thread_count;
int n;
long long *A = NULL;
sem_t* semaphores;
long total=0;
void* Fibo(void* rank);

int main(int argc,char* argv[])
{
	long thread;
	pthread_t* thread_handles;
	thread_count = strtol(argv[1], NULL, 10);
	thread_handles = malloc(thread_count*sizeof(pthread_t));
	semaphores = malloc(thread_count * sizeof(sem_t));
	scanf("%d",&n);
	A = (long long *) malloc(n*sizeof(long long));
	A[0]=0;
	A[1]=1;
	for(thread = 0; thread < thread_count; thread++)
		sem_init(&semaphores[thread], 0, 0);
	sem_post(&semaphores[0]);
	for(thread = 0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL,
						Fibo, (void*) thread);
	for(thread = 0; thread < thread_count; thread++){
		pthread_join(thread_handles[thread], NULL);
		sem_destroy(&semaphores[thread]);
	}
	for (int i=0;i<n;i++){
		printf("%lld ", A[i]);
	}

	printf("La suma total es: %ld",total);
	free(semaphores);
	free(thread_handles);
	return 0;

}


void* Fibo(void* rank)
{
	long my_rank = (long) rank;
	int nums = n/thread_count;
	int inicio=nums*my_rank;
	long local=0;
	int final=inicio+nums;
	long dest = (my_rank + 1) % thread_count;

	sem_wait(&semaphores[my_rank]);
	printf("%ld ",my_rank);
	for(int i=inicio;i<final;i++){
		if(i>1) A[i]=A[i-1]+A[i-2];
		local+=A[i];
	}
	total+=local;
	sem_post(&semaphores[dest]); /* "Unlock" the semaphore of dest */
    return NULL;
}
