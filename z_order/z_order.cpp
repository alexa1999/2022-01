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
#include <bitset>
#include<queue>
using namespace std;
vector<vector<unsigned short>> vector_1;
vector<bitset<8> > bvc;
vector<vector<string>> vec_str(5);
int const it=4;
int cantidad_vecinos[it]={5,10,15,20};
void crear_vectores(int elementos, int dimension){
	int numero_elementos;
	int numero_dimension;
	
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(1,8);
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
struct nodo{
	int dato,indi;
	nodo(int dato1,int indi1)
	{
		dato=dato1;
		indi=indi1;
	}
};
bool comparar(const nodo& a, const nodo& b)
{
	return a.dato<b.dato;
}
void print_Vector(int elementos, int dimension,vector<vector<int>> &arr1,vector<vector<int>> &arr22){
	vector<vector<string>> arr(elementos);
	vector<vector<int>>datos(elementos);
	for(int i = 0; i<elementos; i++){
		cout << i + 1 << ": ";
		for(int j = 0; j<dimension; j++){
			int a;
			std::string binary =std::bitset<4>(vector_1[i][j]).to_string();
			std::cout<<binary<<"--"<<vector_1[i][j]<<endl;
			arr[i].push_back(binary);
			a=std::stoi(binary, nullptr, 2);
			datos[i].push_back(a);
				//cout<<"fdfd"<<vec_str[i][j]<<endl;
		}
		cout<<endl;
	}
	arr1=datos;

	
	string a;
	string b;
	string aqui;
	int dim=dimension;
	int sumita=0;
	vector<vector<string>> arr2(dimension); //01100110011001101100110

	for(int i = 0; i<elementos; i++){ //3 elementos 
		for(int j = 0; j<4; j++){//6 dimensiones
			for(int k=0;k<dimension;k++)
			{
				a=arr.at(i).at(k);
				cout<<a<<endl;
				b=b+a.at(j);
					
				cout<<"aqui    "<<b<<endl;
			}
			sumita++;
			cout<<"aqui"<<b<<endl;
			cout<<"------"<<endl;
			aqui=b;
		}
		cout<<"-->"<<i<<"------"<<aqui<<endl;
		arr2[i].push_back(b);
		b=" ";
	}
		cout<<arr2[1][0];
	cout<<"de bit a entero"<<endl;
	vector<vector<int>>datos1(elementos);
		for(int i = 0; i<elementos; i++){
			int a;
			a=std::stoi(arr2[i][0], nullptr, 2);
			cout<<a<<endl;
			datos1[i].push_back(a);
			//cout<<"aqjnjod "<<datos[i]<<endl;
				
	}
//	return arr;
		
	arr22=datos1;	
	//return arr2;	
}
void int_bits(int elementos,int dimension)
	{
		for(int i = 0; i<elementos; i++){
			cout << i + 1 << ": ";
			for(int j = 0; j<dimension; j++){
			cout<<vector_1[i][j]<<" ";
		}
		cout<<endl;
	}
}
int distancia_eu(vector<vector<int>>datos_multi,vector<vector<int>>dato,int dim,int k)
{
	int sum=0;
	for(int i=0; i<dim;i++)
	{
		sum+=pow((dato[k][i]-datos_multi[0][i]),2);
	}
	return sqrt(sum);
}
vector<int>vecinos_cerca_arr(vector<vector<int>>arr,int cantidad_vecinos,int num_datos,int dim,int k)
{
	cantidad_vecinos++;
	priority_queue<nodo,vector<nodo>,decltype(comparar)*>q1(comparar);
	for(int i=0;i<cantidad_vecinos;i++)
	{
		q1.push(nodo(INT32_MAX,-1));
	}
	
	for(int i=0;i<num_datos;i++)
	{
		nodo distancia(distancia_eu(arr[0][i],dato[i],dim,k);
		q1.push(distancia)
		q1.pop();
	}
	vector<int>ind;
	for(int i=0; i<cantidad_vecinos-1;i++)
	{
		ind.push_back(q1.top().index);
		q1.pop()
	}
	return ind;
}
vector<int>vecinos_cerca_z(vector<vector<int>>arr,int cantidad_vecinos,int num_datos,int dim,int dato)
{
	cantidad_vecinos++;
	priority_queue<nodo,vector<nodo>,decltype(comparar)*>q1(comparar);
	for(int i=0;i<cantidad_vecinos;i++)
	{
		q1.push(nodo(INT32_MAX,-1));
	}
	
	for(int i=0;i<num_datos;i++)
	{
		nodo distancia(abs(arr[0][i]-dato),i);
		q1.push(distancia);
			q1.pop();
	}
	vector<int>ind;
	for(int i=0; i<cantidad_vecinos-1;i++)
	{
		ind.push_back(q1.top().indi);
		q1.pop();
	}
	return ind;
}
int contador_coincide(vector<vector<int>> vec1, vector<vector<int>> vec2, int d)
{
	int a = 0;
	for (int i = 0; i < d; ++i)
	{
		for (int j = 0; j < d; ++j)
		{
			if (vec1[i]==vec2[j])
			{
				a++;
				break;
			}
		}
	}
	return a;
}
int main(int argc, char *argv[]) {
	cout<<"Bienvenido a Estructuras de Datos Avanzadas"<<endl;
	vector<vector<int>> arr,z_order;
	int dim=10;
	int elem=15;
	vector<int> dimensiones ={dim};
	vector<int> elementos = {elem};
	vector<vector<int>>vecinos_arr,vecinos_z;

			cout << "elements " << elementos[0] << " con dimensiones " << dimensiones[0] << endl;
			crear_vectores(elementos[0],dimensiones[0]);
			cout<<"++++++"<<endl;
			// en arr tenemos la matriz de datos multidimensionales
			//en z_order
			print_Vector(elementos[0],dimensiones[0],arr,z_order);
			//multidimensional
			//descomentar para ver los datos
			for(int i = 0; i< elem; i++){
				cout << i + 1 << ": ";
				for(int j = 0; j<dim; j++){
					cout<<arr[i][j]<<" ";
				}
			}
////////////////////////////////////////
			int dato_random=rand()%elem;
			for(int i=0;i<it;i++)
			{
				
				vecinos_z.push_back(vecinos_cerca_z(z_order,cantidad_vecinos,elem,dim,dato_random);
			}
		

	
}
				
