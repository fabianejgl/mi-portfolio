#include "aed2_Servidor.h"
#include "Mapa.h"
#include "Servidor.h"
#include "SimCity.h"

aed2_Servidor::aed2_Servidor() {
    _servidor = Servidor();
}

void aed2_Servidor::nuevaPartida(Jugador j, set<int> horizontales, set<int> verticales) {
    _servidor.fundar(j, Mapa(horizontales, verticales));
}

void aed2_Servidor::agregarCasa(Jugador j, Casilla c) {
    _servidor.agCasaxServidor(j, c);
}

void aed2_Servidor::agregarComercio(Jugador j, Casilla c) {
    _servidor.agComercioxServidor(j, c);
}

void aed2_Servidor::avanzarTurno(Jugador j) {
    _servidor.turnoPorServidor(j);
}

void aed2_Servidor::unir(Jugador j1, Jugador j2) {
    _servidor.unir(j1, j2);
}

set<int> aed2_Servidor::riosHorizontales(Jugador j) const {
    return _servidor.verMapa(j).riosHorizontales();
}

set<int> aed2_Servidor::riosVerticales(Jugador j) const {
    return _servidor.verMapa(j).riosVerticales();
}

set<Casilla> aed2_Servidor::casas(Jugador j) const {
    return _servidor.verCasasEnConjunto(j);
}

set<Casilla> aed2_Servidor::comercios(Jugador j) const {
    return  _servidor.verComerciosEnConjunto(j);
}

Nat aed2_Servidor::nivel(Jugador j, Casilla c) const {
    return _servidor.nivelxCasilla(j,c);
}

bool aed2_Servidor::huboConstruccion(Jugador j) const {
    return _servidor.puedoAvanzar(j);
}

Nat aed2_Servidor::popularidad(Jugador j) const {
    return _servidor.verPopularidad(j);
}

Nat aed2_Servidor::antiguedad(Jugador j) const {
    return _servidor.verTurno(j);
}