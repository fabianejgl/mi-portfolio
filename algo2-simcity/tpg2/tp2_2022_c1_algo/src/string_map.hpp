#include "string_map.h"

template <typename T>
string_map<T>::string_map(): raiz(nullptr), _size(){}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    _size = d.size();
    if(d.raiz == nullptr){
        raiz = nullptr;
    }else{
        raiz = new Nodo(nullptr);
        raiz->copiar(*d.raiz);
    }
    return *this;
}

template <typename T>
string_map<T>::~string_map(){
    if(raiz != nullptr)
        raiz->destruir();
}

template <typename T>
void string_map<T>::Nodo::destruir() {
    if(definicion!= nullptr)
        delete definicion;
    for(Nodo* nodo: siguientes){
        if(nodo!= nullptr)
            nodo->destruir();
    }
    delete this;
}

template <typename T>
T& string_map<T>::operator[](const string& clave){
    if(count(clave) == 0)
        insert(make_pair(clave, T()));
    return at(clave);
}

template <typename T>
void string_map<T>::insert(const pair<string, T> & par) {
    string clave = par.first;
    T* def = new T;
    *def = par.second;
    int aumentar = 0;
    if (raiz == nullptr)
        raiz = new Nodo(nullptr);
    Nodo* actual = raiz;
    for (char i : clave){
        if((actual->siguientes)[i] == nullptr){
            (actual->siguientes)[i] = new Nodo(nullptr);
            aumentar = 1;
        }
        actual = (actual->siguientes)[i];
    }
    if(actual->definicion != nullptr)
        delete actual->definicion;
    actual->definicion = def;
    _size+=aumentar;
}

template <typename T>
int string_map<T>::count(const string& clave) const{
    if (raiz == nullptr)
        return 0;
    Nodo* actual = raiz;
    for (char i : clave){
        if((actual->siguientes)[i] == nullptr)
            return 0;
        actual = (actual->siguientes)[i];
    }
    if(actual->definicion == nullptr)
        return 0;
    return 1;
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    Nodo* actual = raiz;
    for (char i : clave)
        actual = (actual->siguientes)[i];
    return *(actual->definicion);
}

template <typename T>
T& string_map<T>::at(const string& clave) {
    Nodo* actual = raiz;
    for (char i : clave)
        actual = (actual->siguientes)[i];
    return *(actual->definicion);
}

template <typename T>
void string_map<T>::erase(const string& clave) {
    Nodo* actual = raiz;
    vector<string_map<T>::Nodo*> nodos;
    nodos.push_back(nullptr);
    for (char i : clave){
        nodos.push_back(actual);
        actual = (actual->siguientes)[i];
    }
    delete actual->definicion;
    actual->definicion = nullptr;
    _size--;
    for (int i = nodos.size()-1; i >= 0; i--){
        if(i == 0)
            actual = raiz;
        else
            actual = (nodos[i]->siguientes)[clave[i-1]];
        if(actual->definicion != nullptr)
            return;
        for(int j = 0; j < 256; j++)
            if((actual->siguientes)[j] != nullptr)
                return;
        if(i == 0)
            raiz = nullptr;
        else
            (nodos[i]->siguientes)[clave[i-1]] = nullptr;
        if(actual->definicion != nullptr)
            delete actual->definicion;
        delete actual;
    }
}

template <typename T>
int string_map<T>::size() const{
    return _size;
}

template <typename T>
bool string_map<T>::empty() const{
    return raiz == nullptr;
}

template <typename T>
string_map<T>::Nodo::Nodo(T *def): definicion(def)
{
    for( int i = 0; i < 256; i++ )
        siguientes.push_back(nullptr);
}

template <typename T>
void string_map<T>::Nodo::copiar(const Nodo og) {
    if(og.definicion == nullptr)
        definicion = nullptr;
    else{
        definicion = new T;
        *definicion = *(og.definicion);
    }
    for( int i = 0; i < 256; i++ ){
        if(og.siguientes[i] != nullptr){
            siguientes[i] = new Nodo(nullptr);
            siguientes[i]->copiar(*og.siguientes[i]);
        }
    }
}
