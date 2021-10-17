#ifndef CSERIALIZABLE_H
#define CSERIALIZABLE_H

#include <fstream>

using namespace std;

class CSerializable
{
    public:
        CSerializable();
        virtual void Salvar(fstream &out) = 0;
        virtual void Cargar(fstream &in) = 0;
        virtual int GetCLSID() = 0;
};



CSerializable::CSerializable(){}


#endif // CSERIALIZABLE_H
