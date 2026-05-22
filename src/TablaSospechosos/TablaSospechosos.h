#ifndef PROYECTOESTRUCTURADATOS_TABLASOSPECHOSOS_H
#define PROYECTOESTRUCTURADATOS_TABLASOSPECHOSOS_H

#include <string>
#include <list>
#include <unordered_map> //TAD: Tabla Hash - Busqueda O(1) promedio
#include <random>
#include <iostream>
#include <algorithm>
#include "../Sospechoso/Sospechoso.h"

using std::string;
using std:: list;
using std::unordered_map;
using std::mt19937;
using std:: cout;
using std:: pair;
using std:: advance;
using std:: uniform_int_distribution;
using std:: random_device;
using std:: endl;

class TablaSospechosos {

    //Constante: numero de sospechosos al inicio de la partida.

public:
    static const int NUM_SOSPECHOSOS = 8;

private:
    unordered_map<string, Sospechoso*> tabla; //Tabla Hash O(1) buesqueda.
    Sospechoso* culpable; // uno de los 8, marcado al azar.
    mt19937 rng;

    //Metodo interno para crear los 8 sospechosos al inicio.
    void generarSospechosos();

public:
    TablaSospechosos();
    ~TablaSospechosos();

    //Busqueda O(1) promedio:
    Sospechoso* buscar( const string& nombre ) const;

    //Acusar: usar buscar() entonces tambien O(1):
    bool acusar( const string& nombre ) const;

    Sospechoso * getCulpable() const;
    const unordered_map<string, Sospechoso*>& getTabla() const;

    //Muestra la tabla con los atributos del culpable revelados hasta ahora.:
    void mostrar( const list<string>& atributosRevelados, const string& nombreDetectives ) const;



};


#endif 