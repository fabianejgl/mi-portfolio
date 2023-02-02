
#ifndef TP_SIMCITY_MAPA_H
#define TP_SIMCITY_MAPA_H
#include "Tipos.h"

class Mapa {
public:
    Mapa(set<int> hs, set<int> vs);
    void unir(Mapa m2);
    set<int> riosHorizontales() const;
    set<int> riosVerticales() const;

private:
    set<int> _horizontales;
    set<int> _verticales;
};


#endif //TP_SIMCITY_MAPA_H
