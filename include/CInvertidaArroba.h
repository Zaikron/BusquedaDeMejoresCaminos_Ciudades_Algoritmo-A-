#ifndef CInvertidaArroba_H
#define CInvertidaArroba_H

#include <iostream>
#include "CInvertidaIndices.h"
#include "CSerializable.h"

#define CLSID_LISTA_ARROBA  3444
using namespace std;

/***** Clase CNodoArroba *****/

class CNodoArroba
{
    public:
        string ciudadArroba;
        CInvertidaIndices *indicesArroba;

    protected:
        CNodoArroba *pSig;
        CNodoArroba *pAnt;

        CNodoArroba();
        void insertarLista(string ciudad, int indice);
        void insertarListaVacia(string ciudad);

        //Acceso de los metodos protected a la clase CInvertidaArroba
        friend class CInvertidaArroba;
        friend class CIteradorArroba;

};

/************ Metodos ************/

CNodoArroba::CNodoArroba()
{

}

void CNodoArroba::insertarLista(string ciudad, int indice)
{

    CInvertidaIndices *indices = new CInvertidaIndices();
    CNodoArroba *nuevo = new CNodoArroba;

    nuevo->ciudadArroba = ciudad;
    nuevo->indicesArroba = indices;
    nuevo->indicesArroba->insertarFinal(indice);

    nuevo->pSig = this->pSig;
    nuevo->pAnt = this;

    this->pSig = nuevo;
    this->pSig->pSig->pAnt = nuevo;

}


void CNodoArroba::insertarListaVacia(string ciudad)
{
    CInvertidaIndices *indicesArroba = new CInvertidaIndices();
    CNodoArroba *nuevo = new CNodoArroba;

    nuevo->indicesArroba = indicesArroba;
    nuevo->ciudadArroba = ciudad;

    nuevo->pSig = this->pSig;
    nuevo->pAnt = this;

    this->pSig = nuevo;
    this->pSig->pSig->pAnt = nuevo;
}




/***** Clase CIteradorArroba *****/

class CIteradorArroba
{
    private:
        CNodoArroba *pActual;

    public:
        CIteradorArroba();
        void operator = (CIteradorArroba &otro);
        bool operator == (CIteradorArroba &otro);
        bool operator != (CIteradorArroba &otro);
        void operator ++ ();
        void operator ++ (int);
        void operator -- ();
        void operator -- (int);
        CInvertidaIndices *operator * ();
        void insertarAdelante(CInvertidaIndices *indicesArroba);
        CInvertidaIndices *eliminarAdelante();
    private:
        void setNodo(CNodoArroba *nodo);
        CNodoArroba *getNodo();

    friend class CInvertidaArroba;
};

/************ Metodos ************/

CIteradorArroba::CIteradorArroba()
{
}

void CIteradorArroba::setNodo(CNodoArroba* nodo)
{
    pActual = nodo;
}

CNodoArroba* CIteradorArroba::getNodo()
{
    return pActual;
}

void CIteradorArroba::operator = (CIteradorArroba& otro)
{
    pActual = otro.getNodo();
}

bool CIteradorArroba::operator == (CIteradorArroba& otro)
{
    return pActual == otro.getNodo();
}

bool CIteradorArroba::operator != (CIteradorArroba& otro)
{
    return pActual != otro.getNodo();
}

void CIteradorArroba::operator ++ ()
{
    pActual = pActual->pSig;
}

void CIteradorArroba::operator++(int)
{
    pActual = pActual->pSig;
}

void CIteradorArroba::operator --()
{
    pActual = pActual->pAnt;
}

void CIteradorArroba::operator--(int)
{
    pActual = pActual->pAnt;
}

CInvertidaIndices *CIteradorArroba::operator * ()
{
    return pActual->indicesArroba;
}

void CIteradorArroba::insertarAdelante(CInvertidaIndices *indicesArroba)
{
    CNodoArroba *nuevo = new CNodoArroba;
    nuevo->indicesArroba = indicesArroba;

    nuevo->pSig = pActual->pSig;
    nuevo->pAnt = pActual;

    pActual->pSig = nuevo;
    pActual->pSig->pSig->pAnt = nuevo;

}

CInvertidaIndices *CIteradorArroba::eliminarAdelante()
{
    CInvertidaIndices *pListaA = pActual->pSig->indicesArroba;
    CNodoArroba *aux = pActual->pSig->pSig;

    delete pActual->pSig;

    pActual->pSig = aux;
    pActual->pSig->pAnt = pActual;

    return pListaA;
}





/***** Clase CInvertidaArroba *****/

class CInvertidaArroba : CSerializable
{
    private:
        CNodoArroba inicio;
        CNodoArroba fin;

        CIteradorArroba m_itBegin;
        CIteradorArroba m_itEnd;

    public:
        CInvertidaArroba();
        void insertar(string ciudad, int indice);
        CInvertidaIndices *Busqueda(string ciudad);
        void insertarListaVacia(string ciudad);
        int getSize();
        bool isEmpty();
        bool isFull();
        CIteradorArroba &Begin();
        CIteradorArroba &End();

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

/************ Metodos ************/

CInvertidaArroba::CInvertidaArroba()
{
    inicio.pSig = &fin;
    fin.pAnt = &inicio;
}

CIteradorArroba& CInvertidaArroba::Begin()
{
    m_itBegin.setNodo(inicio.pSig);
    return m_itBegin;
}

CIteradorArroba& CInvertidaArroba::End()
{
    m_itEnd.setNodo(&fin);
    return m_itEnd;
}

void CInvertidaArroba::insertar(string ciudad, int indice)
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


CInvertidaIndices *CInvertidaArroba::Busqueda(string ciudad){

    CIteradorArroba it;
    CInvertidaIndices *aux;

    if(isEmpty() != true){
        for(it = this->Begin(); it != this->End(); it++){
            aux = *it;
            //cout << endl << it.getNodo()->ciudadArroba << ", " << ciudad << endl;
            //aux->imprimir();
            if(it.getNodo()->ciudadArroba == ciudad){
                return aux;
            }
        }
    }
    return nullptr;
}


void CInvertidaArroba::insertarListaVacia(string ciudad){
    fin.pAnt->insertarListaVacia(ciudad);
}


int CInvertidaArroba::getSize()
{
    CNodoArroba *p = inicio.pSig;
    int nodos = 0;

    while(p != &fin){
        nodos ++;
        p = p->pSig;
    }
    return nodos;
}

bool CInvertidaArroba::isEmpty()
{
    return inicio.pSig == &fin;
}

bool CInvertidaArroba::isFull()
{
    //Es dinamica, siempre retornara falso
    return false;
}


void CInvertidaArroba::imprimir()
{
    CIteradorArroba it;
    CInvertidaIndices *aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        cout << endl << "       --[ @" << it.getNodo()->ciudadArroba << " ]--" << endl;
        aux->imprimir();
    }

}


void CInvertidaArroba::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSize() << endl;

    CIteradorArroba it;
    CInvertidaIndices *aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        out << it.getNodo()->ciudadArroba << endl;
        aux->Salvar(out);
    }
}

void CInvertidaArroba::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;
    in.ignore();
    for(int i = 0; i < tam; i++){

        string ciudad = "";
        in>>ciudad;
        int CLSID_LO = 0;
        in>>CLSID_LO;

        insertarListaVacia(ciudad);
        fin.pAnt->indicesArroba->Cargar(in);

    }
}

int CInvertidaArroba::GetCLSID()
{
    return CLSID_LISTA_ARROBA;
}

#endif
