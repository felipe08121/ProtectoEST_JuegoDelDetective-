//
// Created by felip on 17/05/2026.
//

#include "Mapa.h"

//Constructor - construye la lista multiplemente enlazada:

Mapa:: Mapa() {
    this->cabeza = nullptr;
    this->rng = mt19937( random_device{}() );

    //Paso 1: Crear Nodos con vector temporal SOLO para el enlazada inicial.
    //Una vez enlazados, el vector se descarta; el gameplay usa solo la lista.

    vector< vector <Ubicacion*>> temp( TOTAL_FILAS, vector<Ubicacion*>(TOTAL_COLUMNAS, nullptr));

    for ( int i = 0; i < TOTAL_FILAS; i++ ) {
        for ( int j = 0; j < TOTAL_COLUMNAS; j++ ) {
            bool esBorde = ( i == 0 || i == TOTAL_FILAS - 1  ||
                             j == 0 || j == TOTAL_COLUMNAS -1 );
            TipoUbicacion t = esBorde ? TipoUbicacion::EDIFICIO
                                      : TipoUbicacion::CALLE;
            temp[ i ][ j ] = new Ubicacion( i, j, t );
        }
    }

    //Paso 2: Enlazar la lista multiplemente enlazada:
    for (int i = 0; i < TOTAL_FILAS; i++ ) {
        for ( int j = 0; j < TOTAL_COLUMNAS; j++ ) {
            if ( i > 0 ) {
                temp[ i ][ j ]->setArriba( temp[ i-1 ][ j ] );
            }
            if ( i < TOTAL_FILAS - 1 ) {
                temp[ i ][ j ]->setAbajo( temp[ i + 1 ][ j ] );
            }
            if ( j > 0 ) {
                temp[ i ][ j ] -> setIzquierda( temp[ i ][ j-1 ]);
            }

            if ( j < TOTAL_COLUMNAS - 1 ) {
                temp[ i ][ j ]->setDerecha( temp[ i ][ j+1 ] );
            }
        }
    }
    this->cabeza = temp[ 0 ][ 0 ];
    //temp sale de alcance aqui - desde ahora solo existe la lista enladada.

    //Paso 3: Colocar elementos aleatorios:

    colocarCallejones();
    colocarPistas();
    colocarTestigos();

}

//Destructor - Libera todos los nodos recorriendo la lista:

Mapa :: ~Mapa() {

    Ubicacion * filaPtr = this->cabeza;

    while ( filaPtr ) {
        Ubicacion* sigFila = filaPtr->getAbajo();
        Ubicacion* n = filaPtr;
        while ( n ) {
            Ubicacion* sig = n ->getDerecha();
            delete n;
            n = sig;
        }
        filaPtr = sigFila;
    }

    for ( Pista * p : this-> pistas ) {
        delete p;
    }

    for ( Testigo * t : this-> testigos ) {
        delete t;
    }
}

//getNodo - transeversal puro: O( fila + columna):

Ubicacion * Mapa:: getNodo( int fila, int columna ) const {

    if ( fila < 0 || fila >= TOTAL_FILAS || columna < 0 || columna >= TOTAL_COLUMNAS ) {
        return nullptr;
    }
    Ubicacion * n = this->cabeza;
    for ( int i =0; i < fila; i++ ) {
        n = n ->getAbajo();
    }
    for ( int j = 0; j < columna; j++ ) {
        n = n-> getDerecha();
    }
    return n;
}

//Colacion aleatoria de elementos:

void Mapa:: colocarCallejones() {

    uniform_int_distribution<int> dist( INTERIOR_MIN, INTERIOR_MAX );
    int colocados = 0;

    while ( colocados < NUM_CALLEJONES ) {
        int f = dist( this->rng ), c = dist( rng );
        Ubicacion* n = getNodo( f,c );
        if ( n && n->getTipo() == TipoUbicacion::CALLE && !n->getTienePista() && !n->getTieneTestigo() ) {
            n->setTipo( TipoUbicacion::CALLEJON );
            colocados++;
        }
    }
}

void Mapa:: colocarPistas() {
    uniform_int_distribution< int > dist( INTERIOR_MIN, INTERIOR_MAX );
    uniform_int_distribution< int > tipoDist( 0, 3 );

    const TipoPista tipos[ 4 ] = {
        TipoPista:: HUELLA , TipoPista::COARTADA,
        TipoPista:: TESTIMONIO, TipoPista:: PRUEBA_FORENSE
    };

    int colocadas = 0;

    while ( colocadas < NUM_PISTAS ) {
        int f = dist( this->rng ), c = dist( this->rng );
        Ubicacion * n = getNodo( f, c );
        if ( n && n->getTipo() == TipoUbicacion::CALLE
               && !n->getTienePista() && !n-> getTieneTestigo() ) {
            this->pistas.push_back( new Pista( tipos[tipoDist( this-> rng )], f, c ));
            n->setTienePista( true );
            colocadas++;
        }
    }
}