
#include "Mapa.h"
#include "Tipos.h"

Mapa::Mapa(set<int> hs, set<int> vs): _horizontales(hs), _verticales(vs){}

void Mapa::unir(Mapa m2) {
    for (auto elem: m2._horizontales) {
        _horizontales.insert(elem);
    }

    for (auto elem: m2._verticales) {
        _verticales.insert(elem);
    }
}
set<int> Mapa::riosHorizontales() const{
    return _horizontales;
}
set<int> Mapa::riosVerticales() const{
    return _verticales;
}
