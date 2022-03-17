#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      fil = col = fil_aux = col_aux = 99;
      minFilAux = minColAux=200;
      
      brujula = 0;
      ultimaAccion = actIDLE;
      girar_derecha = false;
      bien_situado = true;

      vector<unsigned char> aux(MAX_MAPA_AUX, '?');
      for (unsigned int i = 0; i < MAX_MAPA_AUX; i++){
         mapaAuxiliar.push_back(aux);
      }
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void actualizarVistaMapa(Sensores sensores, vector< vector< unsigned char> > & mapa, int ffil, int ccol);
    void actualizarVistaMapaAuxiliar(Sensores sensores);
    void actualizarVistaMapaResultado(Sensores sensores);
    void copiarAuxiliarEnResultado(vector< vector< unsigned char> > & mapaAux, vector< vector< unsigned char> > & mapaRes);

  private:
    int fil, col, brujula, fil_aux, col_aux, minFilAux, minColAux;
    Action ultimaAccion;
    bool girar_derecha, bien_situado;
    const int MAX_MAPA_AUX=200, SIZE = 100;
    vector< vector< unsigned char> > mapaAuxiliar;
    

};

#endif
