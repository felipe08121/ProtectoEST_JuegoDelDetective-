#include "Juego.h"

//Constructor / Destructor:

Juego:: Juego() {
    this->detective = nullptr;
    this->mapa = nullptr;
    this->sospechosos = nullptr;
    this->rng = mt19937( random_device{}() );
    this->jugando = true;
    this->casoResuelto = false;
}

Juego:: ~Juego() {
    delete this->detective;
    delete this->mapa;
    delete this->sospechosos;
    //Los testigos de la cola pertenecen al Mapa (relacion de prestamo).
}

//Inicializacion:

void Juego:: iniciar() {
    limpiarPantalla();
    mostrarBienvenida();

    string nombre;
    cout << endl<<" Tu nombre, detective (Recomiendo 'Holmes' i 'Sherlock'):  " ;
    getline( cin, nombre);

    if ( nombre.empty() ) {
        nombre = "Anonimo";
    }

    cout << endl << " Bienvenido, Detective " << nombre << ". El caso te espera..." << endl;
    pausar();

    this->mapa = new Mapa();
    auto[ df , dc ] = this->mapa->posicionInicialDetective();
    this->detective = new Detective( nombre, df ,dc );
    this->sospechosos = new TablaSospechosos();

}

//Bucle principal:

void Juego:: jugar() {

    while ( this->jugando ) {
        limpiarPantalla();
        mostrarEncabezado();

        this->mapa->mostrar( this->detective->getFila(), this->detective->getColumna() );
        cout << endl;
        mostrarComandos();

        char cmd;
        cin >> cmd;
        cin.ignore( numeric_limits<streamsize>:: max(), '\n' );
        cmd = (char) toupper( cmd );

        switch ( cmd ) {
            case 'W': case 'S': case 'A': case 'D':
                mover( cmd );
                break;

            case 'T' : {
                this->detective->mostrarPilaPistas();
                pausar();
                break;
            }

            case 'X': {
                usarPista();
                break;
            }

            case 'Z': {
                verSospechosos();
                break;
            }

            case 'I': {
                interrogarTestigo();
                break;
            }

            case 'Q': {
                this->jugando = false;
                cout << endl << " Siempre hay una sola verdad!! Hasta la proxima, Detective "
                     << this->detective->getNombre() << endl;
                break;
            }

            default: {
                cout << endl << " Comando no reconocido. Usa W/A/S/D para moverte." << endl;
                pausar();
            }
        }

        //Se recolectaron las 10 pistas? --> Fase de ecusacion:
        if ( this->jugando && this->detective->cantidadPistas() >= Mapa::NUM_PISTAS ) {
            faseAcusacion();
            this->jugando = false;
        }
    }
}

//Comando: Mover (W/A/S/D):

void Juego:: mover( char cmd ) {

    Ubicacion * destino = this->mapa->getNodoVecino(
       this->detective->getFila(), this->detective->getColumna(), cmd );

    if ( !destino ) {
        return;
    }

    this->detective->incrementarPuntaje();

    //Edificio: no se puede pasar:

    if ( destino->getTipo() == TipoUbicacion::EDIFICIO ) {
        cout << endl << " [#] Edificio!! No puedes salir de la ciudad. (Pailas chino) " << endl;
        pausar();
        return;
    }

    //Callejon: se revelo pero no se puede cruzar:
    if ( destino->getTipo() == TipoUbicacion::CALLEJON ) {
        destino->setDescubierto( true );
        cout << endl << " [|] Callejon cerrado! Busca otra ruta (tienes pies)." << endl;
        pausar();
        return;
    }

    //Callejon transitable:
    this->detective->setFila( destino->getFila() );
    this->detective->setColumna( destino->getColumna() );
    destino->setDescubierto( true );

    //Hay pista?:
    if ( destino->getTienePista() ) {
        Pista * p = this-> mapa -> recogerPistaEn( destino->getFila(), destino->getColumna() );
        if ( p ) {
            this->detective->agregarPista( p );
            cout << endl << " [*] Pista encontrada! Tipo: "
                 << p->getTipoStr() << " [" << p->getLetra() << "] " << endl;
            //Cada pista revela un atributo del culpable.
            revelarAtributoDelCulpable( "pista" );
            pausar();
        }
    }

    //Hay testigo?:

    if ( destino->getTieneTestigo() ) {
        Testigo * t = this->mapa->getTestigoEn( destino->getFila(), destino->getColumna() );
        if ( t ) {
            this->mapa->removerTestigoEn( destino->getFila(), destino->getColumna() );
            this->colaTestigos.push( t ); //push a la Cola.
            cout << endl << " [W] Testigo! Su declaracion quedo en la cola." << endl;
            cout << "   Usa [I] para interrogar al siguiente testigo." << endl;
            pausar();
        }
    }
}

//Comando: Usar Pista (X):

void Juego:: usarPista() {

    if ( !this->detective->tienePistas() ) {
        cout << endl << " No tienes pistas para usar aun." << endl;
        pausar();
        return;
    }

    Pista * p = this->detective->sacarUltimaPista();
    cout << endl << " Usando pista: [ " << p->getLetra()
         << " ] " << p->getTipoStr() << endl;

    switch ( p->getTipo() ) {

        case TipoPista::HUELLA: {
            this->detective->setPuntaje( this->detective->getPuntaje()/2 );
            cout << " >> Huella: Puntaje reducido a la mitad ("
                 << this->detective->getPuntaje() << " puntos)." << endl;
            break;
        }

        case TipoPista:: COARTADA: {
            int elim = this->mapa->eliminarCallejonesAleatorios( 2  );
            cout << " >> Coartada: " << elim
                 << " Callejon(es) eliminado(s) de mapa." << endl;
            break;
        }

        case TipoPista:: TESTIMONIO: {
            uniform_int_distribution<int> moneda( 0,1 );
            if ( moneda( this->rng ) == 0 ) {
                this->detective-> setPuntaje( 0 );
                cout << " >> Testimonio: Suerte! Puntaje reseteado a 0." << endl;
            }
            else {
                this->detective->setPuntaje( this->detective->getPuntaje() * 2 );
                cout << " >> Testimonio: Mala suerte! Puntaje duplicado a "
                     << this->detective->getPuntaje() << endl;
            }
            break;
        }

        case TipoPista::PRUEBA_FORENSE: {
            auto[ nf, nc ] = this->mapa->posicionNoCubiertaNiCallejon(
                this->detective->getFila(), this->detective->getColumna() );
            this->detective->setFila( nf );
            this->detective->setColumna( nc );
            this->mapa->marcarDescubierto( nf, nc );
            cout << " >> Prueba Forense: detective teletransportando." << endl;
            break;
        }
    }

    //La pista vuelve al mapa y se cubre todo con 'o':

    this->mapa->reposicionarPista( p );
    this->mapa->resetVisibilidad();
    this->mapa->marcarDescubierto( this->detective->getFila(), this->detective->getColumna() );

    cout  << "La pista volvio al mapa. El tablero se cubrio de nuevo." << endl;
    pausar();
}

//Comando: Ver sospechosos( Z ):

void Juego:: verSospechosos() {
    this->sospechosos->mostrar( this->atributosRevelados, this->detective->getNombre() );
    pausar();
}

//Comando: Interrogar Testigo (I):

void Juego:: interrogarTestigo() {

    if ( this->colaTestigos.empty() ) {
        cout << endl << " No hay testigos en la cola para interrogar." << endl;
        pausar();
        return;
    }

    Testigo* t = this->colaTestigos.front(); //siguiente en la cola FIFO.
    this->colaTestigos.pop();

    cout << endl << " Interrogando al testigo encontrado en ("
         << t->getFila() << "," << t->getColumna() << ")..." << endl;

    //El testigo tambien revela un atributo del culpable.
    revelarAtributoDelCulpable( "testigo" );
    pausar();
}

//Auxiliar: revela un atributo del culpable - lo usa pista o testigo.

void Juego:: revelarAtributoDelCulpable( const string& origen ) {

    Sospechoso * culp = this->sospechosos->getCulpable();
    const vector <string>& todos = culp->getAtributos();

    //Buscar atributos del culpable que no se han revelado aun.

    vector<string> noRevelados;
    for ( const string& attr : todos ) {
        if ( find( this->atributosRevelados.begin(),
                   this->atributosRevelados.end(), attr ) == this->atributosRevelados.end() ) {
            noRevelados.push_back( attr );
        }
    }

    if ( noRevelados.empty() ) {
        cout << " -> Ya conoces todos los atributos del culpable." << endl;
        return;
    }

    //Escoger uno al azar:

    uniform_int_distribution<int> dist( 0, noRevelados.size() - 1 );
    string nuevo = noRevelados[ dist( this->rng ) ];
    this->atributosRevelados.push_back( nuevo );

    cout << " -> Atributo del culpable revelado (" << origen << "): "
         << nuevo << endl;
}

//Fase de Acusacion:

void Juego:: faseAcusacion() {

    limpiarPantalla();
    mostrarEncabezado();

    cout << endl << " *** " << this->detective->getNombre()
         << ", has recolectado las 10 pistas! ***" << endl;
    cout << " Es momento de ACUSAR." << endl;

    //Mostrar la Tabla Hash con todos los atributoa revelados:
    this->sospechosos->mostrar( this->atributosRevelados,
                                this->detective->getNombre() );

    cout << endl << " Atributos del culpable revelados: ";
    for ( int i = 0; i < (int)this->atributosRevelados.size(); i++ ) {
        cout << this->atributosRevelados[ i ];
        if ( i < (int)this->atributosRevelados.size() - 1 ) cout << ", ";
    }

    cout << endl;

    cout << endl << " A quien acusas? > ";
    string acusado;
    getline( cin, acusado );

    //Busqueda O(1) explicita en la Tabla Hash:

    cout << endl << " Buscando '" << acusado << "' en la Tabla Hash (O(1) promedio)..." << endl;
    Sospechoso * s = this->sospechosos->buscar( acusado );

    if ( !s ) {
        cout << " >> '" << acusado << "' no esta entre los sospechosos." << endl;
        cout << " >> Caso fracasado. Puntaje penalizado al doble." << endl;
        this->detective->setPuntaje( this->detective->getPuntaje() * 2 );
        this->casoResuelto = false;
    }

    else if ( this->sospechosos->acusar( acusado ) ) {
        cout << " *** Caso Resuelto! " << acusado
             << " era el culpable. ***" << endl;
        cout << " Puntaje final: " << this->detective->getPuntaje()
             << " movimientos." << endl;
        this->casoResuelto = true;
    }
    else {
        cout << " >> Incorrecto. El verdadero culpable era "
             <<  this->sospechosos->getCulpable()->getNombre() << "." << endl;
        cout << " >> Caso fracasado. Puntaje penalizado al doble." << endl;
        this->detective->setPuntaje( this->detective->getPuntaje() * 2 );
        this->casoResuelto = false;
    }
    pausar();
    //Todo fase 3: guardar puntaje en el BBB historico.
}


//utilidades de UI:

void Juego:: limpiarPantalla() {
    cout << "\033[2J\033[H";
    cout.flush();
}

void Juego:: pausar() {
    cout << endl << " [Presiona Enter para continuar...]";
    cin.get();
}

void Juego:: mostrarEncabezado() {
    cout << endl << "==========================================" << endl;
    cout << " Detective : " << this->detective->getNombre() << endl;
    cout << " Puntaje   : " << this->detective->getPuntaje() << " movimiento(s)" << endl;
    cout << " Pistas    : " << this->detective->cantidadPistas()
         << " / " << Mapa::NUM_PISTAS << endl;
    cout << " Testigos  : " << this->colaTestigos.size() << " en la cola" << endl;
    cout << "==========================================" << endl;
}

void Juego:: mostrarComandos() {
    cout << " [W/A/S/D] Mover [T] Ver pistas [X] Usar pista" << endl;
    cout << " [Z] Sospechosos [I] Interrogar testigo [Q] Salir" << endl;
    cout << " Tu turno > ";
}

void Juego:: mostrarBienvenida() {
    cout << R"(
  +------------------------------------------+
  |        EL CASO DEL DETECTIVE             |
  |   Recolecta 10 pistas y atrapa al        |
  |       sospechoso correcto.               |
  +------------------------------------------+
)";
}