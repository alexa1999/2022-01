#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iterator>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "skip_list.h"
using namespace std;

size_t num_threads = 1;
SkipList skiplist;

vector<int> numbers_insert;
vector<int> numbers_delete;
vector<int> numbers_get;
vector<pair<int,int>> numbers_range;

void show_usage(){
	cout << "Usage: \n\n" ;
	cout << "./benchmark [--name] -i <max_number> -t <num_threads> --benchmark=<insert, delete, search, range, all_operations, high_contention, low_contention> [--help] \n" ;
	cout << "--name                         Imprime el nombre completo \n" ;
	cout << "-i <max_number>               Los números de 0 a número_máximo se insertan en la lista de omisión, se elige un subconjunto para obtener, eliminar y rango \n" ;
	cout << "-t <num_threads>               num max de threads  \n" ;
	cout << "--benchmark=''insert''           realiza el insert multithreaded  del  input \n" ;
	cout << "--benchmark=''delete''           realiza el delete multithreaded  en el input \n" ;
	cout << "--benchmark=''search''           realiza la busqueda multithreaded  en el input \n" ;
    cout << "--help                         imprime el uso del prg\n"; 
    cout << "\n[ max_number tiene que estar entre INT_MIN y INT_MAX y excluyendo el INT_MIN y INT_MAX ]\n";

	exit(EXIT_FAILURE);
}

//genera los datos de entrada para la skip lis

void generate_input(int max_number){
    // generating insert data
    for(int i = 1; i <= max_number; i++){numbers_insert.push_back(i);}
    // generating delete data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        if( rand() % 4 == 0 ){numbers_delete.push_back(numbers_insert[i]);}
    }
    // generating search data
    for(size_t i = 0; i < numbers_insert.size(); i++){
        if( rand() % 4 == 0 ){numbers_get.push_back(numbers_insert[i]);}
    }
    // generating range data
    for(size_t i = 0; i < num_threads; i++){
        int a = (rand() % static_cast<int>(max_number + 1));
        int b = a + (rand() % static_cast<int>(max_number - a + 1));
        numbers_range.push_back(make_pair(a,b));
    }
}

//Realiza una inserción paralela para la skip list desde el índice de inicio hasta el índice final del vector de inserción
void skiplist_add(size_t start, size_t end){
    if(end >= numbers_insert.size()) end = numbers_insert.size();
    if(start == end) skiplist.insertar(numbers_insert[start], to_string(numbers_insert[start]));
    for(size_t i = start; i < end; i++){
        skiplist.insertar(numbers_insert[i], to_string(numbers_insert[i]));
    }
}

//realiza un delete paralelo para la skip list
void skiplist_remove(size_t start, size_t end){
    if(end >= numbers_delete.size()) end = numbers_delete.size();
    if(start == end) skiplist.eliminar(numbers_delete[start]);
    for(size_t i = start; i < end; i++){
        skiplist.eliminar(numbers_delete[i]);
    }
}

//realiza una busqueda paralela
void skiplist_search(size_t start, size_t end){
    if(end >= numbers_get.size()) end = numbers_get.size();
    if(start == end) end++;
    for(size_t i = start; i < end; i++){
        string s = skiplist.buscar(numbers_get[i]);
        if(s.empty()) s = "Not Found";
        cout << "buscando" << numbers_get[i] << " valor de busqueda: " << s << endl;
    }
}

//realiza un rango paralelo
void skiplist_range(int start, int end){
    map<int, string> range_output = skiplist.range(start, end);

    string s = "";
    for (auto const& x : range_output){
        s += x.second + " ";
    }
    cout << "rango (" << start << ", " << end << ") = " << s << endl;
}

//Realiza operaciones en la skip una tras otra. Cada operación funciona simultáneamente.

void concurrent_skiplist_seperate(){
    vector<thread> threads;

    // insert
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_add, i, i+chunk_size));
    }
    for (auto &th : threads) {th.join();}
    cout << "\n---------- Skip list después de insertar ----------" << endl;
    skiplist.display();
    threads.clear();
    cout << "\n---------- Eliminar los siguientes elementos de la Skip list ----------" << endl;

    for (auto i = numbers_delete.begin(); i != numbers_delete.end(); ++i)
    std::cout << *i << ' ';
    // delete
    chunk_size = ceil(float(numbers_delete.size()) / num_threads);
    for(size_t i = 0; i < numbers_delete.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_remove, i, i+chunk_size));
    }
    for (auto &th : threads) { th.join();}

    cout << "\n\n---------- Skip list despues de eliminar---------- " << endl;
    skiplist.display();
    threads.clear();

    cout << "\n---------- buscando num Skip list ---------- " << endl;
    // search
    chunk_size = ceil(float(numbers_get.size()) / num_threads);
    for(size_t i = 0; i < numbers_get.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_search, i, i+chunk_size));
    }
    for (auto &th : threads) {th.join();}
    threads.clear();

    cout << "\n---------- Rango entre números aleatorios en Skip list ---------- " << endl;
    // range
    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(skiplist_range, numbers_range[i].first, numbers_range[i].second));
    }
    for (auto &th : threads) { th.join(); }

    threads.clear();
}


void skiplist_combined_operations(){

    int start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    int end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
    cout << "insertar: " << start << " " << end << endl;
    skiplist_add(start, end);

    start = (rand() % static_cast<int>(numbers_delete.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_delete.size() - start + 1));
    cout << "eliminar: " << start << " " << end << endl;
    skiplist_remove(start, end);

    start = (rand() % static_cast<int>(numbers_get.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_get.size() - start + 1));
    cout << "buscar: " << start << " " << end << endl;
    skiplist_search(start, end);

    start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
    cout << "Rango: " << start << " " << end << endl;
    skiplist_range(start, end);
}

//Realiza todas las operaciones en paralelo en elskip list.

void concurrent_skiplist_combined(){

    cout << "\n---------- Números a insertar ----------" << endl;

    for (auto i = numbers_insert.begin(); i != numbers_insert.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    cout << "\n---------- Numbers a eliminar ----------" << endl;

    for (auto i = numbers_delete.begin(); i != numbers_delete.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    cout << "\n---------- Numbers a buscar ----------" << endl;

    for (auto i = numbers_get.begin(); i != numbers_get.end(); ++i)
    std::cout << *i << ' ';
    cout << endl;

    vector<thread> threads;

    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(skiplist_combined_operations));
    }
    for (auto &th : threads) {th.join();}

    skiplist.display();

}


/**
    Realiza las operaciones de inserción, eliminación, obtención y rango en la skip list según el tipo de operación
*/
int main(int argc, char *argv[]){

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {"operation", required_argument, NULL, 'o'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    //Variables for options
    bool name = false;
    int max_number = -1;
    string operation = "";
    bool help = false;

    while (true) {
        int option_index = 0;
        int flag_char = getopt_long(argc, argv, "i:t:", long_options, &option_index);
        if (flag_char == -1) {
          break;
        }

        switch (flag_char) {
            case 'n':
                name = true;
                break;
            case 'o':
                operation = std::string(optarg);
                break;
            case 'h':
                help = true;
                break;
            case 't':
                num_threads = stoi(optarg);
                break;
            case 'i':
                max_number = stoi(optarg);
                break;
            case '?':
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if(name){
        cout << " skip list";
    }

    if(help){
        show_usage();
    }

	else{
        show_usage();
    }
    return 0;
}