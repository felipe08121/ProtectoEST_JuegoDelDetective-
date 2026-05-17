//
// Created by felip on 17/05/2026.
//

#include "Ubicacion.h"

Ubicacion:: Ubicacion( int fila, int columna, TipoUbicacion tipo) {
    this->fila = fila;
    this->columna = columna;
    this->tipo = tipo;
    this-> descubierto = tipo == TipoUbicacion::EDIFICIO;
    this->tienePista = false;
    this->tieneTestigo = false;

    this-> arriba = nullptr;
    this->abajo = nullptr;
    this->izquierda = nullptr;
    this ->derecha = nullptr;
}

//Getter:

int Ubicacion::getFila() const {
    return this->fila;
}

int Ubicacion:: getColumna() const {
    return this->columna;
}

TipoUbicacion Ubicacion:: getTipo() const {
    return this->tipo;
}

bool Ubicacion::isDescubierto() const{
return this->descubierto;
}

bool Ubicacion:: getTienePista() const {
    return this->tienePista;
}

bool Ubicacion:: getTieneTestigo() const {
    return this-> tieneTestigo;
}

Ubicacion * Ubicacion:: getArriba() const {
    return this->arriba;
}

Ubicacion * Ubicacion:: getAbajo() const {
    return this->abajo;
}

Ubicacion * Ubicacion:: getIzquierda() const {
    return this->izquierda;
}
Ubicacion * Ubicacion:: getDerecha() const {
    return this->derecha;
}

//Setters:

void Ubicacion :: setTipo( TipoUbicacion t ) {
    this-> tipo = t;
}

void Ubicacion:: setDescubierto( bool d ) {
    this->descubierto = d;
}

void Ubicacion:: setTienePista( bool p ) {
    this->tienePista = p;
}

void Ubicacion:: setTieneTestigo( bool w ) {
    this->tieneTestigo = w;
}

void Ubicacion:: setArriba( Ubicacion * u ) {
    this-> arriba = u;
}

void Ubicacion:: setAbajo( Ubicacion * u ) {
    this->abajo = u;
}
void Ubicacion:: setIzquierda( Ubicacion * u ) {
    this->izquierda = u;
}

void Ubicacion:: setDerecha( Ubicacion * u ) {
    this->derecha = u;
}

//Metodos Auxiliares:

char Ubicacion:: getSimbolo() const {
    switch ( this->tipo ) {
        case TipoUbicacion:: EDIFICIO:
            return '#';

        case TipoUbicacion:: CALLEJON:
            //Solo visible si el dectective intento entrar.
            return this->descubierto ? '|' : 'o';

        case TipoUbicacion:: CALLE:
            if ( !descubierto ) {
                return 'o';
            }
            if ( tieneTestigo ) {
                return 'W';
            }
            return ' ';
    }

    return '?';
}

bool Ubicacion:: esTransitable() const {
    return tipo == TipoUbicacion::CALLE;
}