#ifndef PROYECTOESTRUCTURADATOS_MAPA_H
#define PROYECTOESTRUCTURADATOS_MAPA_H

#include <list>
#include <random>
#include <utility>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "../Ubicacion/Ubicacion.h"
#include "../Pista/Pista.h"
#include "../Testigo/Testigo.h"

using std:: cout;
using std:: endl;
using std:: list;
using std:: pair;
using std::uniform_int_distribution;
using std:: vector;
using std:: mt19937;
using std:: random_device;
using std:: shuffle;
using std:: min;


class Mapa {

    // Este public es para constantes del mapa:
public:
    static const int TOTAL_FILAS = 11;
    static const int TOTAL_COLUMNAS = 11;
    static const int INTERIOR_MIN = 1; //Indice interior minimo.
    static const int INTERIOR_MAX = 9; //Indice interior maximo.
    static const int NUM_CALLEJONES = 16;
    static const int NUM_PISTAS = 10;
    static const int NUM_TESTIGOS = 5;


private:

    Ubicacion * cabeza; //nodo( 0,0) - Entrada a la lista.
    list< Pista* > pistas; //10 pistas (Mapa es dueño)
    list< Testigo* > testigos; // 5 testigos (Mapa es dueño)
    mutable mt19937 rng;

    //Transversal puro de la lista enlazada -0 (fila+col), ~O(1) en mapa 11x11.
    Ubicacion * getNodo( int fila, int columna ) const;

    //Metodos:
    void colocarCallejones();
    void colocarPistas();
    void colocarTestigos();

//Public para los metodos:

public:
    Mapa();
    ~Mapa();

    //Posicionamiento:

    pair< int, int > posicionInicialDetective();

    //Busca celda CALLE no descubierta (para prueba forense):
    pair < int , int > posicionNoCubiertaNiCallejon( int exceptFila, int exceptCol );

    //Navegacion:
    //Devuelve el nodo vecino en la direccion dada (W/A/S/D)
    Ubicacion * getNodoVecino( int fila, int col, char direccion ) const;

    //Acciones:
    void marcarDescubierto( int fila, int c );

    Pista* recogerPistaEn( int fila, int columna );
    Testigo* getTestigoEn( int fila, int columna ) const;
    void removerTestigoEn( int fila, int columna );

    void reposicionarPista( Pista * pista ); //Reubica en celda libre.
    void resetVisibilidad(); //Cubre todo con 'o'.
    int eliminarCallejonesAleatorios( int n );

    bool hayPistaEn( int fila, int columna ) const;
    bool hayTestigoEn( int fila, int columna ) const;

    //Display:
    void mostrar( int detFila, int detCol ) const;

    const list<Pista*>& getPistas() const;
    const list<Testigo*>& getTestigos() const;

};


#endif //PROYECTOESTRUCTURADATOS_MAPA_H