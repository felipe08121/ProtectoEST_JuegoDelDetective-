#ifndef PROYECTOESTRUCTURADATOS_JUEGO_H
#define PROYECTOESTRUCTURADATOS_JUEGO_H

#include <string>
#include <queue> // TAD Cola - declaraciones de testigos.
#include <random>
#include <iostream>
#include <limits>
#include <cctype>
#include <algorithm>
#include <list>
#include "../Detective/Detective.h"
#include "../Mapa/Mapa.h"
#include "../TablaSospechosos/TablaSospechosos.h"
#include "../ArbolPuntajes/ArbolPuntajes.h"

using std::list;
using std:: string;
using std:: queue;
using std:: mt19937;
using std:: cout;
using std:: endl;
using std:: cin;
using std:: getline;
using std:: numeric_limits;
using std:: streamsize;
using std:: uniform_int_distribution;
using std:: random_device;
using std:: find;


//Clase controladora: controla todo el flujo del juego.

class Juego {

private:
    Detective * detective;
    Mapa * mapa;
    TablaSospechosos* sospechosos;
    ArbolPuntajes ranking; // Perisiste entre partidas.
    queue<Testigo*> colaTestigos; //TAD Cola
    list<string> atributosRevelados; // atributos del culpable ya conocidos.
    mt19937 rng;
    bool jugando;
    bool casoResuelto;
    bool casoFinalizado; //Para saber si el jugador finalizo el juego.

    //Logica de una partida:
    void iniciarPartida();// <- Antes se llamada iniciar().
    void jugarPartida(); // <- Antes se llamada jugar().

    //Comandos:
    void mover( char cmd );
    void usarPista();
    void verSospechosos();
    void interrogarTestigo();
    void faseAcusacion();

    //Auxiliar:
    void revelarAtributoDelCulpable( const string& origen );

    //UI del menu principal:
    void mostrarMenuPrincipal();
    void opcionBuscarDetective();
    void opcionVerRanking();

    //UI:
    void limpiarPantalla();
    void pausar();
    void mostrarEncabezado();
    void mostrarComandos();
    void mostrarBienvenida();

    //Getter para que main registre el puntaje en el ABB:
    const string& getNombreDetective() const;
    int getPuntajeFinal() const;
    bool fueFinalizado() const;
    bool fueResuelto() const;

public:
    Juego();
    ~Juego();

    //Punto de entrada principal -- contiene el menu y el loop de partidas:
    void ejecutar();


};


#endif //PROYECTOESTRUCTURADATOS_JUEGO_H