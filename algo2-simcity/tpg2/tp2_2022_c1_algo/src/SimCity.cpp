
#include "SimCity.h"
#include "Tipos.h"


//agregue construccion a tipos
void SimCity::avanzarTurno(map<Casilla, Construccion> cs) {
    _turnos++;
    _raiz._casas = casas();
    _raiz._comercios = comercios();

    //agrego construccion
    for(auto c: cs){
        if(c.second == Casa)
            _raiz._casas[c.first] = 0;
        else
            _raiz._comercios[c.first]= maxNivelxManhattan(_raiz._casas, c.first);
    }

    //aumentar nivel casa, luego comercio
    for(auto casa: _raiz._casas)
        _raiz._casas[casa.first]++;

    for(auto comercio: _raiz._comercios)
        _raiz._comercios[comercio.first]++;

}

SimCity::SimCity(Mapa mapa): _popularidad(), _turnos(), _raiz(mapa) {}

void SimCity::unir(SimCity s) {
    _turnos = max(_turnos, s.antiguedad());
    _popularidad += s.popularidad() + 1;
    _raiz._unidas.push_back(s);
}

Mapa SimCity::dameMapa() const{
    Mapa res = _raiz._mapa;

    for(SimCity simUnida: _raiz._unidas){
        res.unir(simUnida.dameMapa());
    }
    return res;
}

map<Casilla, Nat> SimCity::casas() const {
    map<Casilla,Nat> res = _raiz._casas;
    for(SimCity simUnida: _raiz._unidas){
        for(auto casa: simUnida.casas())
            insertarConstruccionEnDiccionario(res, casa);
    }
    return res;
}

map<Casilla, Nat> SimCity::comercios() const {
    map<Casilla,Nat> casas = this->casas();
    map<Casilla,Nat> res = _raiz._comercios;
    for(SimCity simUnida: _raiz._unidas){
        for(auto comercio: simUnida.comercios()){
            insertarConstruccionEnDiccionario(res, comercio);
        }
    }
    //quitar los que se solapan con casas
    for(auto casa: casas){
        res.erase(casa.first);
    }
    //subir nivel
    for(auto comercio: res){
        Nat nivel = maxNivelxManhattan(casas, comercio.first);
        if(comercio.second < nivel)
            res[comercio.first] = nivel;
    }
    return res;
}

Nat SimCity::popularidad() const {
    return _popularidad;
}

Nat SimCity::antiguedad() const {
    return _turnos;
}



SimCity::SimCity::Nodo::Nodo(): _casas(), _comercios(), _unidas() {
    _mapa = Mapa(set<int>(), set<int>());
}
SimCity::SimCity::Nodo::Nodo(Mapa m): _casas(), _comercios(), _unidas() {
    _mapa = m;
}

///auxiliares


void SimCity::insertarConstruccionEnDiccionario(map<Casilla, Nat> &construcciones, pair<Casilla, Nat> c) {
    map<Casilla,Nat>::iterator it;
    it = construcciones.find(c.first);
    if(it == construcciones.end())
        construcciones.insert(c);
    else
    if(it->second < c.second)
        it->second = c.second;
}

Nat SimCity::maxNivelxManhattan(const map<Casilla, Nat>& casas, Casilla c) {
    Nat nivel = 0;
    for(auto casa : casas){
        int dist = distManhattan(c, casa.first);
        if(dist <= 3 && nivel < casa.second)
            nivel = casa.second;
    }
    return nivel;
}

int SimCity::distManhattan(Casilla a, Casilla b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}