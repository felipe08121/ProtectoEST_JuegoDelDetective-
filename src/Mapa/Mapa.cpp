#include "Mapa.h"

//Constructor - construye la lista multiplemente enlazada:

Mapa:: Mapa() {
    cout << endl << "Entrando a la inicializacion del mapa." << endl;
    this->cabeza = nullptr;
    this->rng = mt19937( random_device{}() );

    //Construimos la lista fila por fila.
    //nodoAnteriorFila apunta al primer nodo de la fila recien contruida.
    //y los usamos para enlazar los nodos arriba/abajo de la siguiente fila.

    Ubicacion * nodoAnteriorFila = nullptr;

    for ( int i = 0; i < TOTAL_FILAS; i++ ) {
        Ubicacion * primerNodoActual = nullptr; //Primer nodo de la fila actual.
        Ubicacion * nodoIzq = nullptr; //Nodo anterior en la misma fila.
        Ubicacion * nodoArriba = nodoAnteriorFila; // Recorre la fila anterior para enlace (arriba/abajo).

        for ( int j = 0; j < TOTAL_COLUMNAS; j++ ) {
            bool esBorde = ( i == 0 || i == TOTAL_FILAS - 1 ||
                             j == 0 || j == TOTAL_COLUMNAS - 1 );
            TipoUbicacion t = esBorde ? TipoUbicacion :: EDIFICIO : TipoUbicacion::CALLE;

            Ubicacion * nuevo = new Ubicacion( i, j, t );

            //Enlace horizontal (izquierda <--> derecha):
            if ( nodoIzq ) {
                nodoIzq->setDerecha( nuevo );
                nuevo->setIzquierda( nodoIzq );
            }

            //Enlace vertical ( Arriba / Abajo):
            if ( nodoArriba ) {
                nodoArriba -> setAbajo( nuevo );
                nuevo->setArriba( nodoArriba );
                nodoArriba = nodoArriba->getDerecha(); // avanzar en la fila anterior.
            }

            if ( j == 0) {
                primerNodoActual = nuevo;
            }
            nodoIzq = nuevo;
        }

        if ( i == 0) {
            this->cabeza = primerNodoActual;
        }

        nodoAnteriorFila= primerNodoActual;// Para la siguiente fila
    }

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
        int f = dist( this->rng ), c = dist( this->rng );
        Ubicacion* n = getNodo( f,c );
        if ( n && n->getTipo() == TipoUbicacion::CALLE && !n->getTienePista() && !n->getTieneTestigo() ) {
            n->setTipo( TipoUbicacion::CALLEJON );
            colocados++;
        }
    }
}

void Mapa:: colocarPistas() {
    uniform_int_distribution< int > dist( INTERIOR_MIN, INTERIOR_MAX );


    list <TipoPista> tipos = {
        TipoPista:: HUELLA , TipoPista::COARTADA,
        TipoPista:: TESTIMONIO, TipoPista:: PRUEBA_FORENSE
    };

    //Distribucion para elegir tipo aleatorio: (0 a 3 por cada tipo de TipoPista):
    uniform_int_distribution<int> tipoDist( 0, (int) tipos.size() - 1 );

    int colocadas = 0;

    while ( colocadas < NUM_PISTAS ) {
        int f = dist( this->rng ), c = dist( this->rng );
        Ubicacion * n = getNodo( f,c );
        if ( n && n->getTipo() == TipoUbicacion::CALLE
               && !n->getTienePista() && !n->getTieneTestigo() )
        {
            //Elegir tipo aleatorio con advance:
            auto it = tipos.begin();
            advance( it, tipoDist( this->rng ) );

            this->pistas.push_back( new Pista( *it, f, c ) );
            n->setTienePista( true );
            colocadas++;
        }
    }
}

void Mapa:: colocarTestigos() {
    uniform_int_distribution<int> dist ( INTERIOR_MIN, INTERIOR_MAX );
    int colocados = 0;
    while ( colocados < NUM_TESTIGOS ) {
        int f = dist( this->rng ), c = dist( this->rng );
        Ubicacion * n = getNodo( f, c );
        if ( n  && n->getTipo( ) == TipoUbicacion::CALLE && !n->getTienePista() && !n->getTieneTestigo() ) {
            this->testigos.push_back( new Testigo( f, c ) );
            n->setTieneTestigo( true );
            colocados++;
        }
    }
}

//Posiciones aleatorias:

pair <int, int> Mapa:: posicionInicialDetective() {
    uniform_int_distribution<int> dist( INTERIOR_MIN, INTERIOR_MAX );
    while ( true ) {
        int f = dist ( this-> rng ), c = dist( this->rng );
        Ubicacion * n = getNodo( f, c );
        if ( n && n->getTipo() == TipoUbicacion:: CALLE && !n->getTienePista() && !n->getTieneTestigo() ) {
            n->setDescubierto( true );
            return {f, c }; // <-- Retorna un par de valores.
        }
    }
}

pair <int , int > Mapa:: posicionNoCubiertaNiCallejon( int exceptFila, int exceptCol ) {
    uniform_int_distribution<int> dist ( INTERIOR_MIN, INTERIOR_MAX);
    for ( int intentos = 0; intentos < 500; intentos++) {
        int f = dist( this->rng ), c = dist(this->rng );
        if ( f == exceptFila && c == exceptCol  ) {
            continue; // si 'f' y 'c' llegan a ser los valores excluidos, salto ese ciclo.
        }
        Ubicacion * n = getNodo( f, c );
        if ( n && n->getTipo() == TipoUbicacion:: CALLE && !n->isDescubierto () ) {
            return { f, c };
        }
    }
    return {exceptFila, exceptCol } ;// si después de varios intentos no encuentra una posición válida,
    //devuelve la posición original como salida de emergencia.
}

//Navegacion:

Ubicacion * Mapa:: getNodoVecino( int fila, int col, char direccion ) const {
    Ubicacion * n = getNodo( fila, col );
    if ( !n ) {
        return nullptr;
    }

    char d = (char) toupper( direccion ); //Si el caracter esta en minuscula lo convierte a mayuscula.
    if ( d == 'W' ) {
        return n->getArriba();
    }
    if ( d == 'S' ) {
        return n->getAbajo();
    }
    if ( d == 'A' ) {
        return n->getIzquierda();
    }
    if ( d == 'D' ) {
        return n->getDerecha();
    }
    return nullptr;
}

// Acciones sobre el mapa_

void Mapa:: marcarDescubierto( int fila, int col ) {
    Ubicacion * n = getNodo( fila, col );
    if ( n ) {
        n->setDescubierto( true );
    }
}

Pista * Mapa:: recogerPistaEn( int fila, int columna ) {
    Ubicacion * n = getNodo( fila, columna );
    if ( !n || !n->getTienePista() ) {
        return nullptr;
    }
    for ( Pista * p : this->pistas ) {
        if ( p->getFila() == fila && p->getColumna() == columna ) {
            n->setTienePista( false );
            //Nota: El mapa sigue siendo dueño del objeto Pista.
            //El detective solo recibe el puntero (relacion de prestamo).
            return p;
        }
    }
    return nullptr;
}

Testigo * Mapa:: getTestigoEn( int fila, int columna ) const {
    for ( Testigo* t : this->testigos ) {
        if ( !t->isProcesado() && t->getFila() == fila && t->getColumna() == columna ) {
            return t;
        }
    }
    return nullptr;
}

void Mapa:: removerTestigoEn( int fila, int columna ) {
    Ubicacion * n = getNodo( fila , columna );
    if ( n ) {
        n->setTieneTestigo( false );
    }
    for ( Testigo * t : this->testigos ) {
        if ( t->getFila() == fila && t->getColumna() == columna ) {
            t->setProcesado( true );
        }
    }
}

void Mapa:: reposicionarPista( Pista * pista ) {
    uniform_int_distribution<int> dist( INTERIOR_MIN, INTERIOR_MAX );
    while ( true ) {
        int f = dist( this-> rng ), c = dist( this->rng );
        Ubicacion * n = getNodo( f, c );
        if ( n && n->getTipo() == TipoUbicacion::CALLE && !n->getTienePista() && !n->getTieneTestigo() ) {
            pista->setFila( f );
            pista->setColumna( c );
            n-> setTienePista( true );
            return;
        }
    }
}

void Mapa:: resetVisibilidad() {
    //Recorre la lista enlazada y cubre todo con 'o' ( excepto edificios)

    Ubicacion* filaPtr = this->cabeza;

    while ( filaPtr ) {
        Ubicacion * n = filaPtr;
        while ( n ) {
            if ( n->getTipo() != TipoUbicacion::EDIFICIO ) {
                n->setDescubierto( false );
            }
            n = n->getDerecha();
        }
        filaPtr = filaPtr->getAbajo();
    }
}

int Mapa::eliminarCallejonesAleatorios( int cantidad ) {
    //Recolectar callejones con transversal de la lista (vector locar para shuffle).
    list <Ubicacion*> callejones;
    Ubicacion * filaPtr = this-> cabeza;
    while ( filaPtr ) {
        Ubicacion * n = filaPtr;
        while ( n ) {
            if ( n->getTipo() == TipoUbicacion::CALLEJON ) {
                callejones.push_back( n );
            }
            n = n->getDerecha();
        }
        filaPtr = filaPtr->getAbajo();
    }
    if ( callejones.empty() ) {
        return 0;
    }

    int eliminados = 0;
    int aEliminar = min( cantidad, (int) callejones.size() );

    for ( int i = 0; i < aEliminar; i++ ) {
        //Escoger un callejon al azar de la lista:
        uniform_int_distribution<int> d( 0, (int) callejones.size() - 1 );
        auto it = callejones.begin();
        advance( it, d( this->rng ) ); //Avanzar al indice aleatorio.

        (*it)->setTipo( TipoUbicacion::CALLE );
        (*it)->setDescubierto( false );
        callejones.erase( it ); //Quitarlo para no eligirlo dos veces.
        eliminados++;
    }

    return eliminados;
}

const list<Pista*>& Mapa:: getPistas() const {
    return this->pistas;
}

const list<Testigo*>& Mapa:: getTestigos() const {
    return this->testigos;
}

bool Mapa:: hayPistaEn( int fila, int columna ) const {
    Ubicacion * n = getNodo( fila, columna );
    return n && n->getTienePista();
}

bool Mapa:: hayTestigoEn( int fila, int columna ) const {
    Ubicacion * n = getNodo( fila, columna );
    return n && n->getTieneTestigo();
}

//Display -- Recorre fila por fila con punteros de la lista.

void Mapa:: mostrar( int detFila, int detCol ) const {
    Ubicacion * filaPtr = this->cabeza;

    while ( filaPtr ) {
        Ubicacion * n = filaPtr;
        while ( n ) {
            bool esDetective = ( n->getFila() == detFila && n->getColumna() == detCol );
            cout << ' ' << (esDetective ? 'I' : n->getSimbolo() );
            n = n->getDerecha();
        }
        cout << endl;
        filaPtr = filaPtr -> getAbajo();
    }
}