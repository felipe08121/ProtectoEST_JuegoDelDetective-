//
// Created by felip on 16/05/2026.
//

#include "RankingJson.h"

//Constructor:

RankingJson:: RankingJson( const string& rutaArchivo ) {
    this->rutaArchivo = rutaArchivo;
}

//Verificar si el archivo existe y no esta vacio.


bool RankingJson:: existeYNoEstaVacio() const {
    ifstream archivo( this-> rutaArchivo );

    if ( !archivo.is_open() ) {
        return false; // El archivo no existe.
    }

    //Intenta leer al menos un caracter para si tiene contenido.

    char c;
    bool tieneContenido = ( archivo.get( c ) ) ? true: false;

    archivo.close();
    return tieneContenido;
}

//Auxiliares para parsear JSON manualmente:
//Formato de cada linea:
//{"nombre": "Holmes","puntaje":23}


//Extrae el valor de clave tipo string: "clave":"valor":

string RankingJson:: extraerString( const string& linea, const string& clave) const {
    string buscar = "\"" + clave + "\":\"";
    size_t inicio  = linea.find( buscar );
    if ( inicio == string:: npos ) {
        return "";
    }

    inicio += buscar.size();
    size_t fin = linea.find( "\"", inicio );

    if ( fin == string:: npos ) {
        return "";
    }

    return linea.substr( inicio,  fin - inicio );
}

int RankingJson:: extraerInt( const string& linea, const string& clave ) const {

    string buscar = "\"" + clave + "\":";
    size_t inicio = linea.find( buscar );

    if ( inicio == string:: npos ) {
        return -1;
    }

    inicio += buscar.size();

    //El numero termina en ',' o '}':

    size_t fin = linea.find_first_of(",}", inicio );
    if ( fin == string:: npos ) {
        fin = linea.size();
    }

    try {
        return stoi( linea.substr( inicio, fin - inicio ) );
    }
    catch ( ... ) {
        return -1; //Linea corrupta -- retorna -1.
    }

}

//Cargar - lee el archivo y devuelve las entradas:

list < pair< string, int> > RankingJson:: cargar() const {
    list <pair<string,int>> entradas;

    if ( !existeYNoEstaVacio() ) {
        return entradas; //Archivo inexistente o vacio -- devolver vacio.
    }

    ifstream archivo( this->rutaArchivo );

    if ( !archivo.is_open() ) {
        return entradas;
    }

    string linea;
    while ( getline( archivo, linea) ) {


        //Ignorar lineas vacias.
        if ( linea.empty() ) {
            continue;
        }

        //Solo procesar lineas que parecen un objeto JSON:
        if ( linea.front() != '{' ) {
            continue;
        }

        string nombre = extraerString( linea, "nombre");
        int puntaje = extraerInt( linea, "puntaje" );

        //Solo agregar si ambos valores son validos:
        if ( !nombre.empty() && puntaje >= 0 ) {
            entradas.push_back( { nombre, puntaje } );
        }
    }

    archivo.close();
    return entradas;
}

//Guardar -- Sobreescribe el archivo con las entradas:

void RankingJson:: guardar( const list<pair<string, int>>& entradas ) const {
    ofstream archivo( this->rutaArchivo );

    if ( !archivo.is_open() ) {
        cout << endl <<" Error: no se pudo abrir ranking.json para guardar." << endl;
        return;
    }

    //Si no hay entradas el archivo queda vacio (no escribir nada):
    for ( const pair<string, int>& entrada: entradas ) {
        archivo << "{\"nombre\":\"" << entrada.first
                << "\",\"puntaje\":" << entrada.second << "}" << endl;
    }
    archivo.close();
}