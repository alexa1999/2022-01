#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include "key_value_pair.h"

class Node{
    public:
        KeyValuePair key_value_pair;// Almacena la clave y el valor del Nodo
        vector<Node*> next;// Almacena la referencia del siguiente nodo hasta el nivel superior del nodo
        mutex node_lock; // Lock para bloquear el nodo al modificarlo
        atomic<bool> marked = {false}; // Atomic para marcar si se elimina este Nodo
        atomic<bool> fully_linked = {false}; //var atomic para indicar que el Nodo está completamente vinculado a predecesores y sucesores
        int top_level; // El nivel máximo hasta el cual el nodo está disponible

        Node();
        Node(int key, int level);
        Node(int key, string value, int level);
        ~Node();
        int get_key();
        string get_value();
        void lock();
        void unlock();
};
