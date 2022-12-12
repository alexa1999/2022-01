/**
	Aqui realizaremos las pruebas de tiempos para la skip list
*/
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
#include <chrono>
#include "skip_list.h"

using namespace std::chrono;
using namespace std;

size_t num_threads = 1;
SkipList skiplist;
size_t max_number = 100;
struct timespec start_time, end_time;
vector<int> numbers_insert;
vector<int> numbers_delete;
vector<int> numbers_get;
vector<pair<int,int>> numbers_range;


void mostrar_uso(){
	cout << "Usage: \n\n" ;
	cout << "./tiempos [--name] -i <max_number> -t <num_threads> --tiempos=<insert, delete, search, range> [--help] \n" ;
	cout << "--name                         Imprime el nombre completo \n" ;
	cout << "-i <max_number>               Los números de 0 a número_máximo se insertan en la lista de omisión, se elige un subconjunto para obtener, eliminar y rango \n" ;
	cout << "-t <num_threads>               num max de threads  \n" ;
	cout << "--tiempos=''insert''           realiza el insert multithreaded  del  input \n" ;
	cout << "--tiempos=''delete''           realiza el delete multithreaded  en el input \n" ;
	cout << "--tiempos=''search''           realiza la busqueda multithreaded  en el input \n" ;
    cout << "--help                         imprime el uso del prg\n"; 
    cout << "\n[ max_number tiene que estar entre INT_MIN y INT_MAX y excluyendo el INT_MIN y INT_MAX ]\n";
	exit(EXIT_FAILURE);
}

/**
    Muestra el tiempo transcurrido
*/
void mostrar_tiempos(){
    unsigned long long tiempos;
	tiempos = (end_time.tv_sec-start_time.tv_sec)*1000000000 + (end_time.tv_nsec-start_time.tv_nsec);
	printf("tiempo (ns): %llu\n",tiempos);
	double tiemposs = ((double)tiempos)/1000000000.0;
	printf("tiempo (s): %lf\n",tiemposs);
}

void generate_input(int max_number){
    // generando datos de insercion 
    for(int i = 1; i <= max_number; i++){
        numbers_insert.push_back(i);
    }
    // delete
    for(size_t i = 0; i < numbers_insert.size(); i++){
        numbers_delete.push_back(numbers_insert[i]);
    }
    // buscar
    for(size_t i = 0; i < numbers_insert.size(); i++){
        numbers_get.push_back(numbers_insert[i]);
    }
    // generando datos rand
    for(size_t i = 0; i < num_threads; i++){
        int a = (rand() % static_cast<int>(max_number + 1));
        int b = a + (rand() % static_cast<int>(max_number - a + 1));
        numbers_range.push_back(make_pair(a,b));
    }
}

void skiplist_add(size_t start, size_t end){
    if(end >= numbers_insert.size()) end = numbers_insert.size();
    if(start == end) skiplist.insertar(numbers_insert[start], to_string(numbers_insert[start]));
    for(size_t i = start; i < end; i++){
        skiplist.insertar(numbers_insert[i], to_string(numbers_insert[i]));
    }
}

void skiplist_remove(size_t start, size_t end){
    if(end >= numbers_delete.size()) end = numbers_delete.size();
    if(start == end) skiplist.eliminar(numbers_delete[start]);
    for(size_t i = start; i < end; i++){
        skiplist.eliminar(numbers_delete[i]);
    }
}

void skiplist_search(size_t start, size_t end){
    if(end >= numbers_get.size()) end = numbers_get.size();
    if(start == end) end++;
    for(size_t i = start; i < end; i++){
        string s = skiplist.buscar(numbers_get[i]);
    }
}


void skiplist_range(int start, int end){
    map<int, string> range_output = skiplist.range(start, end);
}

void insert_tiempos(){
    vector<thread> threads;
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    for(size_t i = 0; i < numbers_insert.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_add, i, i+chunk_size));
    }
    for (auto &th : threads) {th.join();}
    threads.clear();
}

void delete_tiempos(){
    vector<thread> threads;
    int chunk_size = ceil(float(numbers_delete.size()) / num_threads);
    for(size_t i = 0; i < numbers_delete.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_remove, i, i+chunk_size));
    }
    for (auto &th : threads) {th.join();}
    threads.clear();
}
void search_tiempos(){
    vector<thread> threads;
    int chunk_size = ceil(float(numbers_get.size()) / num_threads);
    for(size_t i = 0; i < numbers_get.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_search, i, i+chunk_size));
    }
    for (auto &th : threads) {th.join();}
    threads.clear();
}
void range_tiempos(){
    vector<thread> threads;
    for(size_t i = 0; i < num_threads; i++){
        threads.push_back(thread(skiplist_range, numbers_range[i].first, numbers_range[i].second));
    }
    for (auto &th : threads) {th.join();}
    threads.clear();

}

/**
   Realiza las operaciones de inserción, eliminación y rango 
*/
int main(int argc, char *argv[]){

    static struct option long_options[] = {
        {"name", no_argument, NULL, 'n'},
        {"tiempos", required_argument, NULL, 'b'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    bool name = false;
    string tiempos = "";
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
            case 'b':
                tiempos = std::string(optarg);
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

    if(name){cout << "skip list ";}
    if(help){mostrar_uso();}
    if(argc > 2){
	    if(tiempos == "" || max_number <= 0 || num_threads < 1 ){
	        mostrar_uso();
	    }else{

	        if(tiempos == "insert"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                high_resolution_clock::time_point t1 = high_resolution_clock::now();
                insert_tiempos();
                high_resolution_clock::time_point t2 = high_resolution_clock::now();
	            duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	            std::cout << "" << time_span.count() << " seconds." << std::endl;
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }
	        else if (tiempos == "delete"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                insert_tiempos();
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                delete_tiempos();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }else if (tiempos == "search"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                insert_tiempos();
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                search_tiempos();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }else if (tiempos == "range"){
                generate_input(max_number);
                skiplist = SkipList(numbers_insert.size(), 0.5);
                insert_tiempos();
                clock_gettime(CLOCK_MONOTONIC,&start_time);
                range_tiempos();
                clock_gettime(CLOCK_MONOTONIC,&end_time);
	        }
            else{
	            cout << "Invalido  \n";
	            mostrar_uso();
	        }
            mostrar_tiempos();
	    }
    }else{
        mostrar_uso();
    }
    return 0;
}