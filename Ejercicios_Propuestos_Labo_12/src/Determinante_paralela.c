/*
 ============================================================================
 Name        : DETERMINATE DE UNA MATRIZ DE NXN
 Author      : LUIS GERMAN
 Version     : 1.0
 Copyright   : Distribucion libre
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>

//TAMAÑO DE LA MATRIZ:
#define N 4
double determinantOfMatrix(double mat[N][N], int n);
double cofactor(double mat[N][N], int b,int j);
int main (int argc, char *argv[]) {
	double totalDet=0;
	srand(time(NULL));
  int tid;
  double mat[N][N];
  for(int i=0;i<N;i++){
  		for(int j=0;j<N;j++){
  			mat[i][j]=rand()%3;
  		}
  	}
  for(int i=0;i<N;i++){
  		for(int j=0;j<N;j++){
  			printf("%lf ",mat[i][j]);
  		}
  		printf("\n");
  }
  	omp_set_num_threads(N);
  	#pragma omp parallel
 {
	 //por cofactores
	 /*
	  * 00	01	02	03
	  * 10	11	12	13
	  * 20	21	22	23
	  * 30	31	32	33
	  *
	  * tid=0,1,2,3
	  * 00*det
	  *
	  *
	  */
	 //luego cada cofactor se resuleve con determinant of a a matrix
	 tid = omp_get_thread_num();
	 double cof=cofactor(mat,N,tid);
	 #pragma omp critical
	 totalDet+=cof;

 }
 printf("%lf ",totalDet);
 return 0;
}

double determinantOfMatrix(double mat[N][N], int n)
{
	int index;
	double num1,num2,det = 1.0;
	double temp[n + 1];
	for(int i = 0; i < n; i++)
	{
		index = i;

		while(mat[index][i] == 0 && index < n) {
			index++;

		}
		if(index == n)
		{
			continue;

		}
		if(index != i)
		{
			for(int j = 0; j < n; j++)
			{
				int aux=mat[index][j];
				mat[index][j]=mat[i][j];
				mat[i][j]=aux;
			}
				if((index-i)%2!=0){
					det*=-1;
				}
	}

	for(int j = 0; j < n; j++)
	{
		temp[j] = mat[i][j];

	}
	for(int j = i+1; j < n; j++)
	{
		num1 = temp[i];
		num2 = mat[j][i];

		for(int k = 0; k < n; k++)
		{
			mat[j][k] = ((num1 * mat[j][k]) - (num2 * temp[k]))/num1;

		}
		}

	}

	for(int i = 0; i < n; i++)
	{
		det = det * mat[i][i];
	}

	return (det);
}

double cofactor(double mat[N][N], int b,int j){
	double menor[b][b];
	int c;
	for(int i=0;i<b;i++){
			for(int j=0;j<b;j++){
				menor[i][j]=0;
			}
		}

	for(int k=1; k<b; k++){

			c = 0;

			for(int l=0; l<b; l++){

					if(l!=j){

							menor[k-1][c] = mat[k][l];

							c++;

					}

			}

	}
	double determinant = mat[0][j]*determinantOfMatrix(menor,b-1);
	printf("determinant: %lf \n",determinant);
	if(j%2!=0)
		determinant*=-1;
	return determinant;
}

