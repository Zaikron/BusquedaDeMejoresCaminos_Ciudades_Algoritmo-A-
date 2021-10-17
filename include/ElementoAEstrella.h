#ifndef ELEMENTOAESTRELLA_H
#define ELEMENTOAESTRELLA_H

#include <CCamino.h>
#include <CCiudad.h>

using namespace std;

class ElementoAEstrella
{
    public:
        CCiudad *origen;
        CCamino *destino;
        float g = 0;
        int h;
        float fn = 0;

        float costo;

        bool indicadorNoConexo = false;

        ElementoAEstrella();
        void calcularFN();
        void setG();

};


ElementoAEstrella::ElementoAEstrella()
{
    //ctor
}
void ElementoAEstrella::setG(){
    this->g = this->destino->getDistancia();
    this->costo = this->destino->getCostoV();
}

void ElementoAEstrella::calcularFN(){
    fn = g + (float)h;
}

#endif // ELEMENTOAESTRELLA_H
