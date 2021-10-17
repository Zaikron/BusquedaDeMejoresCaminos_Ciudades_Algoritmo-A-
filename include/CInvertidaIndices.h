#ifndef CInvertidaIndices_H
#define CInvertidaIndices_H

#include <iostream>
#include "CSerializable.h"

#define CLSID_LISTA_INDICES  5000

using namespace std;

/***** Clase CNodoIndices *****/

class CNodoIndices
{
    public:
        int indice;

    protected:
        CNodoIndices *pSig;
        CNodoIndices *pAnt;

        CNodoIndices();
        void insertarAdelante(int indice);
        int eliminarAdelante();

        //Acceso de los metodos protected a la clase CLista
        friend class CInvertidaIndices;
        friend class CIteradorIndices;

};

/************ Metodos ************/

CNodoIndices::CNodoIndices()
{

}

void CNodoIndices::insertarAdelante(int indice)
{
    CNodoIndices *nuevo = new CNodoIndices;
    nuevo->indice = indice;

    nuevo->pSig = this->pSig;
    nuevo->pAnt = this;

    this->pSig = nuevo;
    this->pSig->pSig->pAnt = nuevo;

}

int CNodoIndices::eliminarAdelante()
{
    //Guardo el dato del nodo para retornarlo
    int indiceA = this->pSig->indice;
    //Guardo el Nodo siguiente del que voy a eliminar
    CNodoIndices *aux = this->pSig->pSig;
    //Libero memoria del Nodo que quiero eliminar
    delete this->pSig;

    //Defino el nodo siguiente de this como el nodo que esta dos posiciones despues
    this->pSig = aux;

    //Defino el nodo anterior de this, que es el que avanzo dos posiciones adelante, ...
    //... con el nodo this que normalmente podria ser el inicio, pero no necesariamente
    this->pSig->pAnt = this;

    return indiceA;

}




/***** Clase CIteradorIndices *****/

class CIteradorIndices
{
    private:
        CNodoIndices *pActual;

    public:
        CIteradorIndices();
        void operator = (CIteradorIndices &otro);
        bool operator == (CIteradorIndices &otro);
        bool operator != (CIteradorIndices &otro);
        void operator ++ ();
        void operator ++ (int);
        void operator -- ();
        void operator -- (int);
        int &operator * ();
        void insertarAdelante(int indice);
        int eliminarAdelante();
    private:
        void setNodo(CNodoIndices *nodo);
        CNodoIndices *getNodo();

    friend class CInvertidaIndices;
};

/************ Metodos ************/

CIteradorIndices::CIteradorIndices()
{
}

void CIteradorIndices::setNodo(CNodoIndices* nodo)
{
    pActual = nodo;
}

CNodoIndices* CIteradorIndices::getNodo()
{
    return pActual;
}

void CIteradorIndices::operator = (CIteradorIndices& otro)
{
    pActual = otro.getNodo();
}

bool CIteradorIndices::operator == (CIteradorIndices& otro)
{
    return pActual == otro.getNodo();
}

bool CIteradorIndices::operator != (CIteradorIndices& otro)
{
    return pActual != otro.getNodo();
}

void CIteradorIndices::operator ++ ()
{
    pActual = pActual->pSig;
}

void CIteradorIndices::operator++(int)
{
    pActual = pActual->pSig;
}

void CIteradorIndices::operator --()
{
    pActual = pActual->pAnt;
}

void CIteradorIndices::operator--(int)
{
    pActual = pActual->pAnt;
}

int &CIteradorIndices::operator * ()
{
    return pActual->indice;
}

void CIteradorIndices::insertarAdelante(int indice)
{
    CNodoIndices *nuevo = new CNodoIndices;
    nuevo->indice = indice;

    nuevo->pSig = pActual->pSig;
    nuevo->pAnt = pActual;

    pActual->pSig = nuevo;
    pActual->pSig->pSig->pAnt = nuevo;

}

int CIteradorIndices::eliminarAdelante()
{
    int indiceA = pActual->pSig->indice;
    CNodoIndices *aux = pActual->pSig->pSig;

    delete pActual->pSig;

    pActual->pSig = aux;
    pActual->pSig->pAnt = pActual;

    return indiceA;
}





/***** Clase CInvertidaIndices *****/

class CInvertidaIndices : CSerializable
{
    private:
        CNodoIndices inicio;
        CNodoIndices fin;

        CIteradorIndices m_itBegin;
        CIteradorIndices m_itEnd;

    public:
        CInvertidaIndices();
        void insertarInicio(int indice);
        void insertarFinal(int indice);
        int eliminarInicio();
        int eliminarFinal();
        int getSize();
        bool isEmpty();
        bool isFull();
        CIteradorIndices &Begin();
        CIteradorIndices &End();

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();

};

/************ Metodos ************/

CInvertidaIndices::CInvertidaIndices()
{
    inicio.pSig = &fin;
    fin.pAnt = &inicio;
}

CIteradorIndices& CInvertidaIndices::Begin()
{
    m_itBegin.setNodo(inicio.pSig);
    return m_itBegin;
}

CIteradorIndices& CInvertidaIndices::End()
{
    m_itEnd.setNodo(&fin);
    return m_itEnd;
}

void CInvertidaIndices::insertarInicio(int indice)
{
    //Inserto el nodo despues del inicio de la lista
    inicio.insertarAdelante(indice);
}

void CInvertidaIndices::insertarFinal(int indice)
{
    //Inserto el nodo despues del penultimo elemento de la lista
    fin.pAnt->insertarAdelante(indice);
}

int CInvertidaIndices::eliminarInicio()
{
    //Comprobar si hay elementos en la lista
    if(isEmpty() != true){
        //Elimino el elemento que esta despues del inicio de la lista
        return inicio.eliminarAdelante();
    }else{
        cout << "Imposible Eliminar, No hay elementos en la lista" << endl;
        return -1;
    }
}

int CInvertidaIndices::eliminarFinal()
{
    //Comprobar si hay elementos en la lista
    if(isEmpty() != true){
        //Elimino el nodo que esta despues del antepenultimo elemento de la lista
        return fin.pAnt->pAnt->eliminarAdelante();
    }else{
        cout << "Imposible Eliminar, No hay elementos en la lista" << endl;
        return -1;
    }
}

int CInvertidaIndices::getSize()
{
    CNodoIndices *p = inicio.pSig;
    int nodos = 0;

    while(p != &fin){
        nodos ++;
        p = p->pSig;
    }
    return nodos;
}

bool CInvertidaIndices::isEmpty()
{
    return inicio.pSig == &fin;
}

bool CInvertidaIndices::isFull()
{
    //Es dinamica, siempre retornara falso
    return false;
}

void CInvertidaIndices::imprimir()
{
    CIteradorIndices it;
    int aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        cout << endl << "Indice Comentario: " << aux << endl;
    }
    cout << endl;
}


/*void CInvertidaIndices::imprimirComentarios(TablaHashComentario *t)
{
    CIteradorIndices it;
    int aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        cout << endl << "Indice Comentario: " << aux << endl;
        recorrerComentarios(t, aux);
    }
    cout << endl;
}

void CInvertidaIndices::recorrerComentarios(TablaHashComentario *t, int indiceCom){
    for(size_t i = 0; i < t->getSizeHash(); i++){
        if(t->arboles[i]->getArbol() != nullptr){
            t->arboles[i]->imprimirIndiceComentario(t->arboles[i]->getArbol(), 0, indiceCom);
        }
    }
}*/



void CInvertidaIndices::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSize() << endl;

    CIteradorIndices it;
    int aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        out << aux << endl;
    }
}

void CInvertidaIndices::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;

    for(int i = 0; i < tam; i++){

        int indice = 0;

        in>>indice;
        insertarFinal(indice);
    }
}

int CInvertidaIndices::GetCLSID()
{
    return CLSID_LISTA_INDICES;
}
#endif
