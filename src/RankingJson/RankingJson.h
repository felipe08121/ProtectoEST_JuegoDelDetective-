//
// Created by felip on 16/05/2026.
//

#ifndef PROYECTOESTRUCTURADATOS_RANKINGJSON_H
#define PROYECTOESTRUCTURADATOS_RANKINGJSON_H

#include <string>
#include <utility> // Para pair.
#include <fstream>
#include <iostream>
#include <list>

using std:: string;
using std:: pair;
using std:: ifstream;
using std:: ofstream;
using std:: cout;
using std:: endl;
using std:: getline;
using std:: stoi;
using std:: list;

// Clase responsable de leer y escribir ranking.json
// Una entrada por linea, sin conrchetes ni comas entre objetos.

class RankingJson {

private:
    string rutaArchivo;

    //Auxiliares para parsear cada linea JSON manualmente:

    string extraerString( const string& linea, const string& clave ) const;
    int extraerInt( const string& linea, const string& clave ) const;

public:

    RankingJson( const string& rutaArchivo );

    // Verifica si existe y tiene contenido.
    bool existeYNoEstaVacio() const;

    //lee el archivo y devuelve todas las entradas.
    list< pair <string, int> > cargar() const;

    //Sobreescribe el archivo con todas las entradas recibidas.
    void guardar( const list< pair<string, int> >& entradas ) const;

};

#endif //PROYECTOESTRUCTURADATOS_RANKINGJSON_H