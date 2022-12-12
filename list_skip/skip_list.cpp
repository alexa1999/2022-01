#include <iostream>
#include <math.h>
#include <limits>
#include <map> 
#include <stdio.h> 
#include <stdlib.h>
#include "skip_list.h"

#define INT_MINI numeric_limits<int>::min() 
#define INT_MAXI numeric_limits<int>::max()

static int max_level;
//contructor
SkipList::SkipList(int max_elements, float prob){
    max_level = (int) round(log(max_elements) / log(1/prob)) - 1;
    head = new Node(INT_MINI, max_level);
    tail = new Node(INT_MAXI, max_level);

    for (size_t i = 0; i < head->next.size(); i++) {
        head->next[i] = tail;
    }
}


  //  Encuentra los predecesores y sucesores en cada nivel donde existe o podría existir una clave dada.
  //   Actualiza las referencias en el vector utilizando pasar por referencia.
  //   Devuelve -1 si no la clave no existe.

int SkipList::encontrar(int key, vector<Node*> &predecessors, vector<Node*> &successors) {
    int found = -1;
    Node *prev = head; 

    for (int level = max_level; level >= 0; level--){
        Node *curr = prev->next[level];
        while (key > curr->get_key()){
            prev = curr;
            curr = prev->next[level];
        }
        if(found == -1 && key == curr->get_key()){found = level;}
        predecessors[level] = prev;
        successors[level] = curr;
    }
    return found;
}
 //   Genera aleatoriamente un número e incrementa el nivel si el número es menor o igual a 0.5
  //   Una vez más de 0,5, devuelve el nivel o nivel máximo disponible.
  //   Esto decide hasta qué nivel está disponible un nuevo Nodo.

int SkipList::get_random_level() {
    int l = 0;
    while(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= 0.5){l++;}
    return l > max_level ? max_level : l;
}



 //  se inserta en la skip list haciendo el uso de locks
  // return si este existe 

bool SkipList::insertar(int key, string value) {

    //Para obtener el numero de niveles del nuevo nodo
    int top_level = get_random_level();
    // Inicializando el vector con las referencias a los predecesores y sucesores
    vector<Node*> preds(max_level + 1); 
    vector<Node*> succs(max_level + 1);
    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }
    //si es que se cambian los predecesores y sucesores, sigue intentando insertar el elemento
    while(true){
        //encuentra los predecesores y sucesores de donde debe insertarse el key
        int found = encontrar(key, preds, succs);
        // Si se encuentra y se marca, esperar y continuar insertar
         // Si se encuentra y no está marcado, espere hasta que esté completamente vinculado y regrese. No se necesita inserción
         // Si no lo encuentra, continúe con la inserción
        if(found != -1){
            Node* node_found = succs[found];
            if(!node_found->marked){
                while(! node_found->fully_linked){
                }
                return false;
            }
            continue;
        }
    // Almacenar todos los Nodos que bloqueamos adquirimos en un mapa
         // Mapa utilizado para que no intentemos adquirir el bloqueo de un Nodo que ya hemos adquirido
         // Esto puede pasar cuando tenemos el mismo predecesor en diferentes niveles
        map<Node*, int> locked_nodes;
        // Recorre la skip list e intenta adquirir el lock del predecesor en cada nivel
        try{
            Node* pred;
            Node* succ;
            // comprueba si el predecesor y los sucesores son iguales desde que intentamos leerlos antes
            bool valid = true;
            for (int level = 0; valid && (level <= top_level); level++){
                pred = preds[level];
                succ = succs[level];
                // Si no esta bloqueado, entoces se bloquea
                if(!(locked_nodes.count( pred ))){
                    pred->lock();
                    locked_nodes.insert(make_pair(pred, 1));
                }
                // si es que el pred esta marcado o si esque cambia el pred y el sucesor, 
                // se cancela y lo intenta de nuevo 
                 valid = !(pred->marked.load(std::memory_order_seq_cst)) && !(succ->marked.load(std::memory_order_seq_cst)) && pred->next[level]==succ;                
            }
            // Si no se cumplen las condiciones, libere los locks, aborte e inténtelo de nuevo.
            if(!valid){
                for (auto const& x : locked_nodes){x.first->unlock();}
                continue;
            }
            // Al cumplirse las condiciones, se crea un nodo y se insera
            //ya que tenemos todos los locks necesarios
            Node* new_node = new Node(key, value, top_level);
            // Actualizar el predecesor y los sucesores
            for (int level = 0; level <= top_level; level++){
                new_node->next[level] = succs[level];
            }
            for (int level = 0; level <= top_level; level++){
                preds[level]->next[level] = new_node;
            }
            //Marca el nodo como completamente enlazado.()
            new_node->fully_linked = true;
            // libera el lock de todos los nodos al finalizar con los inserts
            for (auto const& x : locked_nodes){x.first->unlock();}
            return true;
        }catch(const std::exception& e){
           //si hay alguna excepcion en la insercion , libera los bloqueos de los nodos y se intenta de nuevo
           std::cerr << e.what() << '\n';
            for (auto const& x : locked_nodes){x.first->unlock();}
        }
    }
}

   // Realiza una búsqueda para encontrar si existe un nodo.
   // Devuelve el valor si se encuentra la clave, en caso contrario devuelve vacío

string SkipList::buscar(int key){
// Encuentra al predecesor y a los sucesores 
    vector<Node*> preds(max_level + 1); 
    vector<Node*> succs(max_level + 1);
    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }
    int found = encontrar(key, preds, succs);

    // Si no se encuentra, devuelve vacío.
    if(found == -1){return "";}

    Node *curr = head; 

    for (int level = max_level; level >= 0; level--){
        while (curr->next[level] != NULL && key > curr->next[level]->get_key()){
            curr = curr->next[level];
        }
    }

    curr = curr->next[0];
    
    // si lo encuentra , si no esta marcado y completamente enlazado devuelve el valor
    //dino devuelve vacion
    if ((curr != NULL) && (curr->get_key() == key) && succs[found]->fully_linked && !succs[found]->marked){
        return curr->get_value();
    }else {
        return "";
    }
}

/**
    Borra de la  Skip list en el lugar apropiado usando locks.
    Devuelve si la clave no existe en la lista.
*/
bool SkipList::eliminar(int key){
    // inicializamos
    Node* victim = NULL;
    bool is_marked = false;
    int top_level = -1;

    // inicializamos las referencias de los pred y sucess
    vector<Node*> preds(max_level + 1); 
    vector<Node*> succs(max_level + 1);

    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }

    // Sigue intentando eliminar el elemento de la lista. En caso de que los predecesores y sucesores se cambien,
    // este bucle ayuda a intentar borrar de nuevo
    while(true){
        
        // Busca los predecesores y sucesores de la clave que se desea eliminar
        int found = encontrar(key, preds, succs);

        // Si se encuentra, seleccione el nodo a eliminar. else return
        if(found != -1){
            victim = succs[found];
        }

        // Si no se encuentra el nodo y el nodo que se eliminara está completamente enlazado y no está marcado lo devuelve
        if(is_marked | (found != -1 && (victim->fully_linked && victim->top_level == found && !(victim->marked))))
        {
        // Si no está marcado, bloqueamos el nodo y lo marcamos para borrarlo
            if(!is_marked){
                top_level = victim->top_level;
                victim->lock();
                if(victim->marked){
                    victim->unlock();
                    return false;
                }
                victim->marked = true;
                is_marked = true;
            }

            // Almacenar todos los Nodos cuyo bloqueo adquirimos en un mapa
           // /El mapa es para no adquirir el bloqueo de un nodo que ya ha sido adquirido
            map<Node*, int> locked_nodes;

            // Recorre la lista de saltos e intenta adquirir el bloqueo del predecesor en cada nivel
            try{
                Node* pred;
                // lo utilizamos para comprobar si los pred no estan marcados para el borrado
                //y si es que el pred siguiente es el nodo que estamos intentando borrar o si esta cambiado 
                bool valid = true;
                for(int level = 0; valid && (level <= top_level); level++){
                    pred = preds[level];
                    // Si aún no se ha adquirido el bloqueo, adquiéralo 
                    if(!(locked_nodes.count( pred ))){
                        pred->lock();
                        locked_nodes.insert(make_pair(pred, 1));
                    }
                        
                    //si el pred marcado o si el sig pred a cambiado
                    // abortamos y se vuelve a intentar
                    valid = !(pred->marked) && pred->next[level] == victim;
                }

                // Si no se cumplen las condiciones, libere los bloqueos, abortamos e inténtelo de nuevo.
                if(!valid){
                    for (auto const& x : locked_nodes){ x.first->unlock(); }
                    continue;
                }

                // Si se cumplen todas las condiciones, elimine el nodo y vincúlelo a sus sucesores.
                for(int level = top_level; level >= 0; level--){
                    preds[level]->next[level] = victim->next[level];
                }
                victim->unlock();
                // Una vez finalizada el borrado, suelte los bloqueos.
                for (auto const& x : locked_nodes){ x.first->unlock(); }

                return true;
            }catch(const std::exception& e){
                // Si se produce alguna excepción durante la eliminación anterior, l
                //libere los bloqueos de los nodos retenidos e inténtelo de nuevo.
                for (auto const& x : locked_nodes){ x.first->unlock(); }
            }

        }
        else{return false;}
    }
}
    //Busca la clave_inicial en la lista de saltos recorriéndola una vez alcanzado un punto más cercano a la clave_inicial
    //llega al nivel 0 para encontrar todas las claves entre clave_inicial y clave_final. Si la búsqueda excede el final, entonces aborta
    //Actualiza y devuelve los pares clave-valor en un mapa.

map<int, string> SkipList::range(int start_key, int end_key){

    map<int, string> range_output;
    if(start_key > end_key){return range_output; }
    Node *curr = head;

    for (int level = max_level; level >= 0; level--){
        while (curr->next[level] != NULL && start_key > curr->next[level]->get_key()){
            if(curr->get_key() >= start_key && curr->get_key() <= end_key){
                range_output.insert(make_pair(curr->get_key(), curr->get_value()));
            }
            curr = curr->next[level];
        }
    }

    while(curr != NULL && end_key >= curr->get_key()){
        if(curr->get_key() >= start_key && curr->get_key() <= end_key){
            range_output.insert(make_pair(curr->get_key(), curr->get_value()));
        }
        curr = curr->next[0];
    }
    return range_output;

}

void SkipList::display(){
    for (int i = 0; i <= max_level; i++) {
        Node *temp = head;
        int count = 0;
        if(!(temp->get_key() == INT_MINI && temp->next[i]->get_key() == INT_MAXI)){
            printf("Level %d  ", i);
            while (temp != NULL){
                printf("%d -> ", temp->get_key());
                temp = temp->next[i];
                count++;
            }
            cout<<endl;
        }
        if(count == 3) break;
    }
    printf("---------- acabo ----------\n\n");
}

SkipList::SkipList(){}

SkipList::~SkipList(){}
