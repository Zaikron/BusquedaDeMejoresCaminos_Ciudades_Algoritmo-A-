#ifndef CInvertidaGato_H
#define CInvertidaGato_H

#include <iostream>
#include "CInvertidaIndices.h"
#include "CSerializable.h"

#define CLSID_LISTA_GATO  3445
using namespace std;

/***** Clase CNodoGato *****/

class CNodoGato
{
    public:
        string ciudadGato;
        CInvertidaIndices *indicesArroba;

    protected:
        CNodoGato *pSig;
        CNodoGato *pAnt;

        CNodoGato();
        void insertarLista(string ciudad, int indice);
        void insertarListaVacia(string ciudad);

        //Acceso de los metodos protected a la clase CInvertidaGato
        friend class CInvertidaGato;
        friend class CIteradorGato;

};

/************ Metodos ************/

CNodoGato::CNodoGato()
{

}

void CNodoGato::insertarLista(string ciudad, int indice)
{

    CInvertidaIndices *indices = new CInvertidaIndices();
    CNodoGato *nuevo = new CNodoGato;

    nuevo->ciudadGato = ciudad;
    nuevo->indicesArroba = indices;
    nuevo->indicesArroba->insertarFinal(indice);

    nuevo->pSig = this->pSig;
    nuevo->pAnt = this;

    this->pSig = nuevo;
    this->pSig->pSig->pAnt = nuevo;

}


void CNodoGato::insertarListaVacia(string ciudad)
{
    CInvertidaIndices *indicesArroba = new CInvertidaIndices();
    CNodoGato *nuevo = new CNodoGato;

    nuevo->indicesArroba = indicesArroba;
    nuevo->ciudadGato = ciudad;

    nuevo->pSig = this->pSig;
    nuevo->pAnt = this;

    this->pSig = nuevo;
    this->pSig->pSig->pAnt = nuevo;
}




/***** Clase CIteradorGato *****/

class CIteradorGato
{
    private:
        CNodoGato *pActual;

    public:
        CIteradorGato();
        void operator = (CIteradorGato &otro);
        bool operator == (CIteradorGato &otro);
        bool operator != (CIteradorGato &otro);
        void operator ++ ();
        void operator ++ (int);
        void operator -- ();
        void operator -- (int);
        CInvertidaIndices *operator * ();
        void insertarAdelante(CInvertidaIndices *indicesArroba);
        CInvertidaIndices *eliminarAdelante();
    private:
        void setNodo(CNodoGato *nodo);
        CNodoGato *getNodo();

    friend class CInvertidaGato;
};

/************ Metodos ************/

CIteradorGato::CIteradorGato()
{
}

void CIteradorGato::setNodo(CNodoGato* nodo)
{
    pActual = nodo;
}

CNodoGato* CIteradorGato::getNodo()
{
    return pActual;
}

void CIteradorGato::operator = (CIteradorGato& otro)
{
    pActual = otro.getNodo();
}

bool CIteradorGato::operator == (CIteradorGato& otro)
{
    return pActual == otro.getNodo();
}

bool CIteradorGato::operator != (CIteradorGato& otro)
{
    return pActual != otro.getNodo();
}

void CIteradorGato::operator ++ ()
{
    pActual = pActual->pSig;
}

void CIteradorGato::operator++(int)
{
    pActual = pActual->pSig;
}

void CIteradorGato::operator --()
{
    pActual = pActual->pAnt;
}

void CIteradorGato::operator--(int)
{
    pActual = pActual->pAnt;
}

CInvertidaIndices *CIteradorGato::operator * ()
{
    return pActual->indicesArroba;
}

void CIteradorGato::insertarAdelante(CInvertidaIndices *indicesArroba)
{
    CNodoGato *nuevo = new CNodoGato;
    nuevo->indicesArroba = indicesArroba;

    nuevo->pSig = pActual->pSig;
    nuevo->pAnt = pActual;

    pActual->pSig = nuevo;
    pActual->pSig->pSig->pAnt = nuevo;

}

CInvertidaIndices *CIteradorGato::eliminarAdelante()
{
    CInvertidaIndices *pListaA = pActual->pSig->indicesArroba;
    CNodoGato *aux = pActual->pSig->pSig;

    delete pActual->pSig;

    pActual->pSig = aux;
    pActual->pSig->pAnt = pActual;

    return pListaA;
}





/***** Clase CInvertidaGato *****/

class CInvertidaGato : CSerializable
{
    private:
        CNodoGato inicio;
        CNodoGato fin;

        CIteradorGato m_itBegin;
        CIteradorGato m_itEnd;

    public:
        CInvertidaGato();
        void insertar(string ciudad, int indice);
        CInvertidaIndices *Busqueda(string ciudad);
        void insertarListaVacia(string ciudad);
        int getSize();
        bool isEmpty();
        bool isFull();
        CIteradorGato &Begin();
        CIteradorGato &End();

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

/************ Metodos ************/

CInvertidaGato::CInvertidaGato()
{
    inicio.pSig = &fin;
    fin.pAnt = &inicio;
}

CIteradorGato& CInvertidaGato::Begin()
{
    m_itBegin.setNodo(inicio.pSig);
    return m_itBegin;
}

CIteradorGato& CInvertidaGato::End()
{
    m_itEnd.setNodo(&fin);
    return m_itEnd;
}

void CInvertidaGato::insertar(string ciudad, int indice)
{
    CInvertidaIndices *aux = nullptr;
    aux = Busqueda(ciudad);
    //Inserto el nodo despues del inicio de la lista

    if(aux != nullptr){
        aux->insertarFinal(indice);
    }else{
        //Insercion al final
        fin.pAnt->insertarLista(ciudad, indice);
    }

}


CInvertidaIndices *CInvertidaGato::Busqueda(string ciudad){

    CIteradorGato it;
    CInvertidaIndices *aux;

    if(isEmpty() != true){
        for(it = this->Begin(); it != this->End(); it++){
            aux = *it;
            if(it.getNodo()->ciudadGato == ciudad){
                return aux;
            }
        }
    }
    return nullptr;
}


void CInvertidaGato::insertarListaVacia(string ciudad){
    fin.pAnt->insertarListaVacia(ciudad);
}


int CInvertidaGato::getSize()
{
    CNodoGato *p = inicio.pSig;
    int nodos = 0;

    while(p != &fin){
        nodos ++;
        p = p->pSig;
    }
    return nodos;
}

bool CInvertidaGato::isEmpty()
{
    return inicio.pSig == &fin;
}

bool CInvertidaGato::isFull()
{
    //Es dinamica, siempre retornara falso
    return false;
}


void CInvertidaGato::imprimir()
{
    CIteradorGato it;
    CInvertidaIndices *aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        cout << endl << "       --[ #" << it.getNodo()->ciudadGato << " ]--" << endl;
        aux->imprimir();
    }

}


void CInvertidaGato::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSize() << endl;

    CIteradorGato it;
    CInvertidaIndices *aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        out << it.getNodo()->ciudadGato << endl;
        aux->Salvar(out);
    }
}

void CInvertidaGato::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;

    for(int i = 0; i < tam; i++){

        string ciudad = "";
        in>>ciudad;

        int CLSID_LO = 0;
        in>>CLSID_LO;

        insertarListaVacia(ciudad);
        fin.pAnt->indicesArroba->Cargar(in);

    }
}

int CInvertidaGato::GetCLSID()
{
    return CLSID_LISTA_GATO;
}

#endif
