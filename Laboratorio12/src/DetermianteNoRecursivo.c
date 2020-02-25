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
int determinantOfMatrix(int mat, int n);

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
		/*int inicio_local=tid*(dimension/numThreads);
		int final_local=(tid+1)*(dimension/numThreads);
		#pragma omp critical
		resultado+=det(A,dimension,inicio_local,final_local);
		//printf("\nHIlo %d resultado: %f",tid,resultado);*/
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

int determinantOfMatrix(int mat, int n)
{
    int num1,num2,det = 1,index,total = 1; // Initialize result

    // temporary array for storing row
    int temp[n + 1];

    //loop for traversing the diagonal elements
    for(int i = 0; i < n; i++)
    {
        index = i; // initialize the index

        //finding the index which has non zero value
        while(mat[index][i] == 0 && index < n) {
            index++;

        }
        if(index == n) // if there is non zero element
        {
            // the determinat of matrix as zero
            continue;


        }
        if(index != i)
        {
            //loop for swaping the diagonal element row and index row
            for(int j = 0; j < n; j++)
            {
                swap(mat[index][j],mat[i][j]);
            }
                //determinant sign changes when we shift rows
                //go through determinant properties
                det = det*pow(-1,index-i);
       }

       //storing the values of diagonal row elements
       for(int j = 0; j < n; j++)
       {
           temp[j] = mat[i][j];

       }
       //traversing every row below the diagonal element
       for(int j = i+1; j < n; j++)
       {
           num1 = temp[i]; //value of diagonal element
           num2 = mat[j][i]; //value of next row element

           //traversing every column of row
           // and multiplying to every row
           for(int k = 0; k < n; k++)
           {
               //multiplying to make the diagonal
               // element and next row element equal
               mat[j][k] = (num1 * mat[j][k]) - (num2 * temp[k]);

           }
           total = total * num1; // Det(kA)=kDet(A);
           }

    }

    //mulitplying the diagonal elements to get determinant
    for(int i = 0; i < n; i++)
    {
        det = det * mat[i][i];

    }
    return (det/total); //Det(kA)/k=Det(A);
}
