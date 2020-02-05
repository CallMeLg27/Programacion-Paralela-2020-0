#include "mpi.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {
    int n, x, local_cont, cont, rank, size;
    vector<int> VectorA, VectorALocal;
	srand(time(NULL));
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        cout << "Ingrese el tamano del vector: ";
        cin>>n;
        VectorA.resize(n, 0);
        cout<< "\nVector generado aleatoriamente:"<<endl;
		for(int i=0;i<n;i++){
        	VectorA[i]=rand()%10;
        	cout<<VectorA[i]<<" ";
		}
		cout << "\nIngrese x: ";
		cin>>x;
    }
    VectorALocal.resize(n/size, 0);
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&x,1,MPI_INT,0,MPI_COMM_WORLD);

    // Repartimos los valores del vector A
    MPI_Scatter(&VectorA[0], // Valores a compartir
            n / size, // Cantidad que se envia a cada proceso
            MPI_INT, // Tipo del dato que se enviara
            &VectorALocal[0], // Variable donde recibir los datos
            n / size, // Cantidad que recibe cada proceso
            MPI_INT, // Tipo del dato que se recibira
            0,  // proceso principal que reparte los datos
            MPI_COMM_WORLD); // Comunicador (En este caso, el global)

    // Conteo de ocurrencias de x en el subvector VectorALocal
    local_cont = 0;
    for (int i = 0; i < n / size; ++i) {
    	if (VectorALocal[i]==x)
        	local_cont ++;
    }

    // Reunimos los datos en un solo proceso, aplicando una operacion
    // aritmetica, en este caso, la suma.
    MPI_Reduce(&local_cont, // Elemento a enviar
            &cont, // Variable donde se almacena la reunion de los datos
            1, // Cantidad de datos a reunir
            MPI_INT, // Tipo del dato que se reunira
            MPI_SUM, // Operacion aritmetica a aplicar
            0, // Proceso que recibira los datos
            MPI_COMM_WORLD); // Comunicador

    if (rank == 0)
        cout << "N° de ocurrencias de x en el vector A = " << cont << endl;

    MPI_Finalize();
    return 0;
}
