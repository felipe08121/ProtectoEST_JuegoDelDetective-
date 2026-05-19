#ifndef PROYECTOESTRUCTURADATOS_SOSPECHOSO_H
#define PROYECTOESTRUCTURADATOS_SOSPECHOSO_H

#include <vector>
#include <string>
#include <algorithm>

using std:: vector;
using std:: string;
using std:: find;

class Sospechoso {

private:
 string nombre;
 vector<string> atributos; //ej: estatura, cabello, piel, sexo, lateralidad.

public:
 Sospechoso( const string& nombre, const vector<string>& atributos );

 //Getters:

 const string& getNombre() const;
 const vector<string>& getAtributos() const;

 //Auxiliar:

 bool tieneAtributo( const string& atributo ) const;


};


#endif 