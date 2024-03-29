﻿// taller5.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

using namespace std;


/*Metodo que ayuda a pasar los datos leidos del archivo txt a un arreglo de enteros utilizando atoi*/
void auxArray(string* arreglo, int* arr, int largo) {

	for (int i = 0; i < largo; i++) {

		string aux = arreglo[i];
		int Aux = atoi(aux.c_str());
		arr[i] = Aux;

	}


}
/*metodo que mezcla dos listas simples y las convierte en una simple lista ordenada*/
void merge(int* arr, int l, int m, int r) {

	int i;
	int j;
	int k;
	int largo1 = m - l + 1;
	int largo2 = r - m;


	int* arrUno = new int[largo1];
	int* arrDos = new int[largo2];

	/*Se copian los datos del arr, a los arreglos arrUno y arrDos*/
	for (i = 0; i < largo1; i++)
		arrUno[i] = arr[l + i];
	for (j = 0; j < largo2; j++)
		arrDos[j] = arr[m + 1 + j];

	/*mezclar los subArreglos*/
	i = 0; //indice inicial del primer sub-arreglo
	j = 0; // indice inicial del segundo sub - arreglo
	k = l; // indice inicial del tercer sub-arreglo
	while (i < largo1 && j < largo2)
	{
		if (arrUno[i] <= arrDos[j]) {

			arr[k] = arrUno[i];

			i++;
		}
		else {
			arr[k] = arrDos[j];
			j++;
		}
		k++;
	}

	while (i < largo1)
	{
		arr[k] = arrUno[i];
		i++;
		k++;
	}

	while (j < largo2)
	{
		arr[k] = arrDos[j];
		j++;
		k++;
	}
}
/*Metodo recursivo de Mergesort*/
void mergeSort(int* arr, int ini, int fin) {

	if (fin > ini) {


		int mitad = (ini + fin) / 2;
		mergeSort(arr, ini, mitad);
		mergeSort(arr, mitad + 1, fin);

		merge(arr, ini, mitad, fin);


	}

}
/*metodo que lee el archivo.txt y retorna un entero que sera el largo del arreglo de los numeros*/
int largoArray() {

	int largo = 0;
	string line;
	ifstream myfile("datos.txt", ios::in);

	if (myfile.is_open()) {

		while (getline(myfile, line)) {

			//cout << "Linea " << (large+1) << ": " <<  line << "\n";

			stringstream st(line);


			while (!st.eof()) {

				string word;
				getline(st, word, ',');
				largo++;

			}
		}
		myfile.close();
	}

	return largo;

}
/*Metodo que lee el archivo.txt e ingresa a un arreglo de strings los valores separados por "," */
void introducirArray(string* arreglo) {


	string line;
	int pos = 0;
	ifstream myfile("datos.txt", ios::in);

	if (myfile.is_open()) {

		while (getline(myfile, line)) {

			stringstream st(line);
			while (!st.eof()) {

				string word;
				getline(st, word, ',');

				arreglo[pos] = word;
				pos++;

			}
		}
		myfile.close();
	}

	return;
}
/*metodo que realiza el cambio de valores para el mergesort*/
void cambiazo(int i, int j, int* vect) {

	int aux = vect[i];
	vect[i] = vect[j];
	vect[j] = aux;

}
/*QuickSort recursivo*/
void quickSort(int* arreglo, int lowerIndex, int highIndex) {

	int i = lowerIndex;
	int j = highIndex;
	int pivote = arreglo[lowerIndex + ((highIndex - lowerIndex) / 2)];

	while (i <= j) {

		while (arreglo[i] < pivote) {

			i++;

		}

		while (arreglo[j] > pivote) {

			j--;
		}

		if (i <= j) {

			cambiazo(i, j, arreglo);
			i++;
			j--;

		}
	}

	if (lowerIndex < j) {

		quickSort(arreglo, lowerIndex, j);

	}

	if (i < highIndex) {

		quickSort(arreglo, i, highIndex);
	}

}
/*QuickSort recursivo, a diferencia del anterior en este metodo el "Pivote" se pasa por parametro*/
void quickSortPivot(int* arreglo, int lowerIndex, int highIndex, int pivote) {

	int i = lowerIndex;
	int j = highIndex;

	while (i <= j) {

		while (arreglo[i] < pivote) {

			i++;

		}

		while (arreglo[j] > pivote) {

			j--;
		}

		if (i <= j) {

			cambiazo(i, j, arreglo);
			i++;
			j--;

		}
	}

	if (lowerIndex < j) {

		quickSort(arreglo, lowerIndex, j);

	}

	if (i < highIndex) {

		quickSort(arreglo, i, highIndex);
	}

}
/*Metodo que retorna un numero entre 0 y el parametro "largo"*/
int random(int largo) {

	srand(time(NULL));

	int random = rand() % largo;

	return random;

}
/*Intento del metodo de quicksort con N hilos, no se llego a algo coherente*/
void parallelQuicksort(int* arreglo, int lowerIndex, int highIndex, int nhebras, int largo) {

	int grain;

	for (grain = 2; grain <= nhebras; ++grain) {

		int residuo = largo % grain;
		int division = largo / grain;

		cout << "residuo: " << residuo << endl;

		if ((largo % grain == 0) && (largo / nhebras == grain)) {

			break;
		}

	};

	cout << endl;
	cout << "Division: " << grain << endl;

	vector<future<void>> vct;
	vector<thread> vect;

	for (int i = 0; i < largo; i += grain) {

		cout << "iteracion: " << i << endl;
		packaged_task<void(int*, int, int)> pk0{ quickSort };
		vct.push_back(pk0.get_future());
		vect.push_back(thread(move(pk0), arreglo, i, (i + grain - 1)));

	}

	for (auto& x : vect) {

		x.join();

	}

}
/*Metodo Quicksort con paralelismo, se utilizan 2 hebras, cada una para una mitad del arreglo respectivo*/
void quicksortParrallel(int* arreglo, int ini, int fin, int pivot) {

	int mitad = (ini + fin) / 2;

	if (ini < fin) {

		thread hebra1{ &quickSortPivot,ref(arreglo),ini,mitad,pivot };		//Hebra que realiza el ordenamiento de la primera mitad del arreglo
		thread hebra2{ &quickSortPivot,ref(arreglo),mitad + 1,fin,pivot };		//Hebra que realiza el ordenamiento de la segunda mitad del arreglo

		hebra2.join(); // -------> join
		hebra1.join();

	}

}

/*Merge sort con paralelismo , se utilizan 2 hebras, cada una para una mitad del arreglo respectivo*/
void merge_sort(int *array, int inicio, int fin) {

	if (inicio >= fin) {
		return;
	}

	int mid = inicio + (fin - inicio) / 2;

	// version con dos hebras para la primera y segunda mitad
	thread th1(merge_sort, ref(array), inicio, mid);
	thread th2(merge_sort, ref(array), mid + 1, fin);
	th1.join();
	th2.join();

	merge(array, inicio, mid, fin);
}


void menu() {
	
	bool salir = false;
	int opcion = 1;
	int option = 1;

	clock_t start, end;
	double tiempo;

	string nHilos; int cantHilos;

	string* arreglo;

	int largo = largoArray();
	arreglo = new string[largo];
	int aleatorio = 0;
	int datosArreglo = (largo - 1);

	int* arregloEnteros = new int[largo];
	introducirArray(arreglo);

	auxArray(arreglo, arregloEnteros, largo);

	do {

		cout << "\n.:::: Menu ::::." << "\n" <<
			"1) Ordenar Arreglo " << "\n" <<
			"2) Salir " << endl;

		cin >> opcion;

		switch (opcion) {

		case 1:

			cout << "\n1. Algoritmo 1 (QuickSort Secuencial) " << "\n" <<
				"2. Algoritmo 2 (QuickSort con Paralelismo) " << "\n" <<
				"3. Algoritmo 3	(MergeSort Secuencial)" << "\n" <<
				"4. Algoritmo 4	(MergeSort con Paralelismo) " << "\n " << endl;

			cin >> option;

			switch (option) {

			case 1:

				cout << "Arreglo desordenado: " << endl;
				cout << "[";
				for (int i = 0; i < largo; i++) { cout << arregloEnteros[i]; if (i != (largo - 1)) { cout << ","; } }
				cout << "]";

				start = clock(); 
				//Quicksort Iterativo				
				quickSort(arregloEnteros, 0, (largo - 1));
				end = clock();
				tiempo = (double)(end - start) / CLOCKS_PER_SEC * 1000.0; 

				cout << "\n";
				cout << "Arreglo ordenado: " << endl;
				cout << "[";
				for (int i = 0; i < largo; i++) { cout << arregloEnteros[i]; if (i != (largo - 1)) { cout << ","; } }
				cout << "]";
				cout << "\n";
				cout << "\n";

				introducirArray(arreglo);
				auxArray(arreglo, arregloEnteros, largo);
				
				cout << "Cantidad de datos del arreglo: " << datosArreglo << endl;

				cout << "Tiempo de ejecucion del Quicksort Secuencial fue de: " <<tiempo<< " milisegundos." << endl;
				cout << "\n";

				break;

			case 2:


				//Validar el ingreso de los hilos
				try {
					cout << "Ingrese el numero de hilos con el que desea trabajar: ";
					cin >> nHilos;
					cantHilos = stoi(nHilos);
				}
				catch (...) {
					cout << "-----ERROR-----\nIngrese un valor valido" << endl;
					break;
				}

				cout << "Arreglo desordenado: " << endl;
				cout << "[";
				for (int i = 0; i < largo; i++) { cout << arregloEnteros[i]; if (i != (largo - 1)) { cout << ","; } }
				cout << "]";

				//Pivote aleatorio
				aleatorio = random(largo);

				start = clock(); 
				//parallelQuicksort(arregloEnteros, 0,largo - 1 , nHilos, largo )
				quicksortParrallel(arregloEnteros, 0, largo - 1, arregloEnteros[aleatorio]);
				end = clock(); 
				tiempo = (double)(end - start) / CLOCKS_PER_SEC * 1000.0; 

				
				cout << "\n";
				cout << "Arreglo ordenado: " << endl;
				cout << "[";
				for (int i = 0; i < largo; i++) { cout << arregloEnteros[i]; if (i != (largo - 1)) { cout << ","; } }
				cout << "]";
				cout << "\n";
				cout << "\n";

				cout << "Cantidad de datos del arreglo: " << datosArreglo << endl;

				cout << "Tiempo de ejecucion del Quicksort con Paralelismo fue de: " << tiempo << " milisegundos." << endl;
				cout << "\n";

				introducirArray(arreglo);					//En estas dos lineas se vuelve a desordenar el arreglo
				auxArray(arreglo, arregloEnteros, largo);	//

				break;

			case 3:

				cout << "Arreglo desordenado: " << endl;
				cout << "[";
				for (int i = 0; i < largo; i++) { cout << arregloEnteros[i]; if (i != (largo - 1)) { cout << ","; } }
				cout << "]";

				start = clock();
				//Mergesort iterativo
				mergeSort(arregloEnteros, 0, largo - 1);
				end = clock();
				tiempo = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

				cout << "\n";
				cout << "Arreglo ordenado: " << endl;
				cout << "[";
				for (int i = 0; i < largo; i++) { cout << arregloEnteros[i]; if (i != (largo - 1)) { cout << ","; } }
				cout << "]";
				cout << "\n";
				cout << "\n";

				cout << "Cantidad de datos del arreglo: " << datosArreglo << endl;
				cout << "Tiempo de ejecucion del Merge sort iterativo fue de: " << tiempo << " milisegundos." << endl;
				cout << "\n";

				introducirArray(arreglo);					//En estas dos lineas se vuelve a desordenar el arreglo
				auxArray(arreglo, arregloEnteros, largo);	//	
				break;

			case 4:

				cout << "Arreglo desordenado: " << endl;
				cout << "[";
				for (int i = 0; i < largo; i++) { cout << arregloEnteros[i]; if (i != (largo - 1)) { cout << ","; } }
				cout << "]";

				start = clock();
				//Mergesort paralelismo
				merge_sort(arregloEnteros, 0, largo - 1);
				end = clock();
				tiempo = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

				cout << "\n";
				cout << "Arreglo ordenado: " << endl;
				cout << "[";
				for (int i = 0; i < largo; i++) { cout << arregloEnteros[i]; if (i != (largo - 1)) { cout << ","; } }
				cout << "]";
				cout << "\n";
				cout << "\n";


				cout << "Tiempo de ejecucion del Merge sort con paralelismo fue de: " << tiempo << " milisegundos." << endl;
				cout << "\n";

				introducirArray(arreglo);
				auxArray(arreglo, arregloEnteros, largo);

				break;
			}
			break;

		case 2:

			salir = true;

			break;

		}

	} while ((opcion > 0 && opcion < 3) && (option > 0 && option < 5) && (salir == false));

	cout << "\nHa salido del programa!!!" << endl;
	system("pause");

}

int main() {

	menu();
	return 0;

}

