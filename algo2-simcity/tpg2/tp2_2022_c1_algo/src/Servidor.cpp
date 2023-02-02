#include "Servidor.h"

#include <utility>
#include "Tipos.h"
#include "Mapa.h"

Servidor::Servidor(): _servidor() {}

void Servidor::fundar(Jugador j, Mapa m) {
    _servidor[j] = Nodo(m);
}

void Servidor::agCasaxServidor(Jugador j, Casilla c) {
    _servidor[j]._constr.agregarConstruccion(make_pair(c, Casa));
}

void Servidor::agComercioxServidor(Jugador j, Casilla c) {
    _servidor[j]._constr.agregarConstruccion(make_pair(c, Comercio));
}

void Servidor::turnoPorServidor(Jugador j) {
    _servidor[j]._simCity.avanzarTurno(_servidor[j]._constr.construcciones());
    _servidor[j]._constr.avanzarTurno();
}

void Servidor::unir(Jugador j1, Jugador j2) {
    Nodo *d1 = &_servidor[j1];
    Nodo *d2 = &_servidor[j2];
    if(d1->_modificable && d2->_modificable){
        d1->_simCity.unir(d2->_simCity);
        d2->_modificable = false;
        d1->_constr.unir(d2->_constr);
    }
    //si no se pueden unir no hago nada
}

bool Servidor::haySimCity(Jugador j) {
    return _servidor.count(j) == 1;
}


SimCity Servidor::darSimCity(Jugador j) {
    return _servidor[j]._simCity;
}

bool Servidor::esUnible(Jugador j)  {
    return _servidor[j]._modificable;
}

Mapa Servidor::verMapa(Jugador j) const{
    return _servidor.at(j)._simCity.dameMapa();
}

//agregar constr con nivel 0 porque SimCity solo tiene nivel 1+
map<Casilla, Nat> Servidor::verCasas(Jugador j) const {
    map<Casilla, Nat> res = _servidor.at(j)._simCity.casas();
    for(auto c: _servidor.at(j)._constr.construcciones()){
        // agregar si son casas y no se solapan(porque las casas ya existentes tienen mas nivel y les ganan)
        if(c.second == Casa && res.count(c.first) == 0)
            res[c.first] = 0;
    }
    return res;
}

//agregar constr con nivel 0 porque SimCity solo tiene nivel 1+
map<Casilla, Nat> Servidor::verComercios(Jugador j) const {
    map<Casilla, Nat> casas = verCasas(j);
    map<Casilla, Nat> res = _servidor.at(j)._simCity.comercios();
    for(auto c: _servidor.at(j)._constr.construcciones()){
        // agregar si son comercios y no se solapan(porque los comercios ya existentes tienen mas nivel y les ganan)
        if(c.second == Comercio && res.count(c.first) == 0)
            res[c.first] = SimCity::maxNivelxManhattan(casas, c.first);
    }
    //quitar si se solapan con casas
    for(auto casa: casas){
        res.erase(casa.first);
    }
    return res;
}

Nat Servidor::verTurno(Jugador j) const {
    return _servidor.at(j)._simCity.antiguedad(); //_turnos = antiguedad()
}

Nat Servidor::verPopularidad(Jugador j) const {
    return _servidor.at(j)._simCity.popularidad();
}

bool Servidor::puedoAvanzar(Jugador j) const {
    Nodo d1 = _servidor.at(j);
    return !(d1._constr.construcciones().empty());
}

Servidor::Servidor::Nodo::Nodo(Mapa m): _modificable(true), _constr() {
    _simCity = SimCity(m);
}

Servidor::Servidor::Nodo::Nodo(): _modificable(true), _constr() {}



//auxiliares

//auxiliar para aed2_Servidor, basicamente claves(verCasas(j))
 set<Casilla> Servidor::verCasasEnConjunto(Jugador j) const{
     set<Casilla> res;
     map<Casilla ,Nat> casas = verCasas(j);
     for(std::map<Casilla ,Nat>::iterator it = casas.begin(); it != casas.end(); it++){
         res.insert(it->first);
     }
     return res;
}

//auxiliar para aed2_Servidor, basicamente claves(verComercios(j))
set<Casilla> Servidor::verComerciosEnConjunto(Jugador j) const{
    set<Casilla> res;
    map<Casilla ,Nat> comercios = verComercios(j);
    for(std::map<Casilla ,Nat>::iterator it = comercios.begin(); it != comercios.end(); it++){
        res.insert(it->first);
    }
    return res;
}

//auxiliar para aed2_Servidor
 Nat Servidor::nivelxCasilla(Jugador j, Casilla c) const{

    map<Casilla, Nat> casas = verCasas(j);
    map<Casilla, Nat> comercios = verComercios(j);
    map<Casilla, Nat>::iterator it = casas.find(c);
    if(it != casas.end())
        return it->second;
    else{
        it = comercios.find(c);
        return it->second;
    }
}