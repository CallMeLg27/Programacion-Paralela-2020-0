#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
int n;
long long *A = NULL;
int *F = NULL;
void* Fibo(void* rank);

int main(int argc,char* argv[])
{
	long thread;
	pthread_t* thread_handles;
	thread_count = strtol(argv[1], NULL, 10);
	thread_handles = malloc(thread_count*sizeof(pthread_t));
	scanf("%d",&n);
	A = (long long *) malloc(n*sizeof(long long));
	F = (int *) malloc(thread_count*sizeof(int));
	A[0]=0;
	A[1]=1;
	for(thread = 0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL,
				Fibo, (void*) thread);

	for(thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);
	for (int i=0;i<n;i++){
		printf("%lld ", A[i]);
	}
	free(thread_handles);
	return 0;

}


void* Fibo(void* rank)
{
	long my_rank = (long) rank;
	int nums = n/thread_count;
	int inicio=nums*my_rank;
	int final=inicio+nums;
	while(!(my_rank==0 || F[my_rank-1]==1));

	for(int i=inicio;i<final;i++){
		if(i>1) A[i]=A[i-1]+A[i-2];
	}
	F[my_rank]=1;
    return NULL;
}
