#ifndef TIPOS_H
#define TIPOS_H

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <set>
#include <tuple>
#include <string>
#include <map>
#include <list>
#include <vector>

using namespace std;

using Nat = unsigned int;

using Casilla = pair<int, int>;

using Jugador = string;

enum Direccion {
    Horizontal, Vertical
};

//agregamos el tipo Construccion para minimizar errores
enum Construccion {
    Casa, Comercio
};

#endif // TIPOS_H

