
#include "Tipos.h"
#include "Constr.h"

Constr::Constr(): _raiz() {}

void Constr::agregarConstruccion(pair<Casilla, Construccion> c) {
    _raiz._construcciones[c.first] = c.second;
}

//limpia _construcciones porque se pasaron a SimCity y limpia _unidas porque paso las de los hijos tambien
void Constr::avanzarTurno() {
    _raiz._construcciones.clear();
    _raiz._unidas.clear();
}

void Constr::unir(Constr con) {
    _raiz._unidas.push_back(con);
}

map<Casilla, Construccion> Constr::construcciones() const {
    map<Casilla, Construccion> res = _raiz._construcciones;
    for(auto constr: _raiz._unidas){
        for(auto c: constr.construcciones())
            //agrega si no hay nada o si hay un comercio(todas tienen el mismo nivel y casa mata comercio)
            if(res.count(c.first) == 0 || res[c.first] == Comercio)
            res[c.first] = c.second;
    }
    return res;
}

Constr::Constr::Nodo::Nodo(): _construcciones(), _unidas() {}