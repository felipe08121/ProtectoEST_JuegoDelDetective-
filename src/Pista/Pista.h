//
// Created by felip on 17/05/2026.
//

#ifndef PROTECTOEST_JUEGODELDETECTIVE__PISTA_H
#define PROTECTOEST_JUEGODELDETECTIVE__PISTA_H
#include <string>

using std::string;

enum class TipoPista {
    HUELLA, //H - divide el puntaje a la mitad.
    COARTADA, // C - elimina 2 callejones.
    TESTIMONIO, // T - puntaje a 0 o 2X (aleatorio).
    PRUEBA_FORENSE // P - Teletransporta al detective.
};

class Pista {

private:
    TipoPista tipo;
    int fila;
    int columna;

public:
    Pista( TipoPista tipo, int fila, int columna);
    //Getters:
    TipoPista getTipo() const;
    int getFila() const;
    int getColumna() const;

    //Setters:

    void setFila( int f );
    void setColumna( int c );

    //Funciones Auxiliares:
    string getTipoStr() const; //Para obtener el nombre completo.
    char getLetra() const; // H/C/T/P
};



#endif //PROTECTOEST_JUEGODELDETECTIVE__PISTA_H