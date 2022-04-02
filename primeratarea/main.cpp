#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <vector>
#include <map>
using namespace std;
vector<vector<int>> vector_1;

void crear_vectores(int elementos, int dimension){
    int numero_elementos;
	int numero_dimension;
//	srand(time(NULL));
//	int numero_aleatorio;
//	for(int i = 0; i<elementos; i++){
//		vector<int> temp;
//		for(int j = 0; j<dimension; j++){
//			numero_aleatorio = 1 + rand() % (101 - 1);  ///numeros entre 1-100
//			temp.push_back(numero_aleatorio);
//		}
//		vector_1.push_back(temp);
//	}
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> dis(1, 100);
		numero_dimension = dimension; numero_elementos = elementos;
		vector_1.resize(elementos);
		for (int i = 0; i < elementos; i++) {
			vector_1[i].resize(dimension);
		}
		srand(time(NULL));
		for (int i = 0; i < elementos; i++) {
			for (int j = 0; j < dimension; j++) {
				vector_1[i][j] = dis(gen);
			}
		}
}

void print_Vector(int elementos, int dimension){
    for(int i = 0; i<elementos; i++){
        cout << i + 1 << ": ";
        for(int j = 0; j<dimension; j++){
            cout<<vector_1[i][j]<<" ";
        }
        cout<<endl;
    }
}

void crearHistograma(map<int,int> &histograma, vector<int> datos){
    map<int,int>::iterator it;
    for(int elemento=0;elemento<datos.size();elemento++){
        it=histograma.find(datos[elemento]);
        //Si no encontro el elemento
        if(it==histograma.end()){
            histograma.insert({datos[elemento],1});
            continue;
        }
        it->second+=1;
    }
}
int distancia_euclidiana(int elementos, int dimension){
    cout<<"------------"<<endl;
    vector<int> elementossss;
    int lista[elementos-1];
    int num=0;
    for(int i = 0; i<1; i++){
        for(int j = i+1; j<elementos; j++){
            int resultadito = 0;
            for(int k = 0; k<dimension; k++){
                resultadito +=pow((vector_1[j][k])-(vector_1[i][k]),2);
                //cout<<karma<<endl;

            }
            resultadito = sqrt(resultadito);
            //cout<<"Distancia Euclidiana:"<<karma<<" entre los vectores "<<i<<" y "<<j<<endl;
            //lista[num]=karma;
            elementossss.push_back(resultadito);
            num++;
            resultadito = 0;
        }
    }

    std::map<int,int> histograma;


    cout<<"distancia-numero de veces"<<endl;
    crearHistograma(histograma,elementossss);

    map<int,int>::iterator it=histograma.begin();
    for (;it!=histograma.end();it++){
        cout<<it->first<<" - "<<it->second<<endl;
    }
}

int main(int argc, char *argv[]) {
    cout<<"Bienvenido a Estructuras de Datos Avanzadas"<<endl;
    cout<<"Primer Trabajo"<<endl;
    vector<int> dimensiones ={1,2,3,4,5,6,7,8,9,10,20,30,40,50,60,70,80,90,100};
    vector<int> elementos = {20000};
    for (int i = 0; i < dimensiones.size(); i++) {
        for (int j = 0; j < elementos.size(); j++) {
                cout << "dimension " << dimensiones[i] << " con elementos " << elementos[j] << endl;
                crear_vectores(elementos[j],dimensiones[i]);
                cout<<"++++++"<<endl;
                //print_Vector(elementos[j],dimensiones[i]);
                distancia_euclidiana(elementos[j],dimensiones[i]);
                cout<<"------------------------------------------------"<<endl;
        }
    }
}
