#include <iostream>
#include <string>
#include <limits>
#include <random>
#include <cctype>
#include "Mapa/Mapa.h"
#include "Detective/Detective.h"
#include "Pista/Pista.h"

using std:: mt19937;
using std:: random_device;
using std:: cout;
using std:: endl;
using std:: cin;
using std:: getline;
using std::numeric_limits;
using std::streamsize;
using std::uniform_int_distribution;

//Generador aleatorio para efectos del main:
static mt19937 mainRng( random_device{}() );

//Utilidades del interfaz de usuario:

void limpiarPantalla() {
    cout << "\033[2J\033[H";
    cout.flush();
}

void pausar() {
    cout << endl << "  [Presiona Enter para continuar...] ";
    cin.get();
}

void mostrarEncabezado( const Detective& d ) {
    cout << endl <<"==========================================" << endl;
    cout << " Detective : " << d.getNombre() << endl;
    cout << " Puntaje   : " << d.getPuntaje() << " movimientos(s)" << endl;
    cout << " Pistas    : " << d.cantidadPistas()
         << " / " << Mapa::NUM_PISTAS << endl;
    cout << "==========================================" << endl;
}

void mostrarComandos() {
    cout << " [W/A/S/D] Mover  [T] ver pistas  [X] Usar pista" << endl;
    /* Nota: 'S' = abajo (movimiento) 'Z' = ver sospechosos.
     * La tecla 'S' del enuncaciado para sospechosos entra en conflicto con 'S' = abajo.
     * Se usa 'Z' para evitar el conflicto. hay que consultar con el profesor para ver si no hay problema.
     */
    cout << " [Z] Sospechosos  [I] Interrogar testigo  [Q] Salir" << endl;
    cout << " Tu turno > ";
}

//Comando X: Usar la ultima pista de la Pila:

void usarPista( Detective& detective, Mapa& mapa ) {
    if ( !detective.tienePistas() ) {
        cout << endl << " No tiene pistas para usar aun." << endl;
        pausar();
        return;
    }

    Pista * p = detective.sacarUltimaPista(); //Pop de la fila.
    cout << endl << " Usando pista: [ " << p->getLetra()
         << " ] " << p->getTipoStr() << endl;

    switch ( p->getTipo() ) {
        //Huella: puntaje a la mitad:
        case TipoPista::HUELLA: {
            detective.setPuntaje( detective.getPuntaje() / 2 );
            cout << " >> Huella: Puntaje reducido a la mitad ("
                 << detective.getPuntaje() << " puntos)." << endl;
            break;
        }
        //Coartada: eliminar 2 callejones.
        case TipoPista::COARTADA: {
            int elim = mapa.eliminarCallejonesAleatorios( 2 );
            cout << " >> Coartada: " << elim
                 << " Callejon(es) elimanado(s) del mapa." << endl;
            break;
        }
        //Testimonio: Puntaje 0 o x2 aleatorio.
        case TipoPista::TESTIMONIO: {
            uniform_int_distribution<int> moneda( 0,1 );
            if ( moneda( mainRng) == 0 ) {
                detective.setPuntaje( 0 );
                cout << " >> Testimonio: suerte! reseateado a 0." << endl;
            }
            else {
                detective.setPuntaje( detective.getPuntaje() * 2 );
                cout << " >> Testimonio: mala suerte! Puntaje duplicado a "
                     << detective.getPuntaje() << endl;
            }
            break;
        }
        // Prueba Forense: teletransporta al detective.
        case TipoPista::PRUEBA_FORENSE: {
            auto[ nf ,nc ] = mapa.posicionNoCubiertaNiCallejon(
                detective.getFila(), detective.getColumna() );
            detective.setFila( nf );
            detective.setColumna( nc );
            mapa.marcarDescubierto( nf, nc );
            cout << " >> Prueba Forense: detctive teletransportado." << endl;
            break;
        }
    }

    //La pista vuelve al mapa en posicion aleatoria y se cubre todo con 'o':
    mapa.reposicionarPista( p );
    mapa.resetVisibilidad();
    mapa.marcarDescubierto( detective.getFila(), detective.getColumna() );

    cout << " >> La pista volvio al mapa. El tablero se cubrio de nuevo." << endl;
    pausar();
}

//Logica del movimiento W-A-S-D:

void mover( char cmd, Detective& detective, Mapa& mapa) {
    Ubicacion * destino = mapa.getNodoVecino(
        detective.getFila(), detective.getColumna(), cmd);

    if ( !destino ) {
        return;
    }

    detective.incrementarPuntaje(); //Todo movimiento cuesta 1 punto.

    //Edificion: borde, no se puede pasar:

    if ( destino->getTipo() == TipoUbicacion:: EDIFICIO ) {
        cout << endl << "  [#] Edificio! No puedes salir de la ciudad.(Pailas chino) " << endl;
        pausar();
        return;
    }

    // Callejon: se revela borde, no se puede pasar:
    if ( destino->getTipo() == TipoUbicacion::CALLEJON ) {
        destino->setDescubierto( true );
        cout << endl << "  [|] Callejon cerrado! Busca otra ruta (tienes pies)." << endl;
        pausar();
        return;
    }

    //Calle transitable: mover el detective:
    detective.setFila( destino->getFila() );
    detective.setColumna( destino->getColumna() );
    destino->setDescubierto( true );

    //Hay Pista? (estan ocultas - se revelan al pisarlas):
    if ( destino->getTienePista() ) {
        Pista * p = mapa.recogerPistaEn( destino->getFila(), destino->getColumna() );
        if ( p ) {
            detective.agregarPista( p ); //Push a la Pila.
            cout << endl << "  [*] Pista encontrada! Tipo: "
                 << p->getTipoStr() << " [" << p->getLetra() << "] " << endl;
                 //Todo fase 2: revelar un atributo del culpable.
            pausar();
        }
    }

    //Hay Testigo?:
    if ( destino->getTieneTestigo() ) {
        Testigo * t = mapa.getTestigoEn( destino->getFila(), destino->getColumna() ) ;
        if ( t ) {
            mapa.removerTestigoEn( destino->getFila(), destino->getColumna() );
            cout << endl << "  [W] Testigo!! Su declaracion quedo en la cola." << endl;
            //Todo fase 2: push a std::queue<Testigo*> de declaraciones.
            pausar();

        }
    }
}


int main() {

    limpiarPantalla();

    cout << R"(
  +------------------------------------------+
  |        EL CASO DEL DETECTIVE             |
  |   Recolecta 10 pistas y atrapa al        |
  |       sospechoso correcto.               |
  +------------------------------------------+
)";
    string nombre;
    cout << endl << " Tu nombre, detective (Recomiendo 'Holmes' o 'Sherlock'):  ";
    getline( cin, nombre );
    if ( nombre.empty() ) {
        nombre = "Anonimo";
    }

    cout << endl << " Bienvenido, Detective " << nombre << ". El caso te espera..." << endl;
    pausar();

    //Inicializacion:
    Mapa mapa;

    auto[ df, dc ] = mapa.posicionInicialDetective();
    Detective detective( nombre, df, dc );

    //Ciclo de juego:

    bool jugando = true;
    while ( jugando ) {
        limpiarPantalla();
        mostrarEncabezado( detective );
        mapa.mostrar( detective.getFila(), detective.getColumna() );
        cout << endl;
        mostrarComandos();

        char cmd;
        cin >> cmd;
        cin.ignore( numeric_limits<streamsize>::max(), '\n');
        cmd = (char)toupper( cmd );

        switch ( cmd ) {
            case 'W': case 'S': case 'A': case 'D':
                mover(cmd, detective, mapa );
            break;

            case 'T': {
                detective.mostrarPilaPistas();
                pausar();
                break;
            }

            case 'X': {
                usarPista( detective, mapa );
                break;
            }

            case 'Z': {
                //Todo en fase 2: mostrar Tabla Hash de sospechosos.
                cout << endl << " [Sospechosos - disponible en fase 2]" << endl;
                pausar();
                break;
            }

            case 'I': {
                //Todo fase 2: mostrar Tabla HAsh de sospechosos.
                cout << endl << "[Interrogar testigo -- Disponible en fase 2]" << endl;
                pausar();
                break;
            }

            case 'Q': {
                jugando = false;
                cout << endl << " Siempre hay una sola verdad! Hasta la proxima, Dectective "
                     << detective.getNombre() << endl;
                break;
            }

            default: {
                cout << endl << "  Comando no reconocido. Usa W/A/S/D para moverte." << endl;
                pausar();

            }
        }

        //Se recolectaron las 10 pistas? --> Fase de Acusacion --

        if ( jugando && detective.cantidadPistas() >= Mapa::NUM_PISTAS ) {
            limpiarPantalla();
            mostrarEncabezado( detective );
            mapa.mostrar( detective.getFila() , detective.getColumna() );
            cout << endl << " *** " << detective.getNombre()
                 << ", has recolectado las 10 pistas!!! ***" << endl;
            cout << " Fase de acusacion -- disponible en Fase 2." << endl;
            //Todo fase 2: mostrar sospechosos, acusar, guardar en ABB.
            pausar();
            jugando = false;
            cout << endl << " Siempre hay una sola verdad! Hasta la proxima, Dectective "
                     << detective.getNombre() << endl;
        }
    }

    return 0;
}
