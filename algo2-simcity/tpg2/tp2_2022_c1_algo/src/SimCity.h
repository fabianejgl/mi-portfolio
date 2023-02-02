
#ifndef TP_SIMCITY_SIMCITY_H
#define TP_SIMCITY_SIMCITY_H

#include "Tipos.h"
#include "Mapa.h"

//El SimCity se mantuvo bastante similar; cambiamos ciertas cosas pero la idea
//es la misma. La implementacion original usaba una tupla con tres arboles;
//ahora usamos un solo arbol donde cada nodo contiene casas, comercios, mapa y
//una lista de punteros a los hijos. En este caso, los hijos son otros SimCity.
//Esto produciria bastante aliasing, pero el servidor se ocupa de no tener
//problemas con eso y el cambio nos permite usar recursividad.
//Decidimos hacer este cambio porque en el original, los arboles eran los tres
//isomorfos y parecia extranno unir tres veces distintas partes de dos
//SimCities cuando con una sola vez alcanza.
//Popularidad y turnos se mantienen iguales.
//Incluimos ciertas auxiliares.

class SimCity {
public:
    SimCity(Mapa mapa);

    void avanzarTurno(map<Casilla, Construccion> cs);

    void unir(SimCity s);

    Mapa dameMapa() const;

    map<Casilla, Nat> casas() const;

    map<Casilla, Nat> comercios() const;

    Nat popularidad() const;

    Nat antiguedad() const;



    static Nat maxNivelxManhattan(const map<Casilla, Nat>& casas, Casilla c) ;

    static int distManhattan(Casilla a, Casilla b) ;

private:
    struct Nodo{
        Nodo();
        Nodo(Mapa m);
        map<Casilla, Nat> _casas;
        map<Casilla, Nat> _comercios;
        Mapa _mapa = Mapa(set<int>(), set<int>());
        //no paso nodo sino SimCity
        list<SimCity> _unidas;
    };
    //un solo arbol en vez de tres
    Nodo _raiz;
    Nat _popularidad;
    Nat _turnos;


    static void insertarConstruccionEnDiccionario(map<Casilla, Nat>& construcciones, pair<Casilla, Nat> c) ;
};

#endif //TP_SIMCITY_SIMCITY_H
