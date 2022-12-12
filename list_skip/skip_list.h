#include <map>
#include "node.h"

class SkipList{
    private:
        // Cabeza y cola de la skip list
        Node *head;
        Node *tail;
    public:
        SkipList();
        SkipList(int max_elements, float probability);
        ~SkipList();
        int get_random_level();
        int encontrar(int key, vector<Node*> &predecessors, vector<Node*> &successors);
        bool insertar(int key, string value);
        string buscar(int key);
        bool eliminar(int key);
        map<int, string> range(int start_key, int end_key);
        void display();
};
