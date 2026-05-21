#include "TablaSospechosos.h"

//Constructor:

TablaSospechosos:: TablaSospechosos() {
    this-> culpable = nullptr;
    this-> rng = mt19937( random_device{}() );
    generarSospechosos();
}

//Destructor:

TablaSospechosos:: ~TablaSospechosos() {
    for ( auto& par : this->tabla) {
        delete par.second;
    }
}

//Generacion inicial -- Crea los 8 sospechosos con atributos aleatorios
//escoge al culpable.

void TablaSospechosos:: generarSospechosos() {
    //Lista predefinida de nombre con su sexo:

    list< pair<string,string> > candidatos = {
        { "Carlos",   "hombre" },  { "Diana",    "mujer" },
        { "Eduardo",  "hombre" },  { "Fernanda", "mujer" },
        { "Gonzalo",  "hombre" },  { "Hilda",    "mujer" },
        { "Bernardo", "hombre" },  { "Alicia",   "mujer" },
        { "Ricardo",  "hombre" },  { "Sofia",    "mujer" },
        { "Mateo",    "hombre" },  { "Lucia",    "mujer" },
        { "Javier",   "hombre" },  { "Camila",   "mujer" }
    };

    //Pool de atributos (uno por categoria para cada sospechoso):

    list<string> estaturas = { "alto", "bajo", "mediano"};
    list<string> cabellos = { "cabello rubio", "cabello moreno",
                              "cabello pelirrojo", "cabello canoso" };
    list<string> pieles = { "piel blanca", "piel triguena", "piel oscura"};
    list<string> lateralidad = {"zurdo", "diestro"};

    //Funcion Auxiliar para eligir elemento aleatorio de una lista:

    auto aleatorio = [&] ( const list<string>& lista ) -> string {
        uniform_int_distribution<int> d( 0, (int) lista.size() - 1 );
        auto it = lista.begin();
        advance( it, d( this->rng ) );
        return *it;
    };

    //Escoger 8 candidatos al azar ( sin repetir) :

    while ( (int) this->tabla.size() < NUM_SOSPECHOSOS && !candidatos.empty() ){
        uniform_int_distribution<int> d( 0, (int) candidatos.size() -1 );
        auto it = candidatos.begin();
        advance( it, d( this->rng ) );

        string nom = it->first;
        string sexo = it->second;
        candidatos.erase( it ); //Quitar para no repetir.

        list<string> attrs;
        attrs.push_back( sexo );
        attrs.push_back( aleatorio (estaturas) );
        attrs.push_back( aleatorio (cabellos) );
        attrs.push_back( aleatorio (pieles) );
        attrs.push_back( aleatorio (lateralidad) );

        Sospechoso * s = new Sospechoso ( nom, attrs );
        this->tabla[ nom ] = s;
    }

    //Escoger culpable al azar entre los 8 ingresados:
    uniform_int_distribution<int> distCulp( 0, NUM_SOSPECHOSOS - 1 );
    int idx = distCulp( this-> rng );
    int actual = 0;
    for ( auto& par : this->tabla ) {
        if ( actual == idx ) {
            this->culpable = par.second;
            break;
        }
        actual++;
    }
}

//Busqueda O(1) promedio.

Sospechoso * TablaSospechosos:: buscar( const string& nombre ) const {
    auto it = this->tabla.find( nombre ); //unordered_map::find es O(1) promedio.
    if ( it == this->tabla.end() ) {
        return nullptr;
    }
    return it->second;
}

bool TablaSospechosos:: acusar( const string& nombre ) const {
    Sospechoso * s = buscar( nombre );
    if ( !s ) {
        return false;
    }
    return s == this->culpable; //comparacion de punteros -- mismo objeto.
}

Sospechoso * TablaSospechosos :: getCulpable() const {
    return this-> culpable;
}

const unordered_map<string, Sospechoso*>& TablaSospechosos:: getTabla() const {
    return this-> tabla;
}

//Display -- muestra cada sospechoso con sus atributos que coinciden con los
//revelados del culpable:

void TablaSospechosos :: mostrar ( const list<string>& atributosRevelados, const string& nombreDetective ) const {
    cout << endl << " " << nombreDetective << ", sospechosos del caso (atributos del culpable revelados hasta ahora):"
         << endl;

    for ( auto& par : this->tabla ) {
        Sospechoso * s = par.second;
        cout << "   "  << s->getNombre() << " | atributos confirmados: ";

        bool primero = true;

        for ( const string& attr : atributosRevelados ) {
            if ( s->tieneAtributo( attr ) ) {
                if ( !primero ) {
                    cout << ", ";
                }
                cout << attr;
                primero = false;
            }
        }

        if ( primero ) {
            cout << "--"; //Ninguno de los revelados coincide con este sospechoso.
        }
        cout << endl;
    }
}