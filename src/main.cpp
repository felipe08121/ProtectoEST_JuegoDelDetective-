#include "Juego/Juego.h"
#include "ArbolPuntajes/ArbolPuntajes.h"

void limpiarMenu() {
    cout << "\033[2J\033[H";
    cout.flush();
}

void pausarMenu() {
    cout << endl << " [Presiona Enter para continuar...]";
    cin.get();
}

void mostrarMenuPrincipal() {
    limpiarMenu();
    cout << R"(
  +------------------------------------------+
  |        EL CASO DEL DETECTIVE             |
  |                                          |
  |   1. Nueva partida                       |
  |   2. Buscar detective (puntaje)          |
  |   3. Ver ranking historico               |
  |   4. Salir                               |
  +------------------------------------------+
)" << endl;
    cout << " Opcion -> ";
}

int main() {

    //El ABB carga automaticamente ranking.json al crearse.
    //Si es la primera vez, simplemente arranca vacio.

    ArbolPuntajes ranking;
    bool ejecutando = true;

    while ( ejecutando ) {
        mostrarMenuPrincipal();

        char opcion;
        cin >> opcion;
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );

        switch ( opcion ) {

            //Nueva Partida:
            case '1': {
                Juego juego;
                juego.iniciar();
                juego.jugar();

                //Guardar puntaje solo si el caso se finalizo:
                if ( juego.fueFinalizado() ) {
                    ranking.registrarPuntaje( juego.getNombreDetective(), juego.getPuntajeFinal() );
                    //El archivo se actualiza automaticamente dentro de registrarPuntaje().
                }
                else {
                    cout << endl << " Partida abandonada -- puntaje no registrado." << endl;
                }
                pausarMenu();
                break;
            }

            case '2': {
                limpiarMenu();
                cout << endl << " === BUSCAR DETECTIVE ===" << endl;
                cout << " Nombre del detective: ";
                string nombre;
                getline( cin, nombre);

                int puntaje;
                if ( ranking.buscarDetective( nombre, puntaje ) ) {
                    cout << endl << " " << nombre << " ha jugado antes."
                         << " Mejor puntaje: " << puntaje << " movimientos." << endl;
                }
                else {
                    cout << endl << " " << nombre
                         << " no tiene puntaje registrado." << endl;
                }
                pausarMenu();
                break;
            }

            //Ver ranking:
            case '3': {
                limpiarMenu();
                ranking.mostrarRanking();
                pausarMenu();
                break;
            }

            case '4': {
                ejecutando = false;
                cout << endl << " Hasta la proxima! " << endl;
                break;
            }

            default: {
                cout << endl << " Opcion no valida." << endl;
                pausarMenu();
                break;
            }
        }
    }

    return 0;
}