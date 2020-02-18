//Autor: Luis Germán
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

const int MAX_THREADS = 1024;

long thread_count;
long long n;
double total_int;
pthread_mutex_t mutex;
double a, b, h;

void* Thread_int(void* rank);

/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);
double f(double x);

int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double start, finish, elapsed;

   /* Get number of threads from command line */
   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));
   a=0;
   b=10;
   n=10000;
   h=(b-a)/n;
   pthread_mutex_init(&mutex, NULL);
   total_int = 0.0;

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
          Thread_int, (void*)thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
   GET_TIME(finish);
   elapsed = finish - start;

   printf("With n = %lld partitions,\n", n);
   printf("   Our estimate of integrate of f(x) in range (%f-%f) = %.15f\n", a,b,total_int);
   printf("The elapsed time is %e seconds\n", elapsed);
   GET_TIME(start);
   total_int = Trap(a,b,n,h);
   GET_TIME(finish);
   elapsed = finish - start;
   printf("   Single thread est  = %.15f\n", total_int);
   printf("The elapsed time is %e seconds\n", elapsed);

   pthread_mutex_destroy(&mutex);
   free(thread_handles);
   return 0;
}  /* main */

/*------------------------------------------------------------------*/
void* Thread_int(void* rank) {
   long my_rank = (long) rank;
   double local_n = n/thread_count;
   double local_a = a + my_rank*local_n*h;
   double local_b = local_a + local_n*h;
   double local_int = Trap(local_a, local_b, local_n, h);

   pthread_mutex_lock(&mutex);
   total_int += local_int;
   pthread_mutex_unlock(&mutex);

   return NULL;
}


/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */
void Get_args(int argc, char* argv[]) {
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   /*if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   n = strtoll(argv[2], NULL, 10);
   if (n <= 0) Usage(argv[0]);*/
}  /* Get_args */


/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   /*fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");*/
   exit(0);
}  /* Usage */

double Trap(
      double left_endpt  /* in */,
      double right_endpt /* in */,
      int    trap_count  /* in */,
      double base_len    /* in */) {
   double estimate, x;
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
} /*  Trap  */


/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   return x*x;
} /* f */
