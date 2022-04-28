#include<bits/stdc++.h>
using namespace std;

void agregar(string &palabra, int maxlen, string a)
{
    while(palabra.size() != maxlen)
        palabra.insert(0, a);
}

bool iguales(vector<float> A, vector<float> B){
    for(size_t i=0; i<A.size(); ++i)
        if( A[i] != B[i] )
            return false;
    return true;
}

class nodo{
    public:
        vector<float> datos;
        nodo *nodos[2];

        nodo(){
            nodos[0] = nodos[1] = nullptr; }

        nodo(int D, vector<float> dato){
            datos.reserve(D);
            for(size_t i = 0; i<D; ++i)
                datos.push_back(dato[i]);
            nodos[0] = nodos[1] = nullptr;
        }
};

class KD_tree{
    public:
        int altura;
        nodo *raiz;

        KD_tree(int D): dimension(D), altura(0), raiz(nullptr){}
        void PRINT(queue<nodo *>, int);
        nodo *insertar(nodo *raiz, vector<float> dato, int profundidad);
        void eliminar(vector<float> dato, int profundidad);
        void setAltura(vector<nodo *>);
        bool buscar(nodo *raiz, vector<float> dato, int profundidad);

    private:
        int dimension;
        nodo *in_delete(nodo *raiz, vector<float> dato, int profundidad, vector<nodo *> &add);
};
bool KD_tree::buscar(nodo *raiz, vector<float> dato, int profundidad)
{
    if(raiz == nullptr)
        return false;
    if(iguales( raiz->datos, dato))
        return true;
    int i = profundidad%dimension;
    if( raiz->datos[i] > dato[i] )
            return buscar(raiz->nodos[0],dato,profundidad+1);
    return buscar(raiz->nodos[1], dato, profundidad+1);
}


nodo *KD_tree::insertar(nodo *raiz, vector<float> dato, int profundidad){
    if(raiz == nullptr){
        return new nodo(dimension, dato);
    }
    else{
        int i = profundidad%dimension;
        if( raiz->datos[i] > dato[i] )
            raiz->nodos[0] = insertar(raiz->nodos[0], dato, profundidad+1);
        else
            raiz->nodos[1] = insertar(raiz->nodos[1], dato, profundidad+1);
    }
    return raiz;
}
void KD_tree::eliminar( vector<float> dato, int profundidad ){
    vector<nodo *> tmp;
    in_delete(raiz, dato, 0, tmp);
    if(iguales(raiz->datos, dato)){ ///caso root
        int i = (tmp[0]->datos[0]>tmp[1]->datos[0] )? 0 : 1;
        this->raiz->datos = tmp[i]->datos;
        tmp.erase(tmp.begin()+i);
        tmp.erase(tmp.begin());
        raiz->nodos[i] = nullptr;

        for(auto i : tmp)
            i = insertar(raiz, i->datos, 0);
        return;
    }
    for(auto i : tmp)
        i = insertar(raiz, i->datos, 0);
}
nodo *KD_tree::in_delete(nodo *raiz, vector<float> dato, int profundidad, vector<nodo *> &add){
    if(iguales(raiz->datos, dato)){
        queue<nodo *> tmp;
        tmp.push(raiz);
        raiz = nullptr;

        while(!tmp.empty()){
            if(tmp.front()){
                if(tmp.front()->nodos[0] != nullptr){
                    tmp.push(tmp.front()->nodos[0]);
                    add.push_back(tmp.front()->nodos[0]);
                }
                if(tmp.front()->nodos[1] != nullptr){
                    tmp.push(tmp.front()->nodos[1]);
                    add.push_back(tmp.front()->nodos[1]);
                }
            }
            tmp.pop();
        }
    }
    else{
        int i = profundidad%dimension;
        if( raiz->datos[i] > dato[i] )
            raiz->nodos[0] = in_delete(raiz->nodos[0], dato, profundidad+1, add);
        else
            raiz->nodos[1] = in_delete(raiz->nodos[1], dato, profundidad+1, add);

    }
    return raiz;
}

void KD_tree::PRINT(queue<nodo *> cola, int n){
    if(n<0)
        return;
    cout << endl;
    queue<nodo *> temp;
    string rellenoX;
    agregar(rellenoX, dimension, "X");
    string relleno;
    agregar(relleno, (2*dimension)+1, "-");

    for(int i=0; i< (pow(2,n)-1); i++)
        cout << relleno;
    while(!cola.empty()){

        if(cola.front()){
            if(cola.front()->datos.size() != 0){
                cout << " ";
                for(auto i : cola.front()->datos)
                    cout << i << " ";
            }
            else
                cout << "["<<rellenoX<<"]";

            for(int i=0; i< (pow(2,n+1)-1); i++)
                cout << relleno ;
            temp.push(cola.front()->nodos[0]);  temp.push(cola.front()->nodos[1]);
        }
        else{
            vector<float> datos;
            temp.push( new nodo(0, datos));        temp.push(new nodo(0, datos));
            for(int i=0; i< (pow(2,n-1)-1); i++)
                cout <<relleno;
            cout << "["<<rellenoX<<"]";//cout << "[null]";
            for(int j=0; j< (pow(2,n+1)-1); j++)
                cout <<relleno;
        }
        cola.pop();
    }
    PRINT(temp,  n-1);
}

void KD_tree::setAltura(vector<nodo *> A){
	    if(A.size()==0)
            return;
	    int i=0;
	    vector<nodo *>  B;
	    while( A.size()>0){
            if(A.at(i)->nodos[0] != nullptr)
                B.push_back(A.at(i)->nodos[0]);
            if(A.at(i)->nodos[1] != nullptr)
                B.push_back(A.at(i)->nodos[1]);
             A.erase(A.begin());
	    }
	    altura++;
	    setAltura(B);
}

int main()
{
    int DIMENSION = 3;
    int nodos = 7;
    float nodo__[][DIMENSION] {{306, 105, 0}, {320, 145, 2}, {505, 22, 1}, {163, 439, 7}, {88, 112, 5}, {216, 117, 10}, {156, 397, 10}};

    vector<vector<float>> vec_; ///pasar los datos a formato vector
    for(size_t i = 0; i<nodos; ++i){
        vector<float> tmp;
        for(size_t j=0; j<DIMENSION; ++j )
            tmp.push_back(nodo__[i][j]);
        vec_.push_back(tmp);
    }

    KD_tree arbol(DIMENSION);
    for(size_t i = 0; i<nodos; ++i)
        arbol.raiz = arbol.insertar(arbol.raiz, vec_[i], 0);

    vector<nodo *> A;   ///para saber altura
    A.push_back(arbol.raiz);
    arbol.setAltura(A);

    vector<float> nodo_borra;
    nodo_borra.push_back(17);
    nodo_borra.push_back(15);
    nodo_borra.push_back(2);
    vector<float> nodoRaiz;
    nodoRaiz.push_back(3);
    nodoRaiz.push_back(6);
    nodoRaiz.push_back(0);

    queue<nodo *> cola;
    cola.push(arbol.raiz);
    arbol.PRINT(cola, arbol.altura-1);
    cout << endl;
    arbol.eliminar(nodoRaiz, 0);
    //arbol.PRINT(cola, arbol.altura-1);

    cout << endl<< endl<< endl<< endl<< endl;
    return 0;
}
