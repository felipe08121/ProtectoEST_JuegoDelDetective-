
#include "Sospechoso.h"

Sospechoso :: Sospechoso( const string& nombre, const vector <string>& atributos ) {
    this->nombre = nombre;
    this->atributos = atributos;
}

//Getter:

const string& Sospechoso:: getNombre() const {
    return this->nombre;
}

const vector<string>& Sospechoso:: getAtributos() const {
    return this->atributos;
}

//Auxiliar:

bool Sospechoso:: tieneAtributo( const string& atributo ) const {
    return find( this->atributos.begin(), this->atributos.end(), atributo ) != this->atributos.end();
}