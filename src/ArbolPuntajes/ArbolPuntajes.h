//
// Created by felip on 16/05/2026.
//

#ifndef PROYECTOESTRUCTURADATOS_ARBOLPUNTAJES_H
#define PROYECTOESTRUCTURADATOS_ARBOLPUNTAJES_H

#include <iostream>
#include <string>
#include <utility>
#include <list>
#include "../RankingJson/RankingJson.h"

using std:: string;
using std:: cout;
using std:: endl;
using std:: list;


//Nodo del ABB -- cada nodo guarda un detective y su mejor puntaje.

struct NodoABB{
    string nombre;
    int puntaje;

    NodoABB* izq;
    NodoABB* der;

    NodoABB( const string& nombre, int puntaje );
};

//ABB implementado desde cero para el ranking historico
//ordenado por puntaje: Recorrido inOrden da de menor a mayor.
// La persistencia la delega a RankingJson:


class ArbolPuntajes {

private:
    NodoABB * raiz;
    RankingJson rankingJson; //Maneja la lectura/escritura del archivo.

    //Metodos iterativos:

    NodoABB * insertar( NodoABB* raiz, const string& nombre, int puntaje );
    NodoABB * encontrarMinimo( NodoABB * nodo ) const;


    //Metodos recursivos (requieren recorres ambas ramas o reconstrui camino):
    NodoABB * eliminar ( NodoABB* raiz, const string& nombre, int puntaje );
    NodoABB * buscarPorNombre( NodoABB* nodo, const string& nombre ) const;
    void inorden ( NodoABB* nodo, int& posicion ) const;
    void liberarMemoria( NodoABB* nodo );
    void recolectarInorden( NodoABB * nodo,
                            list<pair<string, int>>& entradas ) const;

public:

    ArbolPuntajes();
    ~ArbolPuntajes();

    //Registra puntaje: si ya existe, se conserva el mejor/menor.
    void registrarPuntaje( const string& nombre, int puntaje );

    //Busca detective por nombre y devuelve su mejor puntaje.
    bool buscarDetective( const string& nombre, int& mejorPuntaje ) const;

    //Muestra todos los detectives de menor a mayor puntaje:
    void mostrarRanking() const;

    bool estaVacio() const;
};


#endif //PROYECTOESTRUCTURADATOS_ARBOLPUNTAJES_H