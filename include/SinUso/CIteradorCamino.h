#ifndef CIteradorAristas_H
#define CIteradorAristas_H

#include "CNodoCamino.h"

class CIteradorCamino
{
    private:
        CNodoCamino *pActual;

    public:
        CIteradorCamino();
        void operator = (CIteradorCamino &otro);
        bool operator == (CIteradorCamino &otro);
        bool operator != (CIteradorCamino &otro);
        void operator ++ ();
        void operator ++ (int);
        void operator -- ();
        void operator -- (int);
        CCamino *operator * ();
        void insertarAdelante(CCamino *c);
        CCamino *eliminarAdelante();
    private:
        void setNodo(CNodoCamino *nodo);
        CNodoCamino *getNodo();

    friend class CListaCamino;
};

/************ Metodos ************/

CIteradorCamino::CIteradorCamino()
{
}

void CIteradorCamino::setNodo(CNodoCamino* nodo)
{
    pActual = nodo;
}

CNodoCamino* CIteradorCamino::getNodo()
{
    return pActual;
}

void CIteradorCamino::operator = (CIteradorCamino& otro)
{
    pActual = otro.getNodo();
}

bool CIteradorCamino::operator == (CIteradorCamino& otro)
{
    return pActual == otro.getNodo();
}

bool CIteradorCamino::operator != (CIteradorCamino& otro)
{
    return pActual != otro.getNodo();
}

void CIteradorCamino::operator ++ ()
{
    pActual = pActual->pSig;
}

void CIteradorCamino::operator++(int)
{
    pActual = pActual->pSig;
}

void CIteradorCamino::operator --()
{
    pActual = pActual->pAnt;
}

void CIteradorCamino::operator--(int)
{
    pActual = pActual->pAnt;
}

CCamino *CIteradorCamino::operator * ()
{
    return pActual->info;
}

void CIteradorCamino::insertarAdelante(CCamino *c)
{
    CNodoCamino *nuevo = new CNodoCamino;
    nuevo->info = c;

    nuevo->pSig = pActual->pSig;
    nuevo->pAnt = pActual;

    pActual->pSig = nuevo;
    pActual->pSig->pSig->pAnt = nuevo;

}

CCamino *CIteradorCamino::eliminarAdelante()
{
    CCamino *cA = pActual->pSig->info;
    CNodoCamino *aux = pActual->pSig->pSig;

    delete pActual->pSig;

    pActual->pSig = aux;
    pActual->pSig->pAnt = pActual;

    return cA;
}




#endif
