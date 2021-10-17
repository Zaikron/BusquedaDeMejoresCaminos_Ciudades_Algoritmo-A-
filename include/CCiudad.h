#ifndef CCiudad_H
#define CCiudad_H
#include <iostream>
#include <fstream>

#include "CSerializable.h"
#define CLSID_CIUDAD 333

using namespace std;

class CCiudad : public CSerializable
{
    protected:
        string nombre;
        int calificacion;
        float costoEstancia;

    public:
        CCiudad(string nom, int cal, float costo);
        CCiudad(){};
        string getNombre();
        int getCalif();
        float getCostoEstancia();

        void setNombre(string nom);
        void setCalif(int cal);
        void setCosto(float costo);

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

/********* Definicion de  Metodos **********/

CCiudad::CCiudad(string nom, int cal, float costo)
{
    this->nombre = nom;
    this->calificacion = cal;
    this->costoEstancia = costo;
}

void CCiudad::imprimir(){
    cout << endl << "***************************************************" << endl;
    cout << "<<< Ciudad >>> |";
    cout << "Nombre: " << nombre << " [Nodo]";
    cout << " | Calificacion: " << calificacion << " | ";
    cout << "Costo de estancia: " << costoEstancia << " | ";

}

string CCiudad::getNombre(){
    return nombre;
}


int CCiudad::getCalif(){
    return calificacion;
}


float CCiudad::getCostoEstancia(){
    return costoEstancia;
}

void CCiudad::setNombre(string nom)
{
    this->nombre = nom;
}

void CCiudad::setCalif(int cal)
{
    this->calificacion = cal;
}

void CCiudad::setCosto(float costo)
{
    this->costoEstancia = costo;
}

void CCiudad::Salvar(fstream& out)
{
    out<<GetCLSID() << endl;
    out<<nombre << endl;
    out<<calificacion << endl;
    out<<costoEstancia << endl;
}

void CCiudad::Cargar(fstream& in)
{
    in>>nombre;
    in>>calificacion;
    in>>costoEstancia;


}

int CCiudad::GetCLSID()
{
    return CLSID_CIUDAD;
}



#endif

