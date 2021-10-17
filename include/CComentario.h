#ifndef CCOMENTARIO_H
#define CCOMENTARIO_H

using namespace std;
#define CLSID_COMENTARIO 555

class CComentario : public CSerializable
{
    protected:
        int numComentario;
        string comentario;

    public:
        CComentario(string com, int numC);
        CComentario(){};
        void setComentario(string com);
        string getComentario();
        int getNumComentario();

        virtual void imprimir();
        virtual void Salvar(fstream &out);
        virtual void Cargar(fstream &in);
        virtual int GetCLSID();
};

CComentario::CComentario(string com, int numC)
{
    this->comentario = com;
    this->numComentario = numC;
}

void CComentario::setComentario(string com)
{
    this->comentario = com;
}

string CComentario::getComentario()
{
    return comentario;
}

int CComentario::getNumComentario(){
    return numComentario;
}

void CComentario::imprimir()
{
    cout << endl << endl << "      " << (char)192 << "--> Num. Comentario: " << numComentario << endl;
    cout << "       " << comentario << endl;
}

void CComentario::Salvar(fstream& out)
{
    out<<GetCLSID()<<endl;
    out<<numComentario<<endl;
    out<<comentario<<endl;
}

void CComentario::Cargar(fstream& in)
{
    in>>numComentario;
    getline(in, comentario);
    getline(in, comentario);
    //in>>comentario;
}

int CComentario::GetCLSID()
{
    return CLSID_COMENTARIO;
}


#endif // CCOMENTARIO_H
