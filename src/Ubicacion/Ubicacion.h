//
// Created by felip on 17/05/2026.
//

#ifndef PROTECTOEST_JUEGODELDETECTIVE__UBICACION_H
#define PROTECTOEST_JUEGODELDETECTIVE__UBICACION_H

#include <string>

using std::string;

enum class TipoUbicacion{
    EDIFICIO, //Borde del mapa: '#' - siempre visible
    CALLEJON, // Paso bloqueado: '|' visible sola al intentar entrar
    CALLE //Interior transitable: 'o' sin descubirir, ' ' descubierto
}; // Sin enum, tendria que representar eso con algo como un int (0, 1, 2).

class Ubicacion {
private:
    int fila;
    int columna;
    TipoUbicacion tipo;
    bool descubierto; // para saber si fue visitado o revelado.
    bool tienePista; //Para saber si hay una pista oculta aqui.
    bool tieneTestigo;// Para saber si hay un testigo por aca.

    //Punteros de la lista multiplemente enlazada:
    Ubicacion * arriba;
    Ubicacion * abajo;
    Ubicacion * izquierda;
    Ubicacion * derecha;

public:

    Ubicacion( int fila, int columna, TipoUbicacion tipo );

    //Getters:
    int getFila() const;
    int getColumna() const;
    TipoUbicacion getTipo() const;
    bool isDescubierto() const;
    bool getTienePista() const;
    bool getTieneTestigo() const;

    Ubicacion * getArriba() const;
    Ubicacion * getAbajo() const;
    Ubicacion * getIzquierda() const;
    Ubicacion * getDerecha() const;

    //Setter:
    void setTipo( TipoUbicacion t );
    void setDescubierto( bool d );
    void setTienePista( bool p );
    void setTieneTestigo( bool w );

    void setArriba( Ubicacion * u );
    void setAbajo( Ubicacion * u );
    void setIzquierda( Ubicacion * u );
    void setDerecha( Ubicacion * u);

    // Métodos auxiliares:

    char getSimbolo() const;//Simbolo a mostrar en el tablero.
    bool esTransitable() const; //Para saber si el dectective se puede mover aqui.

};

#endif //PROTECTOEST_JUEGODELDETECTIVE__UBICACION_H