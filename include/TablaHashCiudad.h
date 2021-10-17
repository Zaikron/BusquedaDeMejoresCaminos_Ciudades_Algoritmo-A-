#ifndef TablaHashCiudad_H
#define TablaHashCiudad_H

#include <vector>
#include "CSerializable.h"
#include "CCiudad.h"
#include "TablaHashCamino.h"
#include "TablaHashComentario.h"
#include "CInvertidaIndices.h"
#include "ElementoAEstrella.h"

#define CLSID_HASH_CIUDAD 1111
#define CLSID_ABB_CIUDAD 1112
#define HASH_SIZE_CIUDAD 17
using namespace std;


/********** -> Clase ArbolBinario <- **********/

class ArbolBinario
{
    public:
        CCiudad *dato;
        TablaHashCamino *datosDestino;

    protected:
        ArbolBinario *der;
        ArbolBinario *izq;
        ArbolBinario *padre;

        ArbolBinario();
        ArbolBinario *crearArbol(CCiudad *c, ArbolBinario *padre);
        void insertar(ArbolBinario *&arbol, CCiudad *c, ArbolBinario *padre);
        ArbolBinario *eliminarNodo(ArbolBinario *eliminar);
        ArbolBinario *getPadre(ArbolBinario *arbol);
        ArbolBinario *minimo(ArbolBinario *arbol);
        void reemplazar(ArbolBinario *arbol, ArbolBinario *nuevo);
        void destruirNodo(ArbolBinario *nodo);

        int getSumaAscii(string cadena);

        friend class ABB;
};

ArbolBinario::ArbolBinario()
{
    //ctor
}

ArbolBinario *ArbolBinario::crearArbol(CCiudad *c, ArbolBinario *padre){

    ArbolBinario *ab = new ArbolBinario();
    TablaHashCamino *caminos = new TablaHashCamino();
    ab->dato = c;
    caminos->crearHash();
    ab->datosDestino = caminos;
    ab->der = nullptr;
    ab->izq = nullptr;
    ab->padre = padre;

    return ab;
}

void ArbolBinario::insertar(ArbolBinario *&arbol, CCiudad *c, ArbolBinario *padre){
    if(arbol == nullptr){
        ArbolBinario *nuevo_arbol = crearArbol(c, padre);
        arbol = nuevo_arbol;
    }else{
        int valorRaiz = getSumaAscii(arbol->dato->getNombre()); //Los acomoda por la suma en ascii de todos sus caracteres
        if(getSumaAscii(c->getNombre()) < valorRaiz){
            insertar(arbol->izq, c, arbol);
        }else{
            insertar(arbol->der, c, arbol);
        }
    }
}

void ArbolBinario::destruirNodo(ArbolBinario *nodo){

    cout << "Destruido: " << nodo->dato->getNombre() << endl;
    cout << nodo << endl;
    nodo->dato = nullptr;
    nodo->datosDestino = nullptr;
    nodo->izq = nullptr;
    nodo->der = nullptr;
    nodo->padre = nullptr;
    delete nodo;
    nodo = 0;
}


void ArbolBinario::reemplazar(ArbolBinario *arbol, ArbolBinario *nuevo){
    if(arbol->padre){
        if(arbol->padre->izq == arbol){
            arbol->padre->izq = nuevo;
        }else if(arbol->padre->der == arbol){
            arbol->padre->der = nuevo;
        }
    }

    if(nuevo){
        //cout << "nuevo->padre: " << nuevo->padre << endl;
        //cout << "nuevo: " << nuevo << endl;
        //cout << "arbol->padre " << arbol->padre << endl;
        //cout << "arbol " << arbol << endl;
        nuevo->padre = arbol->padre;
    }
}


ArbolBinario *ArbolBinario::minimo(ArbolBinario *arbol){
    if(arbol == nullptr){
        return nullptr;
    }
    if(arbol->izq){
        return minimo(arbol->izq);
    }else{
        return arbol;
    }
}


ArbolBinario *ArbolBinario::eliminarNodo(ArbolBinario *eliminar){
    if(eliminar->izq && eliminar->der){
        ArbolBinario *menor = minimo(eliminar->der);
        eliminar->dato = menor->dato;
        eliminar->datosDestino = menor->datosDestino;
        return eliminarNodo(menor);
    }else if(eliminar->izq){
        ArbolBinario *aux = eliminar->izq;
        reemplazar(eliminar, eliminar->izq);
        destruirNodo(eliminar);
        return getPadre(aux);
    }else if(eliminar->der){
        ArbolBinario *aux = eliminar->der;
        reemplazar(eliminar, eliminar->der);
        destruirNodo(eliminar);
        return getPadre(aux);
    }else{
        ArbolBinario *aux = eliminar->padre;
        reemplazar(eliminar, nullptr);
        destruirNodo(eliminar);
        return getPadre(aux);
    }
}

ArbolBinario *ArbolBinario::getPadre(ArbolBinario *arbol){
    if(arbol != nullptr){
        while(arbol->padre != nullptr){
            arbol = arbol->padre;
        }
        return arbol;
    }else{
        return nullptr;
    }
}

int ArbolBinario::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i];
    }
    return suma;
}



/********** -> Clase ABB <- **********/

class ABB : public CSerializable
{
    private:
        ArbolBinario *arbol = nullptr;

    public:
        ABB();
        int CLSID_DISTINTA = 0;
        CCiudad *busqueda(ArbolBinario *a, string c);
        TablaHashCamino *busquedaEnArbol(ArbolBinario *a, string c);
        ArbolBinario *busquedaDirArbol(ArbolBinario *a, string c);
        void insertarNodo(CCiudad *c);
        void insertarArista(string origen, CCamino *destino);
        void eliminarNodo(ArbolBinario *arbol, string c);
        void eliminarArista(string origen, string destino);
        void actualizarNodo(string ciudad, int cal, float costo);
        void actualizarArista(string origen, string destino, float dist, float costo, string transporte);
        int getSumaAscii(string cadena);
        ArbolBinario *getArbol();
        void SalvarRecursivo(fstream& out, ArbolBinario *arbol);
        int getSize(ArbolBinario *arbol);
        TablaHashComentario *getComentarios();
        void recorrerIndices(TablaHashComentario *comentarios, CInvertidaIndices *indices);
        void recorrerComentarios(TablaHashComentario *t, int indiceCom);
        vector<ElementoAEstrella> getRutas(vector<ElementoAEstrella> rutas);
        vector<ElementoAEstrella> recorrerGrafo(ArbolBinario *a, vector<ElementoAEstrella> rutas);
        ArbolBinario *getPadre(ArbolBinario *arbol);

        virtual void imprimir(ArbolBinario *a, int cont);
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

ABB::ABB()
{
    //ctor
}

ArbolBinario *ABB::getPadre(ArbolBinario *arbol){
    if(arbol != nullptr){
        while(arbol->padre != nullptr){
            arbol = arbol->padre;
        }
        return arbol;
    }else{
        return nullptr;
    }
}

void ABB::insertarNodo(CCiudad *c){
    arbol->insertar(arbol, c, nullptr);
}

void ABB::insertarArista(string origen, CCamino *destino){
    TablaHashCamino *aux = nullptr;
    aux = busquedaEnArbol(arbol, origen);

    if(aux != nullptr){
        aux->insertar(destino);
        //cout << "Arista insertada" << endl;
    }else{
        cout << "No existe el nodo, no se puede hacer la conexion" << endl;
    }
}

//Se inserta en la tabla no el en arbol
TablaHashCamino *ABB::busquedaEnArbol(ArbolBinario *a, string c){

    if(a == nullptr){
        return nullptr;
    }else if(a->dato->getNombre() == c){
        return a->datosDestino;
    }else if(getSumaAscii(c) < getSumaAscii(a->dato->getNombre())){
        return busquedaEnArbol(a->izq, c);
    }else{
        return busquedaEnArbol(a->der, c);
    }
}

CCiudad *ABB::busqueda(ArbolBinario *a, string c){
    if(a == nullptr){
        return nullptr;
    }else if(a->dato->getNombre() == c){
        return a->dato;
    }else if(getSumaAscii(c) < getSumaAscii(a->dato->getNombre())){
        return busqueda(a->izq, c);
    }else{
        return busqueda(a->der, c);
    }
}

ArbolBinario *ABB::busquedaDirArbol(ArbolBinario *a, string c){
    if(a == nullptr){
        return nullptr;
    }else if(a->dato->getNombre() == c){
        return a;
    }else if(getSumaAscii(c) < getSumaAscii(a->dato->getNombre())){
        return busquedaDirArbol(a->izq, c);
    }else{
        return busquedaDirArbol(a->der, c);
    }
}

void ABB::eliminarNodo(ArbolBinario *a, string c){

    if(busqueda(a, c) !=  nullptr){
        int n = getSumaAscii(a->dato->getNombre()); //valor respecto a la suma de caracteres ascii
        if(a == nullptr){
            return;
        }else if(getSumaAscii(c) < n){
            eliminarNodo(a->izq, c);
        }else if(getSumaAscii(c) > n){
            eliminarNodo(a->der, c);
        }else{
            arbol = a->eliminarNodo(a);
        }
    }else{
        cout << endl << "La ciudad no esta registrada" << endl;
    }
}

void ABB::eliminarArista(string origen, string destino){
    TablaHashCamino *aux = nullptr;
    aux = busquedaEnArbol(arbol, origen);
    if(aux != nullptr){
        if(aux->buscar(destino) != nullptr){
           aux->eliminar(destino);
        }
    }

}

void ABB::actualizarNodo(string ciudad, int cal, float costo){
    CCiudad *aux = busqueda(arbol, ciudad);
    if(aux != nullptr){
        aux->setCalif(cal);
        aux->setCosto(costo);
    }
}

void ABB::actualizarArista(string origen, string destino, float dist, float costo, string transporte){
    TablaHashCamino *auxTable = nullptr;
    auxTable = busquedaEnArbol(arbol, origen);
    if(auxTable != nullptr){
        CCamino *auxCamino = auxTable->buscar(destino);
        if(auxCamino != nullptr){
            auxCamino->setDistacia(dist);
            auxCamino->setCostoV(costo);
            auxCamino->setMedioTrans(transporte);
        }
    }
}

void ABB::imprimir(ArbolBinario *a, int cont){
    if(a == nullptr){
        return;
    }else{
        imprimir(a->der, cont + 1);
        for(int i = 0; i < cont; i++){
            cout<<"  ";
        }
        a->dato->imprimir();
        TablaHashComentario *auxComentarios = new TablaHashComentario();
        auxComentarios = getComentarios();
        CInvertidaIndices *auxIndicesA = auxComentarios->arrobas->Busqueda(a->dato->getNombre());
        //CInvertidaIndices *auxIndicesG = auxComentarios->gatos->Busqueda(a->dato->getNombre());
        recorrerIndices(auxComentarios, auxIndicesA);
        //recorrerIndices(auxComentarios, auxIndicesG);
        a->datosDestino->imprimir();
        cout<<endl;
        imprimir(a->izq, cont + 1);
        cont = 0;
    }
}


void ABB::recorrerIndices(TablaHashComentario *comentarios, CInvertidaIndices *indices)
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

void ABB::recorrerComentarios(TablaHashComentario *t, int indiceCom){
    for(int i = 0; i < HASH_SIZE_COMENTARIO; i++){
        if(t->arboles[i]->getArbol() != nullptr){
            t->arboles[i]->imprimirIndiceComentario(t->arboles[i]->getArbol(), 0, indiceCom);
        }
    }
}

TablaHashComentario *ABB::getComentarios(){
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



int ABB::getSize(ArbolBinario *arbol){

    if(arbol == nullptr){
        return 0;
    }else{
        return (getSize(arbol->izq) + 1 + getSize(arbol->der));
    }
}

void ABB::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSize(arbol) << endl;
    SalvarRecursivo(out, arbol);
}

void ABB::SalvarRecursivo(fstream& out, ArbolBinario *arbol){
    if(arbol == nullptr){
        return;
    }else{
        arbol->dato->Salvar(out);
        arbol->datosDestino->Salvar(out);
        SalvarRecursivo(out, arbol->izq);
        SalvarRecursivo(out, arbol->der);
    }
}

void ABB::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;
    bool band = true;
    int CLSID = 0;

    for(int i = 0; i < tam; i++){
        if(band == true){
            in>>CLSID;
        }
        CCiudad *c = nullptr;
        if(CLSID == CLSID_CIUDAD){
            c = new CCiudad();
        }
        c->Cargar(in);
        arbol = getPadre(arbol);
        arbol->insertar(arbol, c, nullptr);

        int CLSIDCAMINOS;
        in>>CLSIDCAMINOS;
        if(CLSIDCAMINOS == CLSID_HASH_CAMINO){
            arbol = getPadre(arbol);
            arbol = busquedaDirArbol(arbol, c->getNombre());
            //cout << endl << arbol->dato->getNombre() << endl;
            arbol->datosDestino->Cargar(in);
            band = true;
        }else if (CLSIDCAMINOS == CLSID_CIUDAD){
            band = false;
        }else{
            CLSID_DISTINTA = CLSIDCAMINOS;
        }
    }
}

int ABB::GetCLSID()
{
    return CLSID_ABB_CIUDAD;
}

int ABB::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i];
    }
    return suma;
}

ArbolBinario *ABB::getArbol(){
    return arbol;
}

vector<ElementoAEstrella> ABB::getRutas(vector<ElementoAEstrella> rutas){
    return recorrerGrafo(arbol, rutas);
}

vector<ElementoAEstrella> ABB::recorrerGrafo(ArbolBinario *a, vector<ElementoAEstrella> rutas){
    if(a != nullptr){
        recorrerGrafo(a->der, rutas);
        ElementoAEstrella e;
        e.origen = a->dato;
        rutas = a->datosDestino->getRutas(rutas, e);
        recorrerGrafo(a->izq, rutas);
    }
    return rutas;
}


/********** -> Clase TablaHashCiudad <- **********/

class TablaHashCiudad : public CSerializable
{
    public:
        ABB *arboles[HASH_SIZE_CIUDAD];
        vector<ElementoAEstrella> rutas;

    public:
        TablaHashCiudad();
        int fnHash(string ciudad);
        CCiudad *buscar(string ciudad);
        void insertarNodo(CCiudad *c);
        void insertarArista(string origen, CCamino *destino);
        void eliminarNodo(string ciudad);
        void eliminarArista(string origen, string destino);
        void actualizarNodo(string ciudad, int cal, float costo);
        void actualizarArista(string origen, string destino, float dist, float costo, string transporte);
        int getSumaAscii(string cadena);
        void crearHash();
        int getSizeHash();
        vector<ElementoAEstrella> getRutas();

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};


TablaHashCiudad::TablaHashCiudad(){}

int TablaHashCiudad::fnHash(string ciudad)
{
    return getSumaAscii(ciudad) % HASH_SIZE_CIUDAD;
}

void TablaHashCiudad::insertarNodo(CCiudad* c)
{
    arboles[fnHash(c->getNombre())]->insertarNodo(c);
}

void TablaHashCiudad::insertarArista(string origen, CCamino *destino){
    arboles[fnHash(origen)]->insertarArista(origen, destino);
}

CCiudad* TablaHashCiudad::buscar(string ciudad)
{
    return arboles[fnHash(ciudad)] -> busqueda(arboles[fnHash(ciudad)]->getArbol(), ciudad);
}

void TablaHashCiudad::eliminarNodo(string ciudad)
{
    ArbolBinario *aux = arboles[fnHash(ciudad)]->getArbol();
    arboles[fnHash(ciudad)] -> eliminarNodo(aux, ciudad);
}

void TablaHashCiudad::eliminarArista(string origen, string destino)
{
    arboles[fnHash(origen)] -> eliminarArista(origen, destino);
}

void TablaHashCiudad::actualizarNodo(string ciudad, int cal, float costo){
    arboles[fnHash(ciudad)] -> actualizarNodo(ciudad, cal, costo);
}

void TablaHashCiudad::actualizarArista(string origen, string destino, float dist, float costo, string transporte){
    arboles[fnHash(origen)] -> actualizarArista(origen, destino, dist, costo, transporte);
}

void TablaHashCiudad::imprimir()
{
    cout << "Size Hash Ciudades: " << getSizeHash() << endl;
    for(size_t i = 0; i < HASH_SIZE_CIUDAD; i++){

        if(arboles[i]->getArbol() != nullptr){
            cout << endl << "[Index: " << i;
            cout << ", Size: " << arboles[i]->getSize(arboles[i]->getArbol()) << "]";
            arboles[i]->imprimir(arboles[i]->getArbol(), 0);
            cout << endl << "****************************************************************" << endl;
        }
    }
}

void TablaHashCiudad::crearHash()
{
    for(size_t i = 0; i < HASH_SIZE_CIUDAD; i++){
        ABB *arbol = new ABB();
        arboles[i] = arbol;
    }
}

void TablaHashCiudad::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSizeHash() << endl;

    for(size_t i = 0; i < HASH_SIZE_CIUDAD; i++){

        if(arboles[i]->getArbol() != nullptr){
            out << i << endl;
            arboles[i]->Salvar(out);
        }

    }

}

void TablaHashCiudad::Cargar(fstream& in)
{
    int tam = 0;
    int band = false;
    int indice = 0;
    in>>tam;
    crearHash();


    for(int i = 0; i < tam; i++){
        if(band == false){
            in>>indice;
        }

        int ID_ABB = 0;
        in>>ID_ABB;

        if(ID_ABB == CLSID_ABB_CIUDAD){
            arboles[indice]->Cargar(in);
            if(arboles[indice]->CLSID_DISTINTA != 0){
                band = true;
                indice = arboles[indice]->CLSID_DISTINTA;
                arboles[indice]->CLSID_DISTINTA = 0;
            }
        }
    }
}

int TablaHashCiudad::getSumaAscii(string cadena){
    int suma = 0;
    for(size_t i = 0; i < cadena.size(); i++){
        suma += cadena[i] * i+1;
    }
    return suma;
}

int TablaHashCiudad::getSizeHash(){
    int cont = 0;
    for(size_t i = 0; i < HASH_SIZE_CIUDAD; i++){
        if(arboles[i]->getArbol() != nullptr){
            cont++;
        }
    }
    return cont;
}

int TablaHashCiudad::GetCLSID()
{
    return CLSID_HASH_CIUDAD;
}

vector<ElementoAEstrella> TablaHashCiudad::getRutas(){
    for(size_t i = 0; i < HASH_SIZE_CIUDAD; i++){
        if(arboles[i]->getArbol() != nullptr){
            rutas = arboles[i]->getRutas(rutas);
        }
    }
    return rutas;
}

#endif



