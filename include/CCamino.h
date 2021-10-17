#ifndef CCamino_H
#define CCamino_H

#include <iostream>
#include "CCiudad.h"

using namespace std;
#define CLSID_CAMINO 444

class CCamino : public CSerializable
{
    private:
        string ciudad;
        float distancia;
        float costoViaje;
        string medioTransporte;
    public:
        CCamino(float dist, float costo, string transporte, string c);
        CCamino(){};
        float getDistancia();
        float getCostoV();
        string getMedioTrans();
        string getCiudad();
        void imprimir(CCiudad *c);

        void setDistacia(float dist);
        void setCostoV(float costo);
        void setMedioTrans(string medio);
        void setCiudad(string ciudad);

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};


CCamino::CCamino(float dist, float costo, string transporte, string c)
{
    this->distancia = dist;
    this->costoViaje = costo;
    this->medioTransporte = transporte;
    this->ciudad = c;
}

float CCamino::getDistancia()
{
    return distancia;
}

float CCamino::getCostoV()
{
    return costoViaje;
}

string CCamino::getMedioTrans()
{
    return medioTransporte;
}

string CCamino::getCiudad()
{
    return ciudad;
}

void CCamino::setDistacia(float dist)
{
   this->distancia = dist;
}

void CCamino::setCostoV(float costo)
{
    this->costoViaje = costo;
}

void CCamino::setMedioTrans(string medio)
{
    this->medioTransporte = medio;
}

void CCamino::setCiudad(string ciudad)
{
    this->ciudad = ciudad;
}

void CCamino::imprimir(CCiudad *c)
{
    cout << endl << endl << "[Arista]";
    cout << endl << "De " << c->getNombre() << " hasta " << ciudad << " hay una distancia de " << distancia << "km";
    cout << endl << "El costo del viaje es de $" << costoViaje;
    cout << endl << "Se viaja a traves de un " << medioTransporte;
}


void CCamino::imprimir()
{
    cout << endl << endl << "   Ciudad destino: " << ciudad;
    cout << " | Distancia de viaje: " << distancia;
    cout << endl << "   Costo del viaje: " << costoViaje;
    cout << " | Medio de transporte: " << medioTransporte;
}

void CCamino::Salvar(fstream& out)
{
    out<<GetCLSID() << endl;
    out<<ciudad << endl;
    out<<distancia << endl;
    out<<costoViaje << endl;
    out<<medioTransporte << endl;
}

void CCamino::Cargar(fstream& in)
{
    in>>ciudad;
    in>>distancia;
    in>>costoViaje;
    in>>medioTransporte;
}

int CCamino::GetCLSID()
{
    return CLSID_CAMINO;
}



#endif // CCamino_H



