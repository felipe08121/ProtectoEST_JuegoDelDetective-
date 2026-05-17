//
// Created by felip on 17/05/2026.
//

#ifndef PROTECTOEST_JUEGODELDETECTIVE__DETECTIVE_H
#define PROTECTOEST_JUEGODELDETECTIVE__DETECTIVE_H


#include <string>
#include <stack> // TAD Pila - para las pistas recolectadas.
#include <vector> //Solo para el display de la pila, no jugabilidad.
#include <iostream>
#include "../Pista/Pista.h"

using std::string;
using std:: stack;
using std:: cout;
using std:: endl;
using std:: vector;

class Detective {

private:
    string nombre;
    int fila;
    int columna;
    int puntaje;
    stack<Pista*> pilaPistas; //TAD Pila: la ultima pila hallada es la primera en usarse.

public:
    Detective( const string& nombre, int fila, int columna );

    //Getters:
    const string& getNombre() const;
    int getFila() const;
    int getColumna() const;
    int getPuntaje() const;
    bool tienePistas() const;
    int cantidadPistas() const;

    //Setters:

    void setFila( int f );
    void setColumna( int c );
    void setPuntaje( int p );

    //Accion:
    void incrementarPuntaje();

    //Operaciones de Pila:

    void agregarPista( Pista * pista ); //push.
    Pista * verUltimaPista() const; // top (no extrae).
    Pista * sacarUltimaPista(); //pop (devuelve el primero)

    //Comando T: muestra la pila visualmente.
    void mostrarPilaPistas() const;

};


#endif //PROTECTOEST_JUEGODELDETECTIVE__DETECTIVE_H