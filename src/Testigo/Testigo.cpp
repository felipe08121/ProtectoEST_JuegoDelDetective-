//
// Created by felip on 17/05/2026.
//

#include "Testigo.h"

Testigo:: Testigo( int fila, int columna ) {
    this->fila = fila;
    this->columna = columna;
    this->procesado = false;
}

//GETTERS:

int Testigo:: getFila() const{
    return this->fila;
}

int Testigo:: getColumna() const {
    return this->columna;
}

bool Testigo:: isProcesado() const {
    return this->procesado;
}

void Testigo:: setFila( int f ) {
    this->fila = f;
}

void Testigo:: setColumna( int c ) {
    this->columna = c;
}

void Testigo:: setProcesado( bool p ) {
    this->procesado = p;
}