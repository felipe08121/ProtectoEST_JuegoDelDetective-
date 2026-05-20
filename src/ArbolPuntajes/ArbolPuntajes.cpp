//
// Created by felip on 16/05/2026.
//

#include "ArbolPuntajes.h"

//Nodo ABB:

NodoABB:: NodoABB( const string& nombre, int puntaje ) {
    this->nombre = nombre;
    this-> puntaje = puntaje;
    this->izq = nullptr;
    this->der = nullptr;
}

//Constructor/Destructor (del Arbol en si, no del nodo):

ArbolPuntajes:: ArbolPuntajes(): rankingJson("../src/RankingJson/ranking.json" ) {
    this-> raiz = nullptr;
    if ( this->rankingJson.existeYNoEstaVacio() ) {
        vector<pair<string, int>> entradas = this->rankingJson.cargar();

        for ( const pair<string, int>& e : entradas ) {
            this->raiz = insertar( this-> raiz, e.first, e.second );
        }
    }
}

ArbolPuntajes:: ~ArbolPuntajes() {
    liberarMemoria( this-> raiz );
}

void ArbolPuntajes:: liberarMemoria( NodoABB * nodo ) {
    if ( !nodo ) {
        return;
    }
    liberarMemoria( nodo->izq );
    liberarMemoria( nodo->der );
    delete nodo;
}

// Recolectar inorden para guardar en archivo:

void ArbolPuntajes:: recolectarInorden( NodoABB * nodo, vector< pair< string, int>>& entradas ) const {
    if ( !nodo ) {
        return;
    }
    recolectarInorden( nodo->izq, entradas );
    entradas.push_back( { nodo->nombre, nodo->puntaje} );
    recolectarInorden( nodo->der, entradas );
}

// Insertar -- Iterativo:
// Baja siempre por un unico camino (izq o der),
// sin necesidad de reconstruir ramas multiples.

NodoABB * ArbolPuntajes:: insertar( NodoABB * raiz,
                                    const string& nombre, int puntaje ) {
    NodoABB * nuevo = new NodoABB( nombre, puntaje );

    //Arbol vacio: el nuevo nodo es la raiz

    if ( !raiz ) {
        return nuevo;
    }

    NodoABB * actual = raiz;
    NodoABB * padre = nullptr;


    //Bajar hasta encontrar el lugar insercion:

    while ( actual ) {
        padre = actual;
        if ( puntaje <= actual->puntaje ) {
            actual = actual->izq;
        }
        else {
            actual = actual->der;
        }
    }

    //Enlazar el nuevo nodo al padre encontrado:

    if ( puntaje <= padre->puntaje ) {
        padre -> izq = nuevo;
    }
    else {
        padre->der = nuevo;
    }

    return raiz; //La raiz no cambia en insecion normal.
}

// Encontrar el nodo minimo -- Iterativo:

NodoABB * ArbolPuntajes:: encontrarMinimo( NodoABB* nodo ) const {
    while ( nodo && nodo->izq ) {
        nodo = nodo->izq;
    }
    return nodo;
}

NodoABB * ArbolPuntajes:: eliminar( NodoABB * nodo,
                                    const string& nombre, int puntaje ) {
    if ( !nodo ) {
        return nullptr;
    }

    if ( puntaje < nodo->puntaje ) {
        nodo->izq = eliminar( nodo->izq, nombre, puntaje );
    }

    else if ( puntaje > nodo->puntaje ) {
        nodo->der = eliminar( nodo->der, nombre, puntaje );
    }
    else {
        //Puntaje coincide -- verificar nombre:
        if ( nodo->nombre == nombre ) {

            //Caso 1: sin hijo izquierdo:

            if ( !nodo->izq ) {
                NodoABB * temp = nodo->der;
                delete nodo;
                return temp;
            }

            //Caso 2: sin hijo derecho:
            if ( !nodo->der ) {
                NodoABB * temp = nodo->izq;
                delete nodo;
                return temp;
            }

            //Caso 3: dos hijos -- reemplazar con sucesor inorden:

            NodoABB * sucesor = encontrarMinimo ( nodo->der);
            nodo->nombre = sucesor->nombre;
            nodo->puntaje = sucesor->puntaje;
            nodo->der = eliminar( nodo->der, sucesor->nombre, sucesor->puntaje );
        }
        else {
            //Mismo puntaje, diferente nombre -- buscar a la izquierda:

            nodo->izq = eliminar( nodo->izq, nombre, puntaje );
        }
    }
    return nodo;
}

// Buscar por nombre -- Recursivo:
// Debe explorar ambas ramas (no hay orden por nombre):

NodoABB* ArbolPuntajes:: buscarPorNombre( NodoABB * nodo,
                                          const string& nombre ) const {
    if ( !nodo ) {
        return nullptr;
    }
    if ( nodo->nombre == nombre ) {
        return nodo;
    }

    NodoABB * encontrado = buscarPorNombre( nodo->izq, nombre );
    if ( encontrado ) {
        return nodo;
    }

    return buscarPorNombre( nodo->der, nombre );
}

//Registrar puntaje:

void ArbolPuntajes:: registrarPuntaje( const string& nombre, int puntaje ) {
    int puntajeExistente;

    if ( buscarDetective ( nombre, puntajeExistente ) ) {
        //El detective ya jugo antes:
        if ( puntaje < puntajeExistente ) {
            //Nuevo puntaje es mejor (menor) -- reemplazar.
            this->raiz = eliminar( this-> raiz, nombre, puntajeExistente );
            this->raiz = insertar( this->raiz, nombre, puntaje );
            cout << " Nuevo record para " << nombre << "! "
                 << puntaje << " (antes: " << puntajeExistente << ") " << endl;
        }
        //Puntaje anterior era mejor -- no se cambia:
        else{
            cout << " " << nombre << " ya tiene un mejor puntaje: "
                 << puntajeExistente << " (actual: " << puntaje << ") " << endl;
        }
    }
    else {
        //Detective nuevo -- insertar.
        this->raiz = insertar( this-> raiz, nombre, puntaje );
        cout << " Puntaje registrado para " << nombre << ": "
             << puntaje << " movimientos." << endl;
    }

    //Recolectar inorden y guardar en ranking.json:
    vector<pair<string, int>> entradas;
    recolectarInorden( this->raiz, entradas );
    this->rankingJson.guardar( entradas );
}

//Buscar Detective por nombre:

bool ArbolPuntajes:: buscarDetective( const string& nombre,int& mejorPuntaje ) const {

    NodoABB* encontrado = buscarPorNombre( this->raiz, nombre );
    if ( encontrado ) {
        mejorPuntaje = encontrado->puntaje; // Devuelve el puntaje por referencia.
        return true; //Confirma que ya jugo antes.
    }
    return false; //Nunca ha jugado antes.
}

//Recorrido Inorden:

void ArbolPuntajes:: inorden( NodoABB * nodo, int& posicion ) const {
    if ( !nodo ) {
        return;
    }

    inorden( nodo->izq, posicion );
    posicion++;
    cout << "   " << posicion << ". " << nodo->nombre
         << " -- " << nodo->puntaje << " movimientos" << endl;
    inorden( nodo->der, posicion );
}

void ArbolPuntajes:: mostrarRanking() const {
    if ( !this->raiz ) {
        cout << endl << "No hay puntajes registrados aun." << endl;
        return;
    }

    cout << endl <<  " === RANKING DE DETECTIVES ===" << endl;
    cout << "  (menor puntaje = mejor)" << endl << endl;
    int pos = 0;
    inorden( this->raiz, pos );
}

bool ArbolPuntajes:: estaVacio() const {
    return this->raiz == nullptr;
}
