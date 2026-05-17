//
// Created by felip on 17/05/2026.
//

#ifndef PROTECTOEST_JUEGODELDETECTIVE__TESTIGO_H
#define PROTECTOEST_JUEGODELDETECTIVE__TESTIGO_H

//Testigo: Aparece en el mapa con W.
//Al pisarlo, entra a una Cola de declaraciones
//( TAD Cola - Fase 2)


class Testigo {

private:
    int fila;
    int columna;
    bool procesado; //true = ya fue agregado a la cola.

public:
    Testigo( int fila, int columna);

    //Getters:
    int getFila() const;
    int getColumna() const;
    bool isProcesado() const;

    //Setters:

    void setFila( int f );
    void setColumna( int c );
    void setProcesado( bool p );

};

#endif //PROTECTOEST_JUEGODELDETECTIVE__TESTIGO_H