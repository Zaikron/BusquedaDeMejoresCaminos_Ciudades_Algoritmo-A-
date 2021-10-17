#ifndef CNodoAristas_H
#define CNodoAristas_H

#include <iostream>
#include "CCiudad.h"
#include "CSerializable.h"
#include "CCamino.h"
#include "TablaHashCamino.h"

using namespace std;

class CNodoCamino
{
    public:
        CCamino *info;

    protected:
        CNodoCamino *pSig;
        CNodoCamino *pAnt;

        CNodoCamino();
        void insertarAdelante(CCamino *c);
        CCamino *eliminarAdelante();

        //Acceso de los metodos protected a la clase CListaCiudad
        friend class CListaCamino;
        friend class CIteradorCamino;

};

/************ Metodos ************/

CNodoCamino::CNodoCamino()
{

}

void CNodoCamino::insertarAdelante(CCamino *c)
{
    CNodoCamino *nuevo = new CNodoCamino;
    nuevo->info = c;

    nuevo->pSig = this->pSig;
    nuevo->pAnt = this;

    this->pSig = nuevo;
    this->pSig->pSig->pAnt = nuevo;

}

CCamino *CNodoCamino::eliminarAdelante()
{
    CCamino *cA = this->pSig->info;
    CNodoCamino *aux = this->pSig->pSig;
    delete this->pSig;

    this->pSig = aux;
    this->pSig->pAnt = this;

    return cA;

}


#endif
