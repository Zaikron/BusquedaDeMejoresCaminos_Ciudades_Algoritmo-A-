#ifndef CNODO_H
#define CNODO_H

#include <iostream>
#include "CListaCamino.h"
#include "CCiudad.h"
#include "CSerializable.h"
#include "TablaHashCiudad.h"

using namespace std;

class CNodoCiudad
{
    public:
        CCiudad *ciudad;
        CListaCamino *pLista;

    protected:
        CNodoCiudad *pSig;
        CNodoCiudad *pAnt;

        CNodoCiudad();
        void insertarN(CCiudad *c);
        CCiudad *eliminarAdelante();
        void insertarLista(CCiudad *c);

        //Acceso de los metodos protected a la clase CListaCiudad
        friend class CListaCiudad;
        friend class CIteradorCiudad;

};

/************ Metodos ************/

CNodoCiudad::CNodoCiudad(){
}

void CNodoCiudad::insertarN(CCiudad* c)
{
    CListaCamino *aristas = new CListaCamino();
    CNodoCiudad *nuevo = new CNodoCiudad;

    nuevo->ciudad = c;
    nuevo->pLista = aristas;

    nuevo->pSig = this->pSig;
    nuevo->pAnt = this;

    this->pSig = nuevo;
    this->pSig->pSig->pAnt = nuevo;
}

CCiudad *CNodoCiudad::eliminarAdelante()
{
    CCiudad *c = this->pSig->ciudad;
    CNodoCiudad *aux = this->pSig->pSig;
    delete this->pSig;

    this->pSig = aux;
    this->pSig->pAnt = this;

    return c;
}

void CNodoCiudad::insertarLista(CCiudad *c)
{
    CListaCamino *pLista = new CListaCamino();
    CNodoCiudad *nuevo = new CNodoCiudad;

    nuevo->pLista = pLista;
    nuevo->ciudad = c;

    nuevo->pSig = this->pSig;
    nuevo->pAnt = this;

    this->pSig = nuevo;
    this->pSig->pSig->pAnt = nuevo;
}

#endif
