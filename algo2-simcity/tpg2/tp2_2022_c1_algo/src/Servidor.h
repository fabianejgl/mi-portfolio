#ifndef TP_SIMCITY_SERVIDOR_H
#define TP_SIMCITY_SERVIDOR_H

#include "SimCity.h"
#include "Tipos.h"
#include "string_map.h"
#include "Mapa.h"
#include "Constr.h"

////comentarios extensos:
//nuestro disenno original hacia de cuenta que las casas y comercios agregadas
//por Servidor no existian en el SimCity hasta avanzar turno. Ademas, requeria
//que no haya construcciones sueltas antes de unir. Estos tests requieren que
//se contabilice toda construccion y no tienen esta ultima restriccion.
//Consideramos que guardarlos en SimCity y permitir agregar casas y comercios
//individualmente y luego avanzar turno cambiaba demasiado el disenno, por lo
//que reemplazamos el dicc constr en servidor por un objeto de tipo Constr, que
//permite devolver las construcciones que hay en este y los SimCities unidos,
//que son agregadas en Servidor a las guardadas en SimCity cuando hay que responder.

//Agregamos la funcion puedoAvanzar porque ninguna de nuestras funciones
//originales observaba a constr.


class Servidor{
public:
    Servidor();

    void fundar(Jugador j, Mapa m);

    void agCasaxServidor(Jugador j, Casilla c);

    void agComercioxServidor(Jugador j, Casilla c);

    void turnoPorServidor(Jugador j);

    void unir(Jugador j1, Jugador j2);

    bool haySimCity(Jugador j);

    SimCity darSimCity(Jugador j);

    bool esUnible(Jugador j);

    Mapa verMapa(Jugador j) const; //cambio a const

    map<Casilla,Nat> verCasas(Jugador j) const;

    map<Casilla,Nat> verComercios(Jugador j) const;

    Nat verTurno(Jugador j) const;

    Nat verPopularidad(Jugador j) const;

    //lo agregue por huboConstruccion
    bool puedoAvanzar(Jugador j) const;


    //auxiliares para llamar desde el aed2_servidor

     set<Casilla> verCasasEnConjunto(Jugador j) const;
     Nat nivelxCasilla(Jugador j, Casilla c) const;
     set<Casilla> verComerciosEnConjunto(Jugador j) const;

private:
    struct Nodo{
        Nodo();
        Nodo(Mapa m);
        SimCity _simCity = SimCity(Mapa(set<int>(), set<int>()));
        bool _modificable;
        Constr _constr;
    };

    string_map<Nodo> _servidor;
};

#endif //TP_SIMCITY_SERVIDOR_H
