#ifndef PROYECTOESTRUCTURADATOS_SOSPECHOSO_H
#define PROYECTOESTRUCTURADATOS_SOSPECHOSO_H

#include <string>
#include <algorithm>
#include <list>


using std:: string;
using std:: find;
using std:: list;

class Sospechoso {

private:
 string nombre;
 list<string> atributos; //ej: estatura, cabello, piel, sexo, lateralidad.

public:
 Sospechoso( const string& nombre, const list<string>& atributos );

 //Getters:

 const string& getNombre() const;
 const list<string>& getAtributos() const;

 //Auxiliar:

 bool tieneAtributo( const string& atributo ) const;


};

#endif 