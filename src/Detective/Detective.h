#ifndef PROYECTOESTRUCTURADATOS_DETECTIVE_H
#define PROYECTOESTRUCTURADATOS_DETECTIVE_H

#include <string>
#include <stack> // TAD Pila - para las pistas recolectadas.
#include <iostream>
#include <list>
#include "../Pista/Pista.h"

using std::string;
using std:: stack;
using std:: cout;
using std:: endl;
using std:: list;

class Detective {

private:
    string nombre;
    int fila;
    int columna;
    int puntaje;
    stack<Pista*> pilaPistas; //TAD Pila: la ultima pista hallada es la primera en usarse.

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


#endif