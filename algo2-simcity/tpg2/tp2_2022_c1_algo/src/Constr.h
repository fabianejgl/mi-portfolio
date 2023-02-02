#ifndef TP_SIMCITY_CONSTR_H
#define TP_SIMCITY_CONSTR_H

#include "Tipos.h"

//Constr es un arbol similar a SimCity. Tiene las operaciones que
//originalmente modificaban a Constr: agregarConstruccion y avanzarTurno;
//tiene a unir, que en nuestra implementacion original no lo modificaba pero
//nuestra implementacion no es la esperada por los casos de test; y tiene al
//observador construcciones, que da el dicc completo como el constr original.
//Las construcciones en cada nodo tambien se guardan en un dicc.

class Constr {
public:
    Constr();

    void agregarConstruccion(pair<Casilla, Construccion> c);

    void avanzarTurno();

    void unir(Constr con);

    map<Casilla, Construccion> construcciones() const;

private:
    struct Nodo{
        Nodo();
        map<Casilla, Construccion> _construcciones;
        list<Constr> _unidas;
    };

    Nodo _raiz;

};

#endif //TP_SIMCITY_CONSTR_H
