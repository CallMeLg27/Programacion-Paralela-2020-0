#include "mpi.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {
    int n, rank, size;
    long long int local_prod, prod;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0) {
        cout << "Ingrese n: ";
        cin>>n;
    }
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    //Agilizamos el trabajo con el siguiente algoritmo:
    //Sea n=8, con p=2
    //n! = 1x2x3x4x5x6x7x8
    //proceso 0: 1x3x5x7
    //proceso 1: 2x4x6x8
    local_prod = 1;
    for (int i = 0; i < n / size; i++) {
    	local_prod *= size*i+rank+1;
    }

    // Reunimos los datos en un solo proceso, aplicando una operacion
    // aritmetica, en este caso, el producto.
    MPI_Reduce(&local_prod, // Elemento a enviar
            &prod, // Variable donde se almacena la reunion de los datos
            1, // Cantidad de datos a reunir
            MPI_LONG_LONG_INT, // Tipo del dato que se reunira
            MPI_PROD, // Operacion aritmetica a aplicar
            0, // Proceso que recibira los datos
            MPI_COMM_WORLD); // Comunicador

    if (rank == 0)
        cout <<n<< "! = " << prod << endl;
        
    MPI_Finalize();
    return 0;
}

