#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

int main(int argc, char * argv[]) {

    int rank_sz,
            my_rank;
    int **A, // Matriz a multiplicar
            *x, // Vector que vamos a multiplicar
            *y, // Vector donde almacenamos el resultado
            *miFila, // La fila que almacena localmente un proceso
            *comprueba; // Guarda el resultado final (calculado secuencialmente), su valor
                        // debe ser igual al de 'y'
	int f,c;  //numero de filas y columnas de la matriz A, f es tambien el tamanio del vector x
    double tInicio, // Tiempo en el que comienza la ejecucion
            tFin; // Tiempo en el que acaba la ejecucion

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &rank_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    if(my_rank==0){
    	cout<< "Ingrese el numero de filas"<< endl;
		cin>>f;
		cout<< "Ingrese el numero de columnas"<< endl;
		cin>>c;
		if(f>rank_sz){
			cout<<"El número de filas debe ser menor o igual al número de procesos."<<
					endl<<"Cantidad de filas cambiada a: "<<rank_sz<<endl;
			f=rank_sz;
		}
	}

	//enviamos el numero de filas a todos los procesos
	MPI_Bcast(&f, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&c, 1, MPI_INT, 0, MPI_COMM_WORLD);
	

    A = new int *[f]; // Reservamos "f" filas
    x = new int [c]; // El vector sera del mismo tamanio que el numero
    					//de columnas de la matriz
	

    // Solo el proceso 0 ejecuta el siguiente bloque
    if (my_rank == 0) {	
        A[0] = new int [f * c];
        for (int i = 1; i < f; i++) {
            A[i] = A[i - 1] + c;
        }
        // Reservamos espacio para el resultado
        y = new int [f];

        // Rellenamos 'A' y 'x' con valores aleatorios
        srand(time(0));
        cout << "La matriz A es:" << endl;
        for (int i = 0; i < f; i++) {
            for (int j = 0; j < c; j++) {
                if (j == 0) cout << "[";
                A[i][j] = rand() % 10;
                cout << A[i][j];
                if (j == c - 1) cout << "]";
                else cout << "  ";
            }
            cout << "\n";
        }
        cout << "El vector x es:" << endl;

        for(int i = 0; i < c; i++){
        	x[i] = rand() % 10;
        	cout << "\t  [" << x[i] << "]" << endl;
        }

        //if(my_rank==0) cout<<"gaaa";
        // Reservamos espacio para la comprobacion
        comprueba = new int [f];
        // Lo calculamos de forma secuencial
        for (int i = 0; i < f; i++) {
            comprueba[i] = 0;
            for (int j = 0; j < c; j++) {
                comprueba[i] += A[i][j] * x[j];
            }
        }
    } // Termina el trozo de codigo que ejecuta solo 0


    // Reservamos espacio para la fila local de cada proceso
    miFila = new int [c];


    // Repartimos una fila por cada proceso, es posible hacer la reparticion de esta
    // manera ya que la matriz esta creada como un unico vector.
    MPI_Scatter(A[0], // Matriz que vamos a compartir
            c, // Numero de columnas a compartir
            MPI_INT, // Tipo de dato a enviar
            miFila, // Vector en el que almacenar los datos
            c, // Numero de columnas a compartir
            MPI_INT, // Tipo de dato a recibir
            0, // Proceso raiz que envia los datos
            MPI_COMM_WORLD); // Comunicador utilizado (En este caso, el global)

    // Compartimos el vector entre todas los procesos
    MPI_Bcast(x, // Dato a compartir
            c, // Numero de elementos que se van a enviar y recibir
            MPI_INT, // Tipo de dato que se compartira
            0, // Proceso raiz que envia los datos
            MPI_COMM_WORLD); // Comunicador utilizado (En este caso, el global)


    // Hacemos una barrera para asegurar que todas los procesos comiencen la ejecucion
    // a la vez, para tener mejor control del tiempo empleado
    MPI_Barrier(MPI_COMM_WORLD);
    // Inicio de medicion de tiempo
    tInicio = MPI_Wtime();

    int subFinal = 0;
    for ( int i = 0; i < c; i++) {
        subFinal += miFila[i] * x[i];
    }

    // Otra barrera para asegurar que todas ejecuten el siguiente trozo de codigo lo
    // mas proximamente posible
    MPI_Barrier(MPI_COMM_WORLD);
    // fin de medicion de tiempo
    tFin = MPI_Wtime();

    // Recogemos los datos de la multiplicacion, por cada proceso sera un escalar
    // y se recoge en un vector, Gather se asegura de que la recoleccion se haga
    // en el mismo orden en el que se hace el Scatter, con lo que cada escalar
    // acaba en su posicion correspondiente del vector.
	MPI_Gather(&subFinal, // Dato que envia cada proceso
				1, // Numero de elementos que se envian
				MPI_INT, // Tipo del dato que se envia
				y, // Vector en el que se recolectan los datos
				1, // Numero de datos que se esperan recibir por cada proceso
				MPI_INT, // Tipo del dato que se recibira
				0, // proceso que va a recibir los datos
				MPI_COMM_WORLD); // Canal de comunicacion (Comunicador Global)



    // Terminamos la ejecucion de los procesos, despues de esto solo existira
    // el proceso 0
    // Ojo! Esto no significa que los demas procesos no ejecuten el resto
    // de codigo despues de "Finalize", es conveniente asegurarnos con una
    // condicion si vamos a ejecutar mas codigo (Por ejemplo, con "if(rank==0)".
    MPI_Finalize();

    if (my_rank == 0) {

        unsigned int errores = 0;

        cout << "El resultado obtenido y el esperado son:" << endl;
        for (int i = 0; i < f; i++) {
            cout << "\t" << y[i] << "\t|\t" << comprueba[i] << endl;
            if (comprueba[i] != y[i])
                errores++;
        }

        delete [] y;
        delete [] comprueba;
        delete [] A[0];

        if (errores) {
            cout << "Hubo " << errores << " errores." << endl;
        } else {
            cout << "No hubo errores" << endl;
            cout << "El tiempo tardado ha sido " << tFin - tInicio << " segundos." << endl;
        }
    }
    delete [] x;
    delete [] A;
    delete [] miFila;
}
