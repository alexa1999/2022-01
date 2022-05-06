
#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <algorithm>

using namespace std;

int prof;
void setprof(int& profs, int val) { profs = val; }
class Punto {
public:
	int x, y;
	Punto(int _x, int _y) {
		x = _x;
		y = _y;
	}
	Punto() {
		x = 0;
		y = 0;
	}
};


class quadTree {
public:
	Punto topLeft;
	Punto botRight;

	vector <Punto*> cantPuntos;
	vector <quadTree*> sons;    //[0]arribaIzq, [1] arrDerec , [2] abajoIzq, [3]abajoDer
	int cont;

	quadTree() {
		cont = 0;
		for (int i = 0; i < 4; i++) {
			sons.push_back(NULL);
		}
	}

	quadTree(Punto topL, Punto botR) {
		topLeft = topL;
		botRight = botR;
		cont = 0;
		for (int i = 0; i < 4; i++) {
			sons.push_back(NULL);
		}
	}

	void insert(Punto*);
	bool find(Punto);
	bool findPunto(Punto);
	bool enQuad(Punto);
};


bool quadTree::enQuad(Punto p) {
	return (p.x >= topLeft.x &&
		p.x <= botRight.x &&
		p.y <= topLeft.y &&
		p.y >= botRight.y);
}


void quadTree::insert(Punto* PuntoI) {

	//No pertence a este quad
	if (!enQuad(*PuntoI)) {
		cout << "   punto " << PuntoI->x << "," << PuntoI->y << " NO Pertenece a quad" << endl;
		return;
	}
	else {
		///cout<<"push "<<endl;
		if (!findPunto(*PuntoI)) {
			cantPuntos.push_back(PuntoI);
			cont++;
		}
		else {
			if (cantPuntos.size() > prof || cont > prof) {
				for (int i = 0; i < cantPuntos.size(); i++) {
					// si esta en el lado izquierd
					if ((topLeft.x + botRight.x) / 2 >= cantPuntos[i]->x) {   //PuntoI->x
						//Si esta arriba a la izquierda
						if ((topLeft.y + botRight.y) / 2 <= cantPuntos[i]->y) {
							///cout<<"  sons[0] "<<endl;
							if (sons[0] == NULL) { //sons[0]
								sons[0] = new quadTree(Punto(topLeft.x, topLeft.y), Punto((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2));
							}
							sons[0]->insert(cantPuntos[i]);
						}

						// si esta en la izquierda abajo
						else {
							if (sons[2] == NULL) {
								sons[2] = new quadTree(  //sons[2]
									Punto(topLeft.x, (topLeft.y + botRight.y) / 2), Punto((topLeft.x + botRight.x) / 2, botRight.y));
							}
							sons[2]->insert(cantPuntos[i]);
						}
					}
					else {
						// si esta arriba a la derecha
						if ((topLeft.y + botRight.y) / 2 <= cantPuntos[i]->y) {
							///cout<<"  sons[1] "<<endl;
							if (sons[1] == NULL) { //sons[1]
								sons[1] = new quadTree(
									Punto((topLeft.x + botRight.x) / 2, topLeft.y), Punto(botRight.x, (topLeft.y + botRight.y) / 2));
							}
							sons[1]->insert(cantPuntos[i]);
						}

						///si esta abajo a la derecha
						else {
							if (sons[3] == NULL) {
								///cout<<"  sons[3] "<<endl;
								sons[3] = new quadTree( //sons[3]
									Punto((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2), Punto(botRight.x, botRight.y));
							}
							sons[3]->insert(cantPuntos[i]);
						}
					}
				}
				cantPuntos.clear();
			}
		}
	}

}

bool quadTree::findPunto(Punto PuntoI) {
	for (int i = 0; i < cantPuntos.size(); i++) {
		if (cantPuntos[i]->x == PuntoI.x && cantPuntos[i]->y == PuntoI.y) {
			return true;
		}
	}
	return false;
}

bool quadTree::find(Punto PuntoI) {
	if (!enQuad(PuntoI)) {
		cout << "El punto no esta en los limites" << endl;
		return false;
	}
	//si esta en la izquierda
	if ((topLeft.x + botRight.x) / 2 >= PuntoI.x) {

		if ((topLeft.y + botRight.y) / 2 <= PuntoI.y) {   //esta arriba / en sons[0]
			cout << " 0 - iz arriba " << endl;
			if (sons[0] == NULL) {
				return findPunto(PuntoI);
			}
			return sons[0]->find(PuntoI);
		}
		else {                                       //esta abajo / en sons[2]
			cout << " 2 -iz abajo " << endl;
			if (sons[2] == NULL) {
				return findPunto(PuntoI);
			}
			return sons[2]->find(PuntoI);;

		}
	}
	else {  //esta en el lado derecho
		if ((topLeft.y + botRight.y) / 2 <= PuntoI.y) {   //esta arriba / en sons[1]
			cout << " 1 - dere arriba" << endl;
			//printLimites();
			if (sons[1] == NULL) {
				return findPunto(PuntoI);
			}
			return sons[1]->find(PuntoI);;
		}
		else {                                           //esta abajo / en sons[3]
			cout << " 3 - dere abajo" << endl;
			//printLimites();
			if (sons[3] == NULL) {
				return findPunto(PuntoI);
			}
			return sons[3]->find(PuntoI);
		}
	}
};


int main()
{
	int opI;
	int profi = 1;
	int ix, iy, lx, ly, rx, ry;
	cout << "Quadtreee" << endl;
	cout << "Ingrese prof " << endl;
	cin >> profi;
	setprof(prof, profi);


	cout << "Ingresar Limite Izquierdo Arriba " << endl << "x = ";
	cin >> lx;
	cout << "y =";
	cin >> ly;
	cout << "Ingresar Limite derecho Abajo " << endl << "x = ";
	cin >> rx;
	cout << "y =";
	cin >> ry;
	quadTree root(Punto(lx, ly), Punto(rx, ry));
	while (true) {
		cout << " 1. Ingresar punto" << endl;
		cout << " 2. Buscar punto" << endl;
		cout << " 3. Exit" << endl;
		cin >> opI;
		if (opI == 1) {
			cout << "Ingresar x ";
			cin >> ix;
			cout << "Ingresar y ";
			cin >> iy;
			root.insert(new Punto(ix, iy));
		}
		if (opI == 2) {
			cout << "Punto a Buscar :" << endl;
			cout << "Ingresar x ";
			cin >> ix;
			cout << "Ingresar y ";
			cin >> iy;
			root.find(Punto(ix, iy));
		}
		if (opI == 3) {
			break;
		}
	}
	return 0;
}