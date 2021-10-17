#include <iostream>
#include <fstream>

#include "TablaHashCiudad.h"
#include "TablaHashComentario.h"

using namespace std;

/*** Funciones utiles para obtener la distancia Heuristica ***/
//Funcion para buscar una ciudad en un vector, retorna el indice
int buscarCiudad(vector<ElementoAEstrella> rutas, string origen, size_t actual){
    for(size_t i = actual; i < rutas.size(); i++){
        if(rutas[i].origen->getNombre() == origen){
            return i;
        }
    }
    return -1;
}

//Funcion para calcular la distancia Heristica de una ruta
//Tabien uso un vector auxiliar para guardar a las ciudades que no son conexas en el grafo
vector<ElementoAEstrella> aux;
int calcularDistanciaHeuristica(vector<ElementoAEstrella> rutas, string origen, string destino, int distancia, int insertado){
    int distanciaHeuristica = distancia;
    int indiceC = 0;

    if(distancia != 0){
        indiceC = buscarCiudad(rutas, origen, 0);
    }else{
        indiceC = buscarCiudad(rutas, origen, insertado);
    }

    if(indiceC != -1){
        //cout << rutas[indiceC].destino->getCiudad() << ", "<<destino<<endl;
        //cout << rutas[indiceC].origen->getNombre()<<endl;
        if(rutas[indiceC].destino->getCiudad() == destino){
            aux[insertado].indicadorNoConexo = false;
            return ++distanciaHeuristica;
        }else if((size_t)insertado != rutas.size()-1){
            return calcularDistanciaHeuristica(rutas, rutas[indiceC].destino->getCiudad(), destino, ++distanciaHeuristica, indiceC);
        }else{
            aux[insertado].indicadorNoConexo = true;
            return -1;
        }
    }else if(rutas[insertado+1].origen->getNombre() == rutas[insertado].origen->getNombre()){
            aux[insertado].indicadorNoConexo = true;
            return calcularDistanciaHeuristica(rutas, rutas[insertado+1].destino->getCiudad(), destino, distanciaHeuristica, insertado+1);
    }else{
        aux[insertado].indicadorNoConexo = true;
        return -1;
    }
    aux[insertado].indicadorNoConexo = true;
    return -1;
}

//Funcion para buscar un destino en un vector, retorna el indice
int buscarDestino(vector<ElementoAEstrella> rutas, string destino, size_t actual){
    for(size_t i = actual; i < rutas.size(); i++){
        if(rutas[i].destino->getCiudad() == destino){
            return i;
        }
    }
    return -1;
}

//Funcion que calcula las distancias Heuristicas de todo el vector de rutas
vector<ElementoAEstrella> establecerDistanciasHeuristicas(vector<ElementoAEstrella> rutas, string destino, vector<ElementoAEstrella> noConexos){

    for(size_t i = 0; i < rutas.size(); i++){
        rutas[i].h = calcularDistanciaHeuristica(rutas, rutas[i].origen->getNombre(), destino, 0, i);
    }
    return rutas;
}

//Funcion que elimina los elementos no conexos del grafo, pues me fue necesario para implementar el algoritmo A*
vector<ElementoAEstrella> quitarNoConexos(vector<ElementoAEstrella> noConexos, vector<ElementoAEstrella> rutas){

    for(size_t i = 0; i < rutas.size(); i++){
        rutas[i].indicadorNoConexo = noConexos[i].indicadorNoConexo;
    }
    size_t tamRutas = rutas.size();
    for(size_t i = 0; i < tamRutas; i++){
        if(rutas[i].indicadorNoConexo == true){
            rutas.erase(rutas.begin()+i);

            tamRutas--;
            i--;
        }
    }
    return rutas;
}


/*** Funciones  utiles para el algoritmo A* ***/
//Funcion que comprueba la existencia de una ciudad en un vector
bool comprobarExistencia(vector<ElementoAEstrella> elementos, ElementoAEstrella actual){
    for(size_t i = 0; i < elementos.size(); i++){
        if(elementos[i].origen->getNombre() == actual.origen->getNombre()){
            if(elementos[i].destino->getCiudad() == actual.destino->getCiudad()){
                return true;
            }
        }
    }
    return false;
}

//Funcion para encontrar y retornar las adyacencias de la ciudad actual
vector<ElementoAEstrella> comprobarAdyacencias(vector<ElementoAEstrella> rutas, string actual,
                                               vector<ElementoAEstrella> abierta, vector<ElementoAEstrella> cerrada){
    for(size_t i = 0; i < rutas.size(); i++){
        if(rutas[i].origen->getNombre() == actual){
            if(!comprobarExistencia(abierta, rutas[i])){
                if(!comprobarExistencia(cerrada, rutas[i])){
                    //Calculo de g
                    rutas[i].setG();
                    for(size_t j = 0; j < cerrada.size(); j++){
                        if(rutas[i].origen->getNombre() == cerrada[j].destino->getCiudad()){
                            rutas[i].g += cerrada[j].g;
                            rutas[i].costo += cerrada[j].costo;
                        }
                    }
                    //calculo de f(n)
                    rutas[i].calcularFN();
                    abierta.push_back(rutas[i]);
                }
            }
        }
    }
    return abierta;
}

//Funcion que obtiene el valor de f(n) minimo en la lista abierta(vector)
ElementoAEstrella obtenerMinimo(vector<ElementoAEstrella> abierta){
    ElementoAEstrella minimo = abierta[0];
    for(size_t i = 1; i < abierta.size(); i++){
        if(minimo.fn > abierta[i].fn){
            minimo = abierta[i];
        }
    }
    return minimo;
}

//Funcion de uso general para eliminar un elemento de un vector
vector<ElementoAEstrella> eliminarElemento(vector<ElementoAEstrella> elementos, ElementoAEstrella eliminado){
    size_t tamRutas = elementos.size();

    for(size_t i = 0; i < tamRutas; i++){
        if(elementos[i].origen->getNombre() == eliminado.origen->getNombre()){
            if(elementos[i].destino->getCiudad() == eliminado.destino->getCiudad()){
                elementos.erase(elementos.begin()+i);
                tamRutas--;
                i--;
            }
        }
    }
    return elementos;
}

//Funcion para agregar un elemento a la lista cerrada (vector)
vector<ElementoAEstrella> agregarACerrada(vector<ElementoAEstrella> cerrada, ElementoAEstrella agregado){
    if(!comprobarExistencia(cerrada, agregado)){
        cerrada.push_back(agregado);
    }
    return cerrada;
}

//Funcion para mostrar de una forma mas ordenada el camino
void mostrarCamino(vector<ElementoAEstrella> cerradaFinal){
    vector<ElementoAEstrella> invertidoVisualizacion;
    int tam = cerradaFinal.size()-1;
    invertidoVisualizacion.push_back(cerradaFinal[cerradaFinal.size()-1]);
    for(int i = tam; i >= 0; i--){
        for(int j = tam; j >= 0; j--){
            if(i != j){
                if(cerradaFinal[i].origen->getNombre() == cerradaFinal[j].destino->getCiudad()){
                    invertidoVisualizacion.push_back(cerradaFinal[j]);
                }
            }
        }
    }

    cout << "*Camino a recorrer: " << endl;
    for(int i = invertidoVisualizacion.size()-1; i >= 0; i--){
        cout << "De " << invertidoVisualizacion[i].origen->getNombre() << " hasta "
        << invertidoVisualizacion[i].destino->getCiudad()
        << "[" << invertidoVisualizacion[i].g << "km] ";
        if(i != 0){
            cout << " y despues..." << endl;
        }else{
            cout << endl << "* La distancia total del recorrido es de " << invertidoVisualizacion[i].g << "km" << endl;
            cout << "- El costo total del viaje seria de " << invertidoVisualizacion[i].costo << "$" << endl;
            cout << "- El medio de transporte usado es un " << invertidoVisualizacion[i].destino->getMedioTrans() << endl;
        }
    }
    cout << endl;
}

int buscarDestinoEnLimite(vector<ElementoAEstrella> rutas, string destino, size_t limite){
    for(size_t i = 0; i < limite; i++){
        if(rutas[i].destino->getCiudad() == destino){
            return i;
        }
    }
    return -1;
}

//Algoritmo A*
vector<ElementoAEstrella> AEstrella(string origen, string destino, vector<ElementoAEstrella> rutas){
    vector<ElementoAEstrella> abierta;
    vector<ElementoAEstrella> cerrada;
    string actual = origen;
    ElementoAEstrella minimo;

    while(actual != destino){
        if(buscarCiudad(rutas, actual, 0) != -1){
            abierta = comprobarAdyacencias(rutas, actual, abierta, cerrada);
            minimo = obtenerMinimo(abierta);
            abierta = eliminarElemento(abierta, minimo);
            cerrada = agregarACerrada(cerrada, minimo);
            actual = minimo.destino->getCiudad();
        }else{
            cerrada.clear();
            return cerrada;
        }
    }
    return cerrada;
}



/*** Funcion Principal ***/
int main()
{
    TablaHashCiudad grafo;
    grafo.crearHash();

    fstream salidaGrafo;
    fstream entradaGrafo;
    fstream entradaComentarios;
    fstream salidaComentarios;
    TablaHashComentario comentarios;
    comentarios.crearHash();

    /*** Cargado de los datos ***/
    entradaComentarios.open("Comentarios.txt", ios_base::in);
    int CLSID_COM = 0;
    entradaComentarios>>CLSID_COM;
    comentarios.Cargar(entradaComentarios);
    entradaComentarios.close();

    entradaGrafo.open("Ciudades.txt", ios_base::in);
    int CLSID = 0;
    entradaGrafo>>CLSID;
    grafo.Cargar(entradaGrafo);
    entradaGrafo.close();
    /***/

    int opc = 0;
    string ciudadOrigen = "AUTLAN_DE_NAVARRO";
    string ciudadDestino = "";
    int indicadorDestino = 0;

    float distanciaUsu = 30;
    float costoViajeUsu = 3500;
    string medio = "Autobus";
    string origenRuta;
    string destinoRuta;

    string busquedaComentarios = "";
    string sinId = "";
    CInvertidaArroba *arrobas = comentarios.arrobas;
    CInvertidaGato *gatos = comentarios.gatos;
    CInvertidaIndices *indices;

    vector<ElementoAEstrella> cerradaAux;
    vector<ElementoAEstrella> rutasSug;
    vector<ElementoAEstrella> rutas = grafo.getRutas();
    aux = rutas;

    int opc2 = 0;
    CCiudad *ciudad;
    CCamino *camino;
    string origenCRUD;
    string destinoCRUD;

    string nombre;
    int calificacion;
    float costoEstancia;
    float distancia;
    float costoViaje;
    string medioTransporte;

    do{
    vector<ElementoAEstrella> rutasAux = rutas;
    aux = rutasAux;

        cout << "***OPCIONES PARA EL VIAJERO***" << endl;
        cout << "1 - Mis Preferencias." << endl;
        cout << "2 - Mostrar Una Ruta." << endl;
        cout << "3 - Obtener Sugerencias." << endl;
        cout << "4 - Buscar Comentarios." << endl;
        cout << "5 - Mostrar Todo." << endl;
        cout << "6 - Opciones para el programador (CRUD)." << endl;
        cout << "0 - Salir." << endl;
        cout << "*Elige una opcion: ";
        cin>>opc;
        cin.ignore();
        cout << endl;

        switch(opc){
            case 1:
                cout << endl << "***PREFERENCIAS DEL VIAJERO***";
                cout << endl<< "¿Cual es tu ubicacion actual? ";
                getline(cin, ciudadOrigen);
                cout << endl<< "¿Tienes ya elegida una ciudad destino? Escribe 1 para: Si, 0 para: No: ";
                cin>>indicadorDestino;
                cin.ignore();
                if(indicadorDestino == 1){
                    cout << endl<< "Escribe tu ciudad destino: ";
                    getline(cin, ciudadDestino);
                }else{
                    ciudadDestino = "";
                    cout << endl<< "Si no tienes ciudad destino te podtiamos dar recomendaciones, ingresa los datos.";
                    cout << endl<< "Distancia maxina de viaje a la ciudad: ";
                    cin>>distanciaUsu;
                    cout << endl<< "Costo maximo para viajar a la ciudad: ";
                    cin>>costoViajeUsu;
                    cout << endl<< "Medio de transporte requerido: ";
                    cin>>medio;
                    cin.ignore();
                }
                cout << endl << "***PREFERENCIAS DEL VIAJERO GUARDADAS***" << endl;;
                break;
            case 2:
                cout << endl << "***VER RUTA***";
                cout << endl<< "Ingresa la ciudad origen: ";
                getline(cin, origenRuta);
                cout << endl<< "Ingresa la ciudad destino: ";
                getline(cin, destinoRuta);

                if(buscarCiudad(rutasAux, origenRuta, 0) != -1){
                    if(buscarDestino(rutasAux, destinoRuta, 0) != -1){
                        rutasAux = establecerDistanciasHeuristicas(rutasAux, destinoRuta, aux);
                        rutasAux = quitarNoConexos(aux, rutasAux);
                        cerradaAux = AEstrella(origenRuta, destinoRuta, rutasAux);
                        if(cerradaAux.size() != 0){
                            mostrarCamino(cerradaAux);
                        }else{
                            cout << "No existe la ruta" << endl;
                        }
                    }else{
                        cout << "La ruta no existe" << endl;
                    }
                }else{
                        cout << "El destino no existe" << endl;
                }
                break;
            case 3:
                cout << "***OBTENER UNA SUGERENCIA***" << endl;
                if(ciudadDestino != ""){
                    if(buscarCiudad(rutasAux, ciudadOrigen, 0) != -1){
                        if(buscarDestino(rutasAux, ciudadDestino, 0) != -1){
                            rutasAux = establecerDistanciasHeuristicas(rutasAux, ciudadDestino, aux);

                            rutasAux = quitarNoConexos(aux, rutasAux);
                            cerradaAux = AEstrella(ciudadOrigen, ciudadDestino, rutasAux);
                            if(cerradaAux.size() != 0){
                                mostrarCamino(cerradaAux);
                            }else{
                                cout << "No existe la ruta" << endl;
                            }
                        }else{
                            cout << "La ruta no existe" << endl;
                        }
                    }else{
                            cout << "El destino no existe" << endl;
                    }
                }else{
                    if(buscarCiudad(rutasAux, ciudadOrigen, 0) != -1){
                        for(size_t j = 0; j < rutasAux.size(); j++){
                            rutasSug = rutasAux;
                            aux = rutasAux;
                            if(buscarDestinoEnLimite(rutas, rutasAux[j].destino->getCiudad(), j) == -1){
                                if(buscarDestino(rutasAux, rutasAux[j].destino->getCiudad(), 0) != -1){
                                    rutasSug = establecerDistanciasHeuristicas(rutasAux, rutasAux[j].destino->getCiudad(), aux);
                                    rutasSug = quitarNoConexos(aux, rutasSug);
                                    cerradaAux = AEstrella(ciudadOrigen, rutasAux[j].destino->getCiudad(), rutasSug);
                                    cout<<"";
                                    if(cerradaAux[cerradaAux.size()-1].g <= distanciaUsu){
                                        if(cerradaAux[cerradaAux.size()-1].costo <= costoViajeUsu){
                                            if(cerradaAux[cerradaAux.size()-1].destino->getMedioTrans() == medio){
                                                if(cerradaAux.size() != 0){
                                                    cout << endl;
                                                    mostrarCamino(cerradaAux);
                                                    break;
                                                }else{
                                                    cout << "No existe la ruta" << endl;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        cout << "No hay ninguna ruta" << endl;
                    }
                }
                break;
            case 4:
                cout << endl << "***BUSQUEDA DE COMENTARIOS***";
                cout << endl << "Ingresa un @ o un # para buscar los comentarios: ";
                getline(cin, busquedaComentarios);
                if(busquedaComentarios[0] == '@'){
                    sinId = "";
                    for(size_t i = 1; i < busquedaComentarios.size(); i++){
                        sinId += busquedaComentarios[i];
                    }
                    indices = arrobas->Busqueda(sinId);
                    comentarios.imprimirDeterminados(indices);
                }else if(busquedaComentarios[0] == '#'){
                    sinId = "";
                    for(size_t i = 1; i < busquedaComentarios.size(); i++){
                        sinId += busquedaComentarios[i];
                    }
                    indices = gatos->Busqueda(sinId);
                    comentarios.imprimirDeterminados(indices);
                }else{
                    cout << "No ingresaste un @ o #, no se ha buscado nada" << endl;
                }

                break;
            case 5:
                cout << "***GRAFO CON ADYACENCIAS Y SUS COMENTARIOS***" << endl;
                grafo.imprimir();
                cout << "***RUTAS/VIAJES DISPONIBLES***" << endl;
                for(size_t i = 0; i < rutasAux.size(); i++){
                    rutasSug = rutasAux;
                    aux = rutasAux;
                    if(buscarCiudad(rutasAux, rutas[i].origen->getNombre(), 0) != -1){
                        for(size_t j = 0; j < rutasAux.size(); j++){
                            if(i != j){
                                if(buscarDestino(rutasAux, rutasAux[j].destino->getCiudad(), 0) != -1){
                                    if(buscarDestinoEnLimite(rutas, rutasAux[j].destino->getCiudad(), j) == -1){
                                        rutasSug = establecerDistanciasHeuristicas(rutasAux, rutasAux[j].destino->getCiudad(), aux);
                                        rutasSug = quitarNoConexos(aux, rutasSug);
                                        cerradaAux = AEstrella(rutas[i].origen->getNombre(), rutasAux[j].destino->getCiudad(), rutasSug);
                                        cout<<"";
                                        if(cerradaAux.size() != 0){
                                            cout << endl;
                                            mostrarCamino(cerradaAux);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                break;


            case 6:
                    do{
                        cout << "***OPCIONES PARA EL PROGRAMADOR***" << endl;
                        cout << "1 - (CREACION) Insertar nodo." << endl;
                        cout << "2 - (CREACION) Insertar arista." << endl;
                        cout << "3 - (LECTURA) Mostrar Grafo." << endl;
                        cout << "4 - (ACTUALIZACION) Actualizar nodo." << endl;
                        cout << "5 - (ACTUALIZACION) Actualizar arista." << endl;
                        cout << "6 - (ELIMINACION) Eliminar nodo)." << endl;
                        cout << "7 - (ELIMINACION) Eliminar arista (CRUD)." << endl;
                        cout << "0 - Salir." << endl;
                        cout << "*Elige una opcion: ";
                        cin>>opc2;
                        cin.ignore();
                        cout << endl<<endl;

                        switch(opc2){
                            case 1:
                                cout << "Ingresa el nombre de la ciudad: ";
                                getline(cin, nombre);
                                cout << "Ingresa la calificacion: ";
                                cin>>calificacion;
                                cout << "Ingresa el costo de estancia: ";
                                cin>>costoEstancia;
                                cin.ignore();
                                ciudad = new CCiudad(nombre, calificacion, costoEstancia);

                                grafo.insertarNodo(ciudad);
                                break;
                            case 2:
                                cout << "Ingresa el origen: ";
                                getline(cin, origenCRUD);
                                cout << "Ingresa el destino: ";
                                getline(cin, destinoCRUD);

                                cout << "Ingresa la distancia: ";
                                cin>>distancia;
                                cout << "Ingresa el costo viaje: ";
                                cin>>costoViaje;
                                cin.ignore();
                                cout << "Ingresa el transporte: ";
                                getline(cin, medioTransporte);

                                camino = new CCamino(distancia, costoViaje, medioTransporte, destinoCRUD);
                                grafo.insertarArista(origenCRUD, camino);
                                break;
                            case 3: grafo.imprimir();

                                break;
                            case 4:
                                cout << "Ingresa el nombre de la ciudad a actualizar"<<endl;
                                getline(cin, nombre);
                                cout <<endl<< "Ingresa los datos nuevos" <<endl;
                                cout << "Ingresa la calificacion: ";
                                cin>>calificacion;
                                cout << "Ingresa el costo de estancia: ";
                                cin>>costoEstancia;
                                cin.ignore();
                                grafo.actualizarNodo(nombre, calificacion, costoEstancia);
                                break;
                            case 5:
                                cout << "Ingresa el nombre origen y destino de la arista a actualizar"<<endl;
                                cout << "Ingresa el origen: ";
                                getline(cin, origenCRUD);
                                cout << "Ingresa el destino: ";
                                getline(cin, destinoCRUD);
                                cout <<endl<< "Ingresa los datos nuevos" <<endl;
                                cout << "Ingresa la distancia: ";
                                cin>>distancia;
                                cout << "Ingresa el costo viaje: ";
                                cin>>costoViaje;
                                cin.ignore();
                                cout << "Ingresa el transporte: ";
                                getline(cin, medioTransporte);
                                grafo.actualizarArista(origenCRUD, destinoCRUD, distancia, costoViaje, medioTransporte);
                                break;
                            case 6:
                                cout << "Ingresa el nombre del nodo a eliminar: ";
                                getline(cin, nombre);
                                grafo.eliminarNodo(nombre);
                                break;
                            case 7:
                                cout << "Ingresa el nombre origen y destino de la arista a eliminar"<<endl;
                                cout << "Ingresa el origen: ";
                                getline(cin, origenCRUD);
                                cout << "Ingresa el destino: ";
                                getline(cin, destinoCRUD);
                                grafo.eliminarArista(origenCRUD, destinoCRUD);
                                break;
                            case 0: cout << "Saliendo de opciones del programador..." << endl;
                                break;
                            default:  cout << "Opcion incorrecta" << endl;
                        }
                        system("pause");
                        system("cls");
                    }while(opc2 != 0);
                break;

            case 0: cout << "Fin del programa" << endl;
                salidaComentarios.open("Comentarios.txt", ios::out);
                comentarios.Salvar(salidaComentarios);
                salidaComentarios.close();

                salidaGrafo.open("Ciudades.txt", ios::out);
                grafo.Salvar(salidaGrafo);
                salidaGrafo.close();
                break;
            default: cout << "Opcion incorrecta" << endl;

        }

        system("pause");
        system("cls");
    }while(opc != 0);
    return 0;
}
