#include "TablaSospechosos.h"

//Constructor
TablaSospechosos:: TablaSospechosos() {
    this-> culpable = nullptr;
    this-> rng = mt19937( random_device{}() );
    generarSospechosos();
}

//Destructor
TablaSospechosos:: ~TablaSospechosos() {
    for ( auto& par : this->tabla) {
        delete par.second;
    }
}

//Generacion inicial -- Crea los 8 sospechosos con atributos aleatorios
//escoge al culpable.

void TablaSospechosos:: generarSospechosos() {
    //Lista predefinida de nombre con su sexo:

    vector< pair<string,string> > nombresCandidatos = {
        { "Carlos",   "hombre" },  { "Diana",    "mujer" },
        { "Eduardo",  "hombre" },  { "Fernanda", "mujer" },
        { "Gonzalo",  "hombre" },  { "Hilda",    "mujer" },
        { "Bernardo", "hombre" },  { "Alicia",   "mujer" },
        { "Ricardo",  "hombre" },  { "Sofia",    "mujer" },
        { "Mateo",    "hombre" },  { "Lucia",    "mujer" },
        { "Javier",   "hombre" },  { "Camila",   "mujer" }
    };

    //Pool de atributos (uno por categoria para cada sospechoso):

    vector<string> estaturas = { "alto", "bajo", "mediano"};
    vector<string> cabellos = { "cabello rubio", "cabello moreno",
                              "cabello pelirrojo", "cabello canoso" };
    vector<string> pieles = { "piel blanca", "piel triguena", "piel oscura"};
    vector<string> lateralidad = {"zurdo", "diestro"};

    //Mezclar nombres y escoger los primeros 8:
    shuffle( nombresCandidatos.begin(), nombresCandidatos.end(), this->rng );

    for ( int i = 0; i < NUM_SOSPECHOSOS; i++ ) {
        string nom = nombresCandidatos[ i ].first;
        string sexo = nombresCandidatos[ i ].second;

        //Cada Sospecho recibe 5 atributos: sexo + uno por categoria:
        vector<string> attrs;
        attrs.push_back( sexo );
        attrs.push_back( estaturas[ uniform_int_distribution<int>(0, estaturas.size() -1 )(this->rng) ] );
        attrs.push_back( cabellos[ uniform_int_distribution<int>(0, cabellos.size() -1 )(this->rng) ] );
        attrs.push_back( pieles[ uniform_int_distribution<int>(0, pieles.size() -1 )(this->rng) ] );
        attrs.push_back( lateralidad[ uniform_int_distribution<int>(0, lateralidad.size() -1 )(this->rng) ] );

        Sospechoso * s = new Sospechoso( nom, attrs );
        this->tabla[ nom ] = s; // Insertar en tabla Hash.
    }

    //Escoger culpable al azar entre los 8 ingresados:
    uniform_int_distribution<int> distCulp( 0, NUM_SOSPECHOSOS - 1 );
    int idxCulp = distCulp( this-> rng );
    int actual = 0;
    for ( auto& par : this->tabla ) {
        if ( actual == idxCulp ) {
            this->culpable = par.second;
            break;
        }
        actual++;
    }
}

//Busqueda O(1) promedio
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
    return s == this->culpable; //comparacion de punteros mismo objeto
}

Sospechoso * TablaSospechosos :: getCulpable() const {
    return this-> culpable;
}

const unordered_map<string, Sospechoso*>& TablaSospechosos:: getTabla() const {
    return this-> tabla;
}

//muestra cada sospechoso con sus atributos que coinciden con los
//revelados del culpable:

void TablaSospechosos :: mostrar ( const vector<string>& atributosRevelados, const string& nombreDetective ) const {
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
            cout << "--"; //ninguno de los revelados coincide con este sospechoso.
        }
        cout << endl;
    }
}