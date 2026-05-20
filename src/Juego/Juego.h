#ifndef PROYECTOESTRUCTURADATOS_JUEGO_H
#define PROYECTOESTRUCTURADATOS_JUEGO_H

#include <string>
#include <queue> // TAD Cola - declaraciones de testigos.
#include <vector>
#include <random>
#include <iostream>
#include <limits>
#include <cctype>
#include <algorithm>
#include "../Detective/Detective.h"
#include "../Mapa/Mapa.h"
#include "../TablaSospechosos/TablaSospechosos.h"

using std:: string;
using std:: queue;
using std:: vector;
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
    queue<Testigo*> colaTestigos; //TAD Cola
    vector<string> atributosRevelados; // atributos del culpable ya conocidos.
    mt19937 rng;
    bool jugando;
    bool casoResuelto;
    bool casoFinalizado; //Para saber si el jugador finalizo el juego.

    //Comandos:
    void mover( char cmd );
    void usarPista();
    void verSospechosos();
    void interrogarTestigo();
    void faseAcusacion();

    //Auxiliar:
    void revelarAtributoDelCulpable( const string& origen );

    //UI:
    void limpiarPantalla();
    void pausar();
    void mostrarEncabezado();
    void mostrarComandos();
    void mostrarBienvenida();

public:
    Juego();
    ~Juego();

    void iniciar();// Configura nombre, mapa, detective,sospechosos.
    void jugar(); //bucle principal.

    //Getter para que main registre el puntaje en el ABB:
    const string& getNombreDetective() const;
    int getPuntajeFinal() const;
    bool fueFinalizado() const;
    bool fueResuelto() const;

};


#endif //PROYECTOESTRUCTURADATOS_JUEGO_H