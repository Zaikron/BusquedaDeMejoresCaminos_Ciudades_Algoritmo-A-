#ifndef CITERADOR_H
#define CITERADOR_H

#include "CNodoCiudad.h"

class CIteradorCiudad
{
    private:
        CNodoCiudad *pActual;

    public:
        CIteradorCiudad();
        void operator = (CIteradorCiudad &otro);
        bool operator == (CIteradorCiudad &otro);
        bool operator != (CIteradorCiudad &otro);
        void operator ++ ();
        void operator ++ (int);
        void operator -- ();
        void operator -- (int);
        CListaCamino *operator * ();
        void insertarAdelante(CListaCamino *pLista);
        CListaCamino *eliminarAdelante();
    private:
        void setNodo(CNodoCiudad *nodo);
        CNodoCiudad *getNodo();

    friend class CListaCiudad;
};

/************ Metodos ************/

CIteradorCiudad::CIteradorCiudad()
{
}

void CIteradorCiudad::setNodo(CNodoCiudad* nodo)
{
    pActual = nodo;
}

CNodoCiudad* CIteradorCiudad::getNodo()
{
    return pActual;
}

void CIteradorCiudad::operator = (CIteradorCiudad& otro)
{
    pActual = otro.getNodo();
}

bool CIteradorCiudad::operator == (CIteradorCiudad& otro)
{
    return pActual == otro.getNodo();
}

bool CIteradorCiudad::operator != (CIteradorCiudad& otro)
{
    return pActual != otro.getNodo();
}

void CIteradorCiudad::operator ++ ()
{
    pActual = pActual->pSig;
}

void CIteradorCiudad::operator++(int)
{
    pActual = pActual->pSig;
}

void CIteradorCiudad::operator --()
{
    pActual = pActual->pAnt;
}

void CIteradorCiudad::operator--(int)
{
    pActual = pActual->pAnt;
}

CListaCamino *CIteradorCiudad::operator * ()
{
    return pActual->pLista;
}

void CIteradorCiudad::insertarAdelante(CListaCamino *pLista)
{
    CNodoCiudad *nuevo = new CNodoCiudad;
    nuevo->pLista = pLista;

    nuevo->pSig = pActual->pSig;
    nuevo->pAnt = pActual;

    pActual->pSig = nuevo;
    pActual->pSig->pSig->pAnt = nuevo;

}

CListaCamino *CIteradorCiudad::eliminarAdelante()
{
    CListaCamino *pListaA = pActual->pSig->pLista;
    CNodoCiudad *aux = pActual->pSig->pSig;

    delete pActual->pSig;

    pActual->pSig = aux;
    pActual->pSig->pAnt = pActual;

    return pListaA;
}




#endif
