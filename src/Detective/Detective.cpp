#include "Detective.h"

Detective :: Detective (const string& nombre, int fila, int columna ) {
    this->nombre = nombre;
    this->fila = fila;
    this->columna = columna;
    this->puntaje = 0;
}

//Getters:

const string& Detective:: getNombre() const{
    return this->nombre;
}

int Detective:: getFila() const {
    return this->fila;
}

int Detective:: getColumna() const {
    return this->columna;
}

int Detective:: getPuntaje() const {
    return this-> puntaje;
}

bool Detective:: tienePistas() const {
    return !this->pilaPistas.empty();
}

int Detective:: cantidadPistas() const {
    return (int)pilaPistas.size();
}

//Setters:

void Detective:: setFila( int f ) {
    this->fila = f;
}

void Detective:: setColumna( int c ) {
    this->columna = c;
}

void Detective:: setPuntaje(int p ) {
    this->puntaje = p;
}

//Accion:

void Detective:: incrementarPuntaje() {
    this->puntaje++;
}

//Operaciones de Pila:

void Detective:: agregarPista( Pista* pista) {
    this->pilaPistas.push(pista);
}

Pista * Detective:: verUltimaPista() const {
    return this->pilaPistas.empty() ? nullptr : pilaPistas.top();
}

Pista * Detective:: sacarUltimaPista() {
    if ( this->pilaPistas.empty() ) {
        return nullptr;
    }

    Pista * p = this->pilaPistas.top();
    this->pilaPistas.pop();
    return p;
}

//Comando T de mostrar toda la pila:

void Detective::mostrarPilaPistas() const {
    cout << endl << " " << this->nombre << ", mira las pistas que llevas: " << endl;
    if ( this->pilaPistas.empty() ) {
        cout << " [ Sin pistas recolectadas aun ] " << endl;
        return;
    }

    //Copiar la pilas para iterar sin destruirla.
    //El vector es solo para mostrar (display)

    stack <Pista*> copia = pilaPistas;
    vector <Pista*> elems;

    while ( !copia.empty() ) {
        elems.push_back( copia.top() );
        copia.pop();
    }

    // Celdas vacias en la cima visual de la pila
    cout << " ['#     #']" << endl;
    cout << " ['#     #']" << endl;

    for ( int i = 0; i < (int)elems.size(); i++ ) {
        string etiqueta;
        if ( i == 0 ) {
            etiqueta = " <- ultima (usar con X)";
        }
        else if ( i == 1 ) {
            etiqueta = " <- penultima";
        }
        else if ( i == 2 ) {
            etiqueta = " <- antepenultima";
        }
        cout << " ['#  " << elems[ i ]->getLetra()
             << "  #']" << etiqueta << endl;
    }
    // Base de la pila
    cout << " ['# # # #']" << endl;
}