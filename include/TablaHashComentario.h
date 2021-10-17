#ifndef TablaHashComentario_H
#define TablaHashComentario_H

#include "CSerializable.h"
#include "CComentario.h"
#include "CInvertidaArroba.h"
#include "CInvertidaGato.h"
#include <fstream>

#define CLSID_HASH_COMENTARIO 3333
#define CLSID_ABB_COMENTARIO 3334
#define HASH_SIZE_COMENTARIO 17
using namespace std;


/********** -> Clase ArbolBinarioComentario <- **********/

class ArbolBinarioComentario
{
    public:
        CComentario *dato;

    protected:
        ArbolBinarioComentario *der;
        ArbolBinarioComentario *izq;
        ArbolBinarioComentario *padre;

        ArbolBinarioComentario();
        ArbolBinarioComentario *crearArbol(CComentario *c, ArbolBinarioComentario *padre);
        void insertarNodo(ArbolBinarioComentario *&arbol, CComentario *c, ArbolBinarioComentario *padre);
        ArbolBinarioComentario *eliminarNodo(ArbolBinarioComentario *eliminar);
        ArbolBinarioComentario *getPadre(ArbolBinarioComentario *arbol);
        ArbolBinarioComentario *minimo(ArbolBinarioComentario *arbol);
        void reemplazar(ArbolBinarioComentario *arbol, ArbolBinarioComentario *nuevo);
        void destruirNodo(ArbolBinarioComentario *nodo);

        int getSumaAscii(string cadena);

        friend class ABBComentario;
};

ArbolBinarioComentario::ArbolBinarioComentario()
{
    //ctor
}

ArbolBinarioComentario *ArbolBinarioComentario::crearArbol(CComentario *c, ArbolBinarioComentario *padre){

    ArbolBinarioComentario *ab = new ArbolBinarioComentario();
    ab->dato = c;
    ab->der = nullptr;
    ab->izq = nullptr;
    ab->padre = padre;

    return ab;
}

void ArbolBinarioComentario::insertarNodo(ArbolBinarioComentario *&arbol, CComentario *c, ArbolBinarioComentario *padre){
    if(arbol == nullptr){
        ArbolBinarioComentario *nuevo_arbol = crearArbol(c, padre);
        arbol = nuevo_arbol;
    }else{
        int valorRaiz = getSumaAscii(arbol->dato->getComentario()); //Los acomoda por la suma en ascii de todos sus caracteres
        if(getSumaAscii(c->getComentario()) < valorRaiz){
            insertarNodo(arbol->izq, c, arbol);
        }else{
            insertarNodo(arbol->der, c, arbol);
        }
    }
}

void ArbolBinarioComentario::destruirNodo(ArbolBinarioComentario *nodo){
    nodo->dato = nullptr;
    nodo->izq = nullptr;
    nodo->der = nullptr;
    nodo->padre = nullptr;
    delete nodo;
    nodo = 0;
}


void ArbolBinarioComentario::reemplazar(ArbolBinarioComentario *arbol, ArbolBinarioComentario *nuevo){
    if(arbol->padre){
        if(arbol->padre->izq == arbol){
            arbol->padre->izq = nuevo;
        }else if(arbol->padre->der == arbol){
            arbol->padre->der = nuevo;
        }
    }
    if(nuevo){
        nuevo->padre = arbol->padre;
    }
}


ArbolBinarioComentario *ArbolBinarioComentario::minimo(ArbolBinarioComentario *arbol){
    if(arbol == nullptr){
        return nullptr;
    }
    if(arbol->izq){
        return minimo(arbol->izq);
    }else{
        return arbol;
    }
}

ArbolBinarioComentario *ArbolBinarioComentario::eliminarNodo(ArbolBinarioComentario *eliminar){
    if(eliminar->izq && eliminar->der){
        ArbolBinarioComentario *menor = minimo(eliminar->der);
        eliminar->dato = menor->dato;
        return eliminarNodo(menor);
    }else if(eliminar->izq){
        ArbolBinarioComentario *aux = eliminar->izq;
        reemplazar(eliminar, eliminar->izq);
        destruirNodo(eliminar);
        return getPadre(aux);
    }else if(eliminar->der){
        ArbolBinarioComentario *aux = eliminar->der;
        reemplazar(eliminar, eliminar->der);
        destruirNodo(eliminar);
        return getPadre(aux);
    }else{
        ArbolBinarioComentario *aux = eliminar->padre;
        reemplazar(eliminar, nullptr);
        destruirNodo(eliminar);
        return getPadre(aux);
    }
}

ArbolBinarioComentario *ArbolBinarioComentario::getPadre(ArbolBinarioComentario *arbol){
    if(arbol != nullptr){
        while(arbol->padre != nullptr){
            arbol = arbol->padre;
        }
        return arbol;
    }else{
        return nullptr;
    }
}

int ArbolBinarioComentario::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i];
    }
    return suma;
}





/********** -> Clase ABBComentario <- **********/

class ABBComentario : public CSerializable
{
    private:
        ArbolBinarioComentario *arbol = nullptr;

    public:
        ABBComentario();
        CComentario *busqueda(ArbolBinarioComentario *a, string c);
        void insertar(CComentario *c);
        void eliminar(ArbolBinarioComentario *arbol, string c);
        int getSumaAscii(string cadena);
        ArbolBinarioComentario *getArbol();
        void SalvarRecursivo(fstream& out, ArbolBinarioComentario *arbol);
        int getSize(ArbolBinarioComentario *arbol);
        void imprimirIndiceComentario(ArbolBinarioComentario *a, int cont, int indice);
        void imprimirDeterminados(ArbolBinarioComentario *a, CInvertidaIndices *indices);

        virtual void imprimir(ArbolBinarioComentario *a, int cont);
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

ABBComentario::ABBComentario()
{
    //ctor
}

void ABBComentario::insertar(CComentario *c){
    arbol->insertarNodo(arbol, c, nullptr);
}


CComentario *ABBComentario::busqueda(ArbolBinarioComentario *a, string c){
    if(a == nullptr){
        return nullptr;
    }else if(a->dato->getComentario() == c){
        return a->dato;
    }else if(getSumaAscii(c) < getSumaAscii(a->dato->getComentario())){
        return busqueda(a->izq, c);
    }else{
        return busqueda(a->der, c);
    }
}


void ABBComentario::eliminar(ArbolBinarioComentario *a, string c){

    if(busqueda(arbol, c) !=  nullptr){
        int n = getSumaAscii(a->dato->getComentario()); //valor respecto a la suma de caracteres ascii
        if(a == nullptr){
            return;
        }else if(getSumaAscii(c) < n){
            eliminar(a->izq, c);
        }else if(getSumaAscii(c) > n){
            eliminar(a->der, c);
        }else{
            this->arbol = a->eliminarNodo(a);
        }
    }else{
        cout << endl << "Buscado en caminos para eliminar. Arista" << endl;
    }
}


void ABBComentario::imprimir(ArbolBinarioComentario *a, int cont){
    if(a == nullptr){
        return;
    }else{
        imprimir(a->der, cont + 1);
        for(int i = 0; i < cont; i++){
            cout<<"  ";
        }
        a->dato->imprimir();
        cout<<endl;
        imprimir(a->izq, cont + 1);
        cont = 0;
    }
}

void ABBComentario::imprimirIndiceComentario(ArbolBinarioComentario *a, int cont, int indice){
    if(a == nullptr){

        return;
    }else{
        imprimirIndiceComentario(a->der, cont + 1, indice);
        if(a->dato->getNumComentario() == indice){
            a->dato->imprimir();
        }
        imprimirIndiceComentario(a->izq, cont + 1, indice);
        cont = 0;
    }
}



int ABBComentario::getSize(ArbolBinarioComentario *arbol){
    if(arbol == nullptr){
        return 0;
    }else{
        return (getSize(arbol->izq) + 1 + getSize(arbol->der));
    }
}

void ABBComentario::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSize(arbol) << endl;
    SalvarRecursivo(out, arbol);
}

void ABBComentario::SalvarRecursivo(fstream& out, ArbolBinarioComentario *arbol){
    if(arbol == nullptr){
        return;
    }else{
        arbol->dato->Salvar(out);
        SalvarRecursivo(out, arbol->izq);
        SalvarRecursivo(out, arbol->der);
    }
}

void ABBComentario::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;

    for(int i = 0; i < tam; i++){
        int CLSID;
        in>>CLSID;
        CComentario *c = nullptr;
        if(CLSID == CLSID_COMENTARIO){
            c = new CComentario();
        }
        c->Cargar(in);
        arbol->insertarNodo(arbol, c, nullptr);
    }
}

void ABBComentario::imprimirDeterminados(ArbolBinarioComentario *a, CInvertidaIndices *indices){
    if(a == nullptr){
        return;
    }else{
        imprimirDeterminados(a->der, indices);

        CIteradorIndices it;
        int aux;

        for(it = indices->Begin(); it != indices->End(); it++){
            aux = *it;
            if(a->dato->getNumComentario() == aux){
                a->dato->imprimir();
            }
        }
        imprimirDeterminados(a->izq, indices);
    }
}

int ABBComentario::GetCLSID()
{
    return CLSID_ABB_COMENTARIO;
}

int ABBComentario::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i];
    }
    return suma;
}

ArbolBinarioComentario *ABBComentario::getArbol(){
    return arbol;
}



/********** -> Clase TablaHashComentario <- **********/

class TablaHashComentario : public CSerializable
{
    public:
        ABBComentario *arboles[HASH_SIZE_COMENTARIO];
        CInvertidaArroba *arrobas = new CInvertidaArroba();
        CInvertidaGato *gatos = new CInvertidaGato();

    public:
        TablaHashComentario();
        int fnHash(string ciudad);
        void insertar(CComentario *c);
        CComentario *buscar(string ciudad);
        void eliminar(string ciudad);
        int getSumaAscii(string cadena);
        void crearHash();
        int getSizeHash();
        void imprimirDeterminados(CInvertidaIndices *indices);

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};


TablaHashComentario::TablaHashComentario(){}

int TablaHashComentario::fnHash(string ciudad)
{
    return getSumaAscii(ciudad) % HASH_SIZE_COMENTARIO;
}

void TablaHashComentario::insertar(CComentario *c)
{
    arboles[fnHash(c->getComentario())]->insertar(c);

    string comentario = c->getComentario();
    string arroba = "";
    string gato = "";

    for(size_t i = 0; i < comentario.size(); i++){
        if(comentario[i] == '@'){
            i++;
            while(comentario[i] != ' ' && comentario[i] != '\0'){
                arroba += comentario[i];
                i++;
            }
            arrobas->insertar(arroba, c->getNumComentario());
        }

        if(comentario[i] == '#'){
            i++;
            while(comentario[i] != ' ' && comentario[i] != '\0'){
                gato += comentario[i];
                i++;
            }
            gatos->insertar(gato, c->getNumComentario());
        }
        arroba = "";
        gato = "";
    }
}

CComentario* TablaHashComentario::buscar(string ciudad)
{
    return arboles[fnHash(ciudad)] -> busqueda(arboles[fnHash(ciudad)]->getArbol(), ciudad);
}

void TablaHashComentario::eliminar(string ciudad)
{
    ArbolBinarioComentario *aux = arboles[fnHash(ciudad)]->getArbol();
    arboles[fnHash(ciudad)] -> eliminar(aux, ciudad);
}

void TablaHashComentario::imprimir()
{
    cout << endl << "Size Hash Comentarios: " << getSizeHash() << endl;
    for(size_t i = 0; i < HASH_SIZE_COMENTARIO; i++){

        if(arboles[i]->getArbol() != nullptr){
            //cout << endl << "****************************************************************" << endl;
            cout << endl << "(Index: " << i;
            cout << ", Size: " << arboles[i]->getSize(arboles[i]->getArbol()) << ")";
            arboles[i]->imprimir(arboles[i]->getArbol(), 0);
        }
    }
    arrobas->imprimir();
    gatos->imprimir();
}

void TablaHashComentario::imprimirDeterminados(CInvertidaIndices *indices){
    for(size_t i = 0; i < HASH_SIZE_COMENTARIO; i++){
        if(arboles[i]->getArbol() != nullptr){
            arboles[i]->imprimirDeterminados(arboles[i]->getArbol(), indices);
        }
    }
}

void TablaHashComentario::crearHash()
{
    for(size_t i = 0; i < HASH_SIZE_COMENTARIO; i++){
        ABBComentario *arbol = new ABBComentario();
        arboles[i] = arbol;
    }
}

void TablaHashComentario::Salvar(fstream& out)
{
    fstream salidaArroba;
    fstream salidaGato;

    out << GetCLSID() << endl;
    out << getSizeHash() << endl;

    for(size_t i = 0; i < HASH_SIZE_COMENTARIO; i++){

        if(arboles[i]->getArbol() != nullptr){
            out << i << endl;
            arboles[i]->Salvar(out);
        }
    }
    salidaArroba.open("IndicesArroba.txt", ios::out);
    arrobas->Salvar(salidaArroba);
    salidaArroba.close();

    salidaGato.open("IndicesGato.txt", ios::out);
    gatos->Salvar(salidaGato);
    salidaGato.close();
}

void TablaHashComentario::Cargar(fstream& in)
{
    fstream entradaArroba;
    fstream entradaGato;

    int tam = 0;
    in>>tam;
    crearHash();
    for(int i = 0; i < tam; i++){
        int indice;
        in>>indice;

        int ID_ABB;
        in>>ID_ABB;

        if(ID_ABB == CLSID_ABB_COMENTARIO){
            arboles[indice]->Cargar(in);
        }
    }
    entradaArroba.open("IndicesArroba.txt", ios_base::in);
    int CLSID_A = 0;
    entradaArroba>>CLSID_A;
    arrobas->Cargar(entradaArroba);
    entradaArroba.close();

    entradaGato.open("IndicesGato.txt", ios_base::in);
    int CLSID_G = 0;
    entradaGato>>CLSID_G;
    gatos->Cargar(entradaGato);
    entradaGato.close();

}

int TablaHashComentario::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i];
    }
    return suma;
}

int TablaHashComentario::getSizeHash(){
    int cont = 0;
    for(size_t i = 0; i < HASH_SIZE_COMENTARIO; i++){
        if(arboles[i]->getArbol() != nullptr){
            cont++;
        }
    }
    return cont;
}


int TablaHashComentario::GetCLSID()
{
    return CLSID_HASH_COMENTARIO;
}

#endif


