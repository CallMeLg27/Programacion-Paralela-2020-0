//============================================================================
// Name        : EjemploProduct.cpp
// Author      : Esquivel Grados Luis Germán
// Version     : 1.0
// Copyright   : Distribucion libre
// Description : Efectúa la operación "Producto" usando varios procesos para agilizar el trabajo
//============================================================================

#include "mpi.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <time.h>
using namespace std;
double producto(vector<double> vec, int n);
int main(int argc, char *argv[]) {
    int tama, rank, size;
    double local_prod,total_prod;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL));
    if (argc < 2) {
        if (rank == 0) {
            cout << "No se ha especificado numero de elementos, multiplo "
            		"de la cantidad de entrada, por defecto sera " << size * 100;
            cout << "\nUso: <ejecutable> <cantidad>" << endl;
        }
        tama = size * 100;
    } else {
        tama = atoi(argv[1]);
        if (tama < size)
        	tama = size;
        else {
            int i = 1, num = size;
            while (tama > num) {
                ++i;
                num = size*i;
            }
            if (tama != num) {
                if (rank == 0)
                    cout << "Cantidad cambiada a " << num << endl;
                tama = num;
            }
        }
    }
    vector<double> VectorA, VectorALocal;
	VectorA.resize(tama, 0);
	VectorALocal.resize(tama/size, 0);
	if (rank == 0) {
		for (long i = 0; i < tama; ++i) {
			VectorA[i] = 1+rand()%10; // Generamos numeros aleatorios de 100 a 999
		}
	}
	MPI_Scatter(&VectorA[0], // Valores a compartir
	            tama/size, // Cantidad que se envia a cada proceso
	            MPI_DOUBLE, // Tipo del dato que se enviara
	            &VectorALocal[0], // Variable donde recibir los datos
	            tama/size, // Cantidad que recibe cada proceso
	            MPI_DOUBLE, // Tipo del dato que se recibira
	            0,  // proceso principal que reparte los datos
	            MPI_COMM_WORLD); // Comunicador (En este caso, el global)
	local_prod=producto(VectorALocal,tama/size);
	MPI_Reduce(&local_prod, // Elemento a enviar
	            &total_prod, // Variable donde se almacena la reunion de los datos
				1, // Cantidad de datos a reunir
	            MPI_DOUBLE, // Tipo del dato que se reunira
	            MPI_PROD, // Operacion aritmetica a aplicar
	            0, // Proceso que recibira los datos
	            MPI_COMM_WORLD); // Comunicador
	 if(rank==0){
		 cout<<"Vector:"<<endl;
		 for(int i=0;i<tama;i++){
			 cout<<VectorA[i]<<" ";
		 }
		 cout<<"Producto de valores de A: "<<total_prod<<endl;
	 }
	 MPI_Finalize();
	return 0;
}

double producto(vector<double> vec, int n){
	int prod=1;
	for (int i=0; i<n;i++){
		prod*=vec[i];
	}
	return prod;
}
