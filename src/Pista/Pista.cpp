//
// Created by felip on 17/05/2026.
//

#include "Pista.h"

Pista:: Pista( TipoPista tipo, int fila, int columna ) {
    this->tipo = tipo;
    this->fila = fila;
    this->columna = columna;
}

//GETTERS:

TipoPista Pista:: getTipo() const{
    return this->tipo;
}

int Pista:: getFila() const {
    return this->fila;
}

int Pista:: getColumna() const {
    return this->columna;
}

//Setters:

void Pista:: setFila( int f ) {
    this->fila = f;
}

void Pista:: setColumna( int c ) {
    this->columna = c;
}

//Metodos Auxiliares:

string Pista:: getTipoStr() const {

    switch ( this->tipo ) {
        case TipoPista:: HUELLA :
        {
            return "Huella";
        }
        case TipoPista::COARTADA: {
            return "Coartada";
        }
        case TipoPista::TESTIMONIO: {
            return "Testimonio";
        }
        case TipoPista::PRUEBA_FORENSE: {
            return "Prueba Forense";
        }
    }
    return "Desconocida";
}

char Pista:: getLetra() const {

    switch ( this->tipo ) {
        case TipoPista::HUELLA:
            return 'H';

        case TipoPista:: COARTADA:
            return 'C';


        case TipoPista:: TESTIMONIO:
            return 'T';


        case TipoPista:: PRUEBA_FORENSE:
            return 'P';
    }
    return '?';
}