#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int thread_count;
int factorial=1;
int n;
int resto;
void* Fact(void* rank);

int main(int argc,char* argv[])
{
	long thread;
	pthread_t* thread_handles;
	thread_count = strtol(argv[1], NULL, 10);
	thread_handles = malloc(thread_count*sizeof(pthread_t));
	scanf("%d",&n);
	resto=n%thread_count;
	for(thread = 0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL,
				Fact, (void*) thread);

	for(thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);
	printf("%d",factorial);
	free(thread_handles);
	return 0;

}


void* Fact(void* rank)
{
	long my_rank = (long) rank;
	int nums = n/thread_count;
	int inicio=nums*my_rank+1;
	int final=inicio+nums;
	if (resto>0){

		resto--;
	}

    for(int i=inicio;i<final;i++){
    	if(i>n) break;
    	factorial*=i;
    }
    return NULL;
}
