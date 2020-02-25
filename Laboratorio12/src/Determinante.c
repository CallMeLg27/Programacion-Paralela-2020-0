/*
 ============================================================================
 Name        : Prog02-AddVectors.c
 Author      : Herminio
 Version     :
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double det(int **m, int b, int w, int z);
int cuadrado(int a, int b);


int main(int argc, char *argv[]) {
	srand(time(NULL));
	int dimension;
	int numThreads, tid;
	double resultado=0.0;
	printf("\n Digite el orden de la matriz... ");
	scanf("%d",&dimension);
	int **A = (int **)malloc(sizeof(int *)*dimension);

	for(int i=0; i<dimension; i++) A[i]=(int *)malloc(sizeof(int)*dimension);

	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			A[i][j]=rand()%100;
			printf(" %d",A[i][j]);

		}
		printf("\n");
	}

	omp_set_num_threads(dimension);
	#pragma omp parallel private(numThreads, tid)
	{
		int tid = omp_get_thread_num();
		int numThreads = omp_get_num_threads();
		int inicio_local=tid*(dimension/numThreads);
		int final_local=(tid+1)*(dimension/numThreads);
		#pragma omp critical
		resultado+=det(A,dimension,inicio_local,final_local);
		//printf("\nHIlo %d resultado: %f",tid,resultado);
	}

	printf("\nDeterminante %f",resultado);
	printf("\n");
	for(int r=0; r<dimension; r++) free(A[r]);
	free(A);
	return 0;
}

double det(int **m, int b, int w, int z){

        double determinante = 0, aux = 0.0;

        int c;

        if(b==2)

                return m[0][0]*m[1][1] - m[1][0]*m[0][1];

        else{

                for(int j=w; j<z; j++){

                        int **menor = (int **)malloc(sizeof(int *)*(b-1));

                        for(int h=0; h<(b-1); h++) menor[h] = (int *)malloc(sizeof(int)*(b-1));

                        for(int k=1; k<b; k++){

                                c = 0;

                                for(int l=0; l<b; l++){

                                        if(l!=j){

                                                menor[k-1][c] = m[k][l];

                                                c++;

                                        }

                                }

                        }

                        aux = cuadrado(-1, 2+j)*m[0][j]*det(menor, b-1,w,z);

                        determinante += aux;

                        for(int q = 0; q<(b-1); q++)

                                free(menor[q]);

                        free(menor);

                }
                //free(m);
                printf("\nResultado Parcial: %f",determinante);
                return determinante;

        }

}

int cuadrado(int a, int b){
	int c=1;
	for(int i=0;i<b;i++){
		c=c*a;
	}
	return c;

}
