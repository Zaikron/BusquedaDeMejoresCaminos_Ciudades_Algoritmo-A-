#ifndef TablaHashCamino_H
#define TablaHashCamino_H

#include "CSerializable.h"
#include "CCamino.h"
#include "TablaHashComentario.h"
#include "CInvertidaIndices.h"
#include <vector>
#include "ElementoAEstrella.h"

#define CLSID_HASH_CAMINO 2222
#define CLSID_ABB_CAMINO 2223
#define HASH_SIZE_CAMINO 17
using namespace std;


/********** -> Clase ArbolBinarioCamino <- **********/

class ArbolBinarioCamino
{
    public:
        CCamino *dato;

    protected:
        ArbolBinarioCamino *der;
        ArbolBinarioCamino *izq;
        ArbolBinarioCamino *padre;

        ArbolBinarioCamino();
        ArbolBinarioCamino *crearArbol(CCamino *c, ArbolBinarioCamino *padre);
        void insertarNodo(ArbolBinarioCamino *&arbol, CCamino *c, ArbolBinarioCamino *padre);
        ArbolBinarioCamino *eliminarNodo(ArbolBinarioCamino *eliminar);
        ArbolBinarioCamino *getPadre(ArbolBinarioCamino *arbol);
        ArbolBinarioCamino *minimo(ArbolBinarioCamino *arbol);
        void reemplazar(ArbolBinarioCamino *arbol, ArbolBinarioCamino *nuevo);
        void destruirNodo(ArbolBinarioCamino *nodo);

        int getSumaAscii(string cadena);

        friend class ABBCamino;
};

ArbolBinarioCamino::ArbolBinarioCamino()
{
    //ctor
}

ArbolBinarioCamino *ArbolBinarioCamino::crearArbol(CCamino *c, ArbolBinarioCamino *padre){

    ArbolBinarioCamino *ab = new ArbolBinarioCamino();
    ab->dato = c;
    ab->der = nullptr;
    ab->izq = nullptr;
    ab->padre = padre;

    return ab;
}

void ArbolBinarioCamino::insertarNodo(ArbolBinarioCamino *&arbol, CCamino *c, ArbolBinarioCamino *padre){
    if(arbol == nullptr){
        ArbolBinarioCamino *nuevo_arbol = crearArbol(c, padre);
        arbol = nuevo_arbol;
    }else{
        int valorRaiz = getSumaAscii(arbol->dato->getCiudad()); //Los acomoda por la suma en ascii de todos sus caracteres
        if(getSumaAscii(c->getCiudad()) < valorRaiz){
            insertarNodo(arbol->izq, c, arbol);
        }else{
            insertarNodo(arbol->der, c, arbol);
        }
    }
}

void ArbolBinarioCamino::destruirNodo(ArbolBinarioCamino *nodo){
    cout << "Destruido[Camino]: " << nodo->dato->getCiudad() << endl;
    cout << nodo << endl;
    nodo->dato = nullptr;
    nodo->izq = nullptr;
    nodo->der = nullptr;
    nodo->padre = nullptr;
    delete nodo;
    nodo = 0;
}


void ArbolBinarioCamino::reemplazar(ArbolBinarioCamino *arbol, ArbolBinarioCamino *nuevo){
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


ArbolBinarioCamino *ArbolBinarioCamino::minimo(ArbolBinarioCamino *arbol){
    if(arbol == nullptr){
        return nullptr;
    }
    if(arbol->izq){
        return minimo(arbol->izq);
    }else{
        return arbol;
    }
}

ArbolBinarioCamino *ArbolBinarioCamino::eliminarNodo(ArbolBinarioCamino *eliminar){
    if(eliminar->izq && eliminar->der){
        ArbolBinarioCamino *menor = minimo(eliminar->der);
        eliminar->dato = menor->dato;
        return eliminarNodo(menor);
    }else if(eliminar->izq){
        ArbolBinarioCamino *aux = eliminar->izq;
        reemplazar(eliminar, eliminar->izq);
        destruirNodo(eliminar);
        return getPadre(aux);
    }else if(eliminar->der){
        ArbolBinarioCamino *aux = eliminar->der;
        reemplazar(eliminar, eliminar->der);
        destruirNodo(eliminar);
        return getPadre(aux);
    }else{
        ArbolBinarioCamino *aux = eliminar->padre;
        reemplazar(eliminar, nullptr);
        destruirNodo(eliminar);
        return getPadre(aux);
    }
}

ArbolBinarioCamino *ArbolBinarioCamino::getPadre(ArbolBinarioCamino *arbol){
    if(arbol != nullptr){
        while(arbol->padre != nullptr){
            arbol = arbol->padre;
        }
        return arbol;
    }else{
        return nullptr;
    }
}

int ArbolBinarioCamino::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i];
    }
    return suma;
}





/********** -> Clase ABBCamino <- **********/

class ABBCamino : public CSerializable
{
    private:
        ArbolBinarioCamino *arbol = nullptr;
        int tam = 0;

    public:
        ABBCamino();
        CCamino *busqueda(ArbolBinarioCamino *a, string c);
        void insertar(CCamino *c);
        void eliminar(ArbolBinarioCamino *arbol, string c);
        int getSumaAscii(string cadena);
        ArbolBinarioCamino *getArbol();
        void SalvarRecursivo(fstream& out, ArbolBinarioCamino *arbol);
        int getSize(ArbolBinarioCamino *arbol);
        TablaHashComentario *getComentarios();
        void recorrerIndices(TablaHashComentario *comentarios, CInvertidaIndices *indices);
        void recorrerComentarios(TablaHashComentario *t, int indiceCom);
        vector<ElementoAEstrella> getRutas(vector<ElementoAEstrella> rutas, ElementoAEstrella e);
        vector<ElementoAEstrella> recorrerGrafo(ArbolBinarioCamino *a, vector<ElementoAEstrella> rutas, ElementoAEstrella e);
        ArbolBinarioCamino *getPadre(ArbolBinarioCamino *arbol);

        virtual void imprimir(ArbolBinarioCamino *a, int cont);
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

ABBCamino::ABBCamino()
{
    //ctor
}

ArbolBinarioCamino *ABBCamino::getPadre(ArbolBinarioCamino *arbol){
    if(arbol != nullptr){
        while(arbol->padre != nullptr){
            arbol = arbol->padre;
        }
        return arbol;
    }else{
        return nullptr;
    }
}

void ABBCamino::insertar(CCamino *c){
    arbol->insertarNodo(arbol, c, nullptr);
}


CCamino *ABBCamino::busqueda(ArbolBinarioCamino *a, string c){
    if(a == nullptr){
        return nullptr;
    }else if(a->dato->getCiudad() == c){
        return a->dato;
    }else if(getSumaAscii(c) < getSumaAscii(a->dato->getCiudad())){
        return busqueda(a->izq, c);
    }else{
        return busqueda(a->der, c);
    }
}


void ABBCamino::eliminar(ArbolBinarioCamino *a, string c){

    if(busqueda(arbol, c) !=  nullptr){
        int n = getSumaAscii(a->dato->getCiudad()); //valor respecto a la suma de caracteres ascii
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


void ABBCamino::imprimir(ArbolBinarioCamino *a, int cont){
    if(a == nullptr){
        return;
    }else{
        imprimir(a->der, cont + 1);
        for(int i = 0; i < cont; i++){
            cout<<"  ";
        }
        a->dato->imprimir();
        TablaHashComentario *auxComentarios = getComentarios();
        CInvertidaIndices *auxIndicesA = auxComentarios->arrobas->Busqueda(a->dato->getCiudad());
        //CInvertidaIndices *auxIndicesG = auxComentarios->gatos->Busqueda(a->dato->getCiudad());
        recorrerIndices(auxComentarios, auxIndicesA);
        //recorrerIndices(auxComentarios, auxIndicesG);
        cout<<endl;
        imprimir(a->izq, cont + 1);
        cont = 0;
    }
}

void ABBCamino::recorrerIndices(TablaHashComentario *comentarios, CInvertidaIndices *indices)
{
    CIteradorIndices it;
    int aux;

    if(indices != nullptr){
        for(it = indices->Begin(); it != indices->End(); it++){
            aux = *it;
            recorrerComentarios(comentarios, aux);
        }
        cout << endl;
    }


}

void ABBCamino::recorrerComentarios(TablaHashComentario *t, int indiceCom){
    for(int i = 0; i < HASH_SIZE_COMENTARIO; i++){
        if(t->arboles[i]->getArbol() != nullptr){
            t->arboles[i]->imprimirIndiceComentario(t->arboles[i]->getArbol(), 0, indiceCom);
        }
    }
}

TablaHashComentario *ABBCamino::getComentarios(){
    fstream entradaComentarios;

    TablaHashComentario *comentarios = new TablaHashComentario();
    comentarios->crearHash();

    entradaComentarios.open("Comentarios.txt", ios_base::in);
    int CLSID_COM = 0;
    entradaComentarios>>CLSID_COM;

    comentarios->Cargar(entradaComentarios);
    entradaComentarios.close();
    return comentarios;
}



int ABBCamino::getSize(ArbolBinarioCamino *arbol){
    if(arbol == nullptr){
        return 0;
    }else{
        return (getSize(arbol->izq) + 1 + getSize(arbol->der));
    }
}

void ABBCamino::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSize(arbol) << endl;
    SalvarRecursivo(out, arbol);
}

void ABBCamino::SalvarRecursivo(fstream& out, ArbolBinarioCamino *arbol){
    if(arbol == nullptr){
        return;
    }else{
        arbol->dato->Salvar(out);
        SalvarRecursivo(out, arbol->izq);
        SalvarRecursivo(out, arbol->der);
    }
}

void ABBCamino::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;

    for(int i = 0; i < tam; i++){
        int CLSID;
        in>>CLSID;
        CCamino *c = nullptr;
        if(CLSID == CLSID_CAMINO){
            c = new CCamino();
        }
        c->Cargar(in);
        arbol = getPadre(arbol);
        arbol->insertarNodo(arbol, c, nullptr);
    }
}

int ABBCamino::GetCLSID()
{
    return CLSID_ABB_CAMINO;
}

int ABBCamino::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i];
    }
    return suma;
}

ArbolBinarioCamino *ABBCamino::getArbol(){
    return arbol;
}


vector<ElementoAEstrella> ABBCamino::getRutas(vector<ElementoAEstrella> rutas, ElementoAEstrella e){
    return recorrerGrafo(arbol, rutas, e);
}
vector<ElementoAEstrella> prueba;
vector<ElementoAEstrella> ABBCamino::recorrerGrafo(ArbolBinarioCamino *a, vector<ElementoAEstrella> rutas, ElementoAEstrella e){
    if(a != nullptr){
        recorrerGrafo(a->der, rutas, e);
        e.destino = a->dato;
        prueba.push_back(e);
        rutas.push_back(e);
        recorrerGrafo(a->izq, rutas, e);
    }
    return prueba;
}




/********** -> Clase TablaHashCamino <- **********/

class TablaHashCamino : public CSerializable
{
    public:
        ABBCamino *arboles[HASH_SIZE_CAMINO];

    public:
        TablaHashCamino();
        int fnHash(string ciudad);
        void insertar(CCamino *c);
        CCamino *buscar(string ciudad);
        void eliminar(string ciudad);
        int getSumaAscii(string cadena);
        void crearHash();
        int getSizeHash();
        vector<ElementoAEstrella> getRutas(vector<ElementoAEstrella> rutas, ElementoAEstrella e);

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};


TablaHashCamino::TablaHashCamino(){}

int TablaHashCamino::fnHash(string ciudad)
{
    return getSumaAscii(ciudad) % HASH_SIZE_CAMINO;
}

void TablaHashCamino::insertar(CCamino* c)
{
    arboles[fnHash(c->getCiudad())]->insertar(c);
}

CCamino* TablaHashCamino::buscar(string ciudad)
{
    return arboles[fnHash(ciudad)] -> busqueda(arboles[fnHash(ciudad)]->getArbol(), ciudad);
}

void TablaHashCamino::eliminar(string ciudad)
{
    ArbolBinarioCamino *aux = arboles[fnHash(ciudad)]->getArbol();
    arboles[fnHash(ciudad)] -> eliminar(aux, ciudad);
}

void TablaHashCamino::imprimir()
{
    cout << endl << "   Size Hash Caminos: " << getSizeHash() << endl;
    for(size_t i = 0; i < HASH_SIZE_CAMINO; i++){

        if(arboles[i]->getArbol() != nullptr){
            //cout << endl << "****************************************************************" << endl;
            cout << endl << "   (Index: " << i;
            cout << ", Size: " << arboles[i]->getSize(arboles[i]->getArbol()) << ")";
            arboles[i]->imprimir(arboles[i]->getArbol(), 0);
        }
    }
}

void TablaHashCamino::crearHash()
{
    for(size_t i = 0; i < HASH_SIZE_CAMINO; i++){
        ABBCamino *arbol = new ABBCamino();
        arboles[i] = arbol;
    }
}

void TablaHashCamino::Salvar(fstream& out)
{

    if(getSizeHash() != 0){
        out << GetCLSID() << endl;
        out << getSizeHash() << endl;

        for(size_t i = 0; i < HASH_SIZE_CAMINO; i++){

            if(arboles[i]->getArbol() != nullptr){
                out << i << endl;
                arboles[i]->Salvar(out);
            }
        }
    }


}

void TablaHashCamino::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;
    crearHash();
    for(int i = 0; i < tam; i++){
        int indice;
        in>>indice;

        int ID_ABB;
        in>>ID_ABB;

        if(ID_ABB == CLSID_ABB_CAMINO){
            arboles[indice]->Cargar(in);
        }
    }
}

int TablaHashCamino::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i] * (i+1);
    }
    return suma;
}

int TablaHashCamino::getSizeHash(){
    int cont = 0;
    for(size_t i = 0; i < HASH_SIZE_CAMINO; i++){
        if(arboles[i]->getArbol() != nullptr){
            cont++;
        }
    }
    return cont;
}


int TablaHashCamino::GetCLSID()
{
    return CLSID_HASH_CAMINO;
}

vector<ElementoAEstrella> TablaHashCamino::getRutas(vector<ElementoAEstrella> rutas, ElementoAEstrella e){
    for(size_t i = 0; i < HASH_SIZE_CAMINO; i++){
        if(arboles[i]->getArbol() != nullptr){
            rutas = arboles[i]->getRutas(rutas, e);
        }
    }
    return rutas;
}

#endif


