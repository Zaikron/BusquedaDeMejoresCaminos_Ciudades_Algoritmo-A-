#ifndef CListaAristas_H
#define CListaAristas_H

#include <iostream>
#include "CNodoCamino.h"
#include "CIteradorCamino.h"

#define CLSID_LISTA_ARISTA 2222

class CListaCamino : public CSerializable
{
    private:
        CNodoCamino inicio;
        CNodoCamino fin;

        CIteradorCamino m_itBegin;
        CIteradorCamino m_itEnd;

    public:
        CListaCamino();
        void insertarInicio(CCamino *pObj);
        void insertarFinal(CCamino *pObj);
        CCamino *eliminarInicio();
        CCamino *eliminarFinal();
        CCamino *Eliminar(string ciudad);
        int getSize();
        bool isEmpty();
        bool isFull();
        CIteradorCamino &Begin();
        CIteradorCamino &End();

        void imprimir(CCiudad *c);
        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

/************ Metodos ************/

CListaCamino::CListaCamino()
{
    inicio.pSig = &fin;
    fin.pAnt = &inicio;
}

CIteradorCamino& CListaCamino::Begin()
{
    m_itBegin.setNodo(inicio.pSig);
    return m_itBegin;
}

CIteradorCamino& CListaCamino::End()
{
    m_itEnd.setNodo(&fin);
    return m_itEnd;
}

void CListaCamino::insertarInicio(CCamino *pObj)
{
    //Inserto el nodo despues del inicio de la lista
    inicio.insertarAdelante(pObj);
}

void CListaCamino::insertarFinal(CCamino *pObj)
{
    //Inserto el nodo despues del penultimo elemento de la lista
    fin.pAnt->insertarAdelante(pObj);
}

CCamino *CListaCamino::eliminarInicio()
{
    //Comprobar si hay elementos en la lista
    if(isEmpty() != true){
        //Elimino el elemento que esta despues del inicio de la lista
        return inicio.eliminarAdelante();
    }else{
        cout << "Imposible Eliminar, No hay elementos en la lista" << endl;
        return nullptr;
    }
}

CCamino *CListaCamino::eliminarFinal()
{
    //Comprobar si hay elementos en la lista
    if(isEmpty() != true){
        //Elimino el nodo que esta despues del antepenultimo elemento de la lista
        return fin.pAnt->pAnt->eliminarAdelante();
    }else{
        cout << "Imposible Eliminar, No hay elementos en la lista" << endl;
        return nullptr;
    }
}


CCamino *CListaCamino::Eliminar(string ciudad){
    CIteradorCamino it;
    CCamino *aux;

    for(it = this->Begin(); it != this->End(); it++){
        if(it.getNodo()->info->getCiudad() == ciudad){
            aux = *it;
            it.getNodo()->pAnt->eliminarAdelante();
            return aux;
        }
    }
    return nullptr;
}


int CListaCamino::getSize()
{
    CNodoCamino *p = inicio.pSig;
    int nodos = 0;

    while(p != &fin){
        nodos ++;
        p = p->pSig;
    }
    return nodos;
}

bool CListaCamino::isEmpty()
{
    return inicio.pSig == &fin;
}

bool CListaCamino::isFull()
{
    //Es dinamica, siempre retornara falso
    return false;
}

void CListaCamino::imprimir(CCiudad *c)
{
    CIteradorCamino it;
    CCamino *aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        aux->imprimir(c);
    }
    cout << endl;
}


void CListaCamino::imprimir()
{

}

void CListaCamino::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSize() << endl;

    CIteradorCamino it;
    CCamino *aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        aux->Salvar(out);
    }
}

void CListaCamino::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;

    for(int i = 0; i < tam; i++){

        int CLSID = 0;
        in>>CLSID;

        CCamino *info = nullptr;

        if(CLSID == CLSID_ARISTA){
            info = new CCamino();
        }

        info -> Cargar(in);
        insertarFinal(info);
    }
}

int CListaCamino::GetCLSID()
{
    return CLSID_LISTA_ARISTA;
}




#endif
