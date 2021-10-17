#ifndef CLISTA_H
#define CLISTA_H

#include <iostream>
#include "CNodoCiudad.h"
#include "CIteradorCiudad.h"

#define CLSID_LISTA 1111

class CListaCiudad  : public CSerializable
{
    private:
        CNodoCiudad inicio;
        CNodoCiudad fin;

        CIteradorCiudad m_itBegin;
        CIteradorCiudad m_itEnd;

    public:
        CListaCiudad();
        CListaCamino *Busqueda(string c);
        void insertarNodo(CCiudad *obj);
        void insertarArista(string origen, CCamino *destino);
        void insertarListaVacia(CCiudad *c);
        void eliminarNodo(string nomCiudad);
        void eliminarArista(string c1, string c2);
        int getSize();
        bool isEmpty();
        bool isFull();
        CIteradorCiudad &Begin();
        CIteradorCiudad &End();

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

/************ Metodos ************/

CListaCiudad::CListaCiudad()
{
    inicio.pSig = &fin;
    fin.pAnt = &inicio;
}

CIteradorCiudad& CListaCiudad::Begin()
{
    m_itBegin.setNodo(inicio.pSig);
    return m_itBegin;
}

CIteradorCiudad& CListaCiudad::End()
{
    m_itEnd.setNodo(&fin);
    return m_itEnd;
}


void CListaCiudad::insertarNodo(CCiudad *obj){
    fin.pAnt->insertarN(obj);
}

void CListaCiudad::insertarArista(string origen, CCamino *destino){

    CListaCamino *aux = nullptr;
    aux = Busqueda(origen);

    if(aux != nullptr){
        aux->insertarFinal(destino);
    }else{
        cout << "No existe el nodo, no se puede hacer la conexion" << endl;
    }

}


CListaCamino *CListaCiudad::Busqueda(string c){

    CIteradorCiudad it;
    CListaCamino *aux;

    if(isEmpty() != true){
        for(it = this->Begin(); it != this->End(); it++){
            aux = *it; //*it obtiene lista de aristas, lo guarda en aux que es lista de aristas
            if(it.getNodo()->ciudad->getNombre() == c){
                return aux;
            }
        }
    }
    return nullptr;
}

void CListaCiudad::eliminarNodo(string nomCiudad){

    CIteradorCiudad it;

    for(it = this->Begin(); it != this->End(); it++){
        if(it.getNodo()->ciudad->getNombre() == nomCiudad){
            //Cuando se encuentre el nodo se eliminara
            cout << "Se elimino el nodo " << it.getNodo()->ciudad->getNombre() << endl;
            it.getNodo()->pAnt->eliminarAdelante();
        }else{
            //Mientras no se encuentre el nodo se eliminaran las aristas con el mismo nombre que el nodo
            eliminarArista(it.getNodo()->ciudad->getNombre(), nomCiudad);
        }
    }
}

void CListaCiudad::eliminarArista(string c1, string c2){

    CIteradorCiudad it;
    CCamino *info;

    for(it = this->Begin(); it != this->End(); it++){

        if(it.getNodo()->ciudad->getNombre() == c1){
            //Busca a c1 y si lo encuentra lo elimina
            info = it.getNodo()->pLista->Eliminar(c2);
            if(info != nullptr){
               cout << "Se elimino la arista " << info->getCiudad() << endl;
            }

        }
        if(it.getNodo()->ciudad->getNombre() == c2){
            info = it.getNodo()->pLista->Eliminar(c1);
            if(info != nullptr){
               cout << "Se elimino la arista " << info->getCiudad() << endl;
            }
        }

    }
}


int CListaCiudad::getSize()
{
    CNodoCiudad *p = inicio.pSig;
    int nodos = 0;

    while(p != &fin){
        nodos ++;
        p = p->pSig;
    }
    return nodos;
}

bool CListaCiudad::isEmpty()
{
    return inicio.pSig == &fin;
}

bool CListaCiudad::isFull()
{
    //Es dinamica, siempre retornara falso
    return false;
}


void CListaCiudad::imprimir()
{
    CIteradorCiudad it;
    CListaCamino *aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;

        it.getNodo()->ciudad->imprimir();
        aux->imprimir(it.getNodo()->ciudad);
    }

}

void CListaCiudad::insertarListaVacia(CCiudad *c){
    fin.pAnt->insertarLista(c);
}


void CListaCiudad::Salvar(fstream& out)
{
    out << GetCLSID() << endl;
    out << getSize() << endl;

    CIteradorCiudad it;
    CListaCamino *aux;

    for(it = this->Begin(); it != this->End(); it++){
        aux = *it;
        //out << it.getNodo()->ciudad->getNombre() << endl;
        it.getNodo()->ciudad->Salvar(out);
        aux->Salvar(out);
    }
}

void CListaCiudad::Cargar(fstream& in)
{
    int tam = 0;
    in>>tam;

    for(int i = 0; i < tam; i++){

        int CLSID = 0;
        int idListaAristas;

        in>>CLSID;

        CCiudad *c;
        if(CLSID == CLSID_CIUDAD){
            c = new CCiudad();
        }
        c->Cargar(in);
        insertarListaVacia(c);

        in>>idListaAristas;
        fin.pAnt->pLista->Cargar(in);
    }
}

int CListaCiudad::GetCLSID()
{
    return CLSID_LISTA;
}


#endif
