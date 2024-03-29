#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include "stack"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
  
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      fil = col = fil_aux = col_aux =  fil_ini_aux = col_ini_aux = fil_ini = col_ini= fila_anterior = columna_anterior= 99;

      fil_bus=5; col_bus=4;
      minFilAux = minColAux=200;
      
      brujula = brujula_interna = 0;
      ultimaAccion = actIDLE;
      girar_derecha = true;
      bien_situado = true;
      
      bikini = zapatillas = hay_bikini = hay_zapatillas = hay_bateria = turnL =false;
      
      mismo_camino = buscaSalida= salida = salida_encontrada = hay_posicionamiento= false;
      primeraVez= segundaVez= true;
      num_iteracion= nSalida = 0;

      vector<unsigned char> aux(MAX_MAPA_AUX, '?');
      vector<unsigned char> aux_busqueda(6, '?');
      vector<int> rec(MAX_MAPA_AUX, -1);
      
      for (unsigned int i = 0; i < MAX_MAPA_AUX; i++){
         mapaAuxiliar.push_back(aux);
         mapaRecorrido.push_back(rec);
      }
      for (unsigned int i=0; i < 4; i++)
        mapaBusqueda.push_back(aux_busqueda);

      resetMapaRadial();
      
      he_reseteado = false;

      pintaPrecipicios();

    }



    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void actualizarVistaMapa(Sensores sensores, vector< vector< unsigned char> > & mapa, int ffil, int ccol);
    void actualizarVistaMapaAuxiliar(Sensores sensores);
    void actualizarVistaMapaResultado(Sensores sensores);
    void actualizarVistaMapaRecorrido(Sensores sensores);

    void actualizarVista (Sensores sensores);
    void copiarAuxiliarEnResultado(vector< vector< unsigned char> > & mapaAux, vector< vector< unsigned char> > & mapaRes);

    void ResetMapa(vector< vector< unsigned char> > & mapa);
    bool pasoPosible (Sensores sensores, int sen, bool estoy_buscando = false);
    bool pasoMapaNoVisto(Sensores sensores);


    // para dejar el codigo mas limpio
    void comprobacionesIniciales(Sensores sensores);
    void switchAccion ();
    void comprobaciones(Sensores sensores);
    void encuentraCasillasImportantes(Sensores sensores, bool busca_bateria);
    
    void meterEnPila(Action accion);
    void pintaPrecipicios();
    // mapaRadial
    void resetMapaRadial();
    void actualizaMapaRadial(int fil_obj, int col_obj);
    void pintaMapaRadial(unsigned char c);


    // mapa potencial
    void resetMapaPotencial();
    void actualizaMapaPotencial(int fil_obj, int col_obj);
    void pintaMapaPotencial(unsigned char c);

    // Acciones
    Action accionSinNada(Sensores sensores);
    Action accionBusquedaCasilla(Sensores sensores);
    Action accionBuscaSalidaBienSituado(Sensores sensores);
    Action accionBuscaSalidaMalSituado(Sensores sensores);
    Action accionMapaPotencial(int min, int b);

    Action buscaObjetoPotencial(Sensores sensores);
    int encuentraMenor();

    Action accionPorDefecto(Sensores sensores);
    bool esInaccesible (Sensores sensores, int num_sensor);

  private:
    int fil, col, brujula, fil_aux, col_aux, minFilAux, minColAux, fil_ini_aux, col_ini_aux, fil_ini, col_ini, brujula_interna, fila_anterior,columna_anterior;
    Action ultimaAccion, ultAccionBus;
    bool girar_derecha, bien_situado, he_reseteado;
    const int MAX_MAPA_AUX=200, SIZE = 100;
    bool bikini, zapatillas, hay_bikini, hay_zapatillas, hay_posicionamiento, hay_bateria, turnL;
    bool salida_bucle, salida_encontrada;
    int iter_bucle=0, iter_mismo=0;
    const int NUM_ITER_SALIDA_BUCLE = 6;
    const int FIL_MATRIZ_RADIAL = 6, COL_MATRIZ_RADIAL=9;
    const int VAL_MAX_SALIDA = 1000000;
    const int VAL_MIN_BATERIA = 2000;
    const int VAL_MAX_BATERIA = 5000;
    const int FIL_MATRIZ_POTENCIAL=mapaResultado.size(), COL_MATRIZ_POTENCIAL =mapaResultado.size();

    bool cambio_180 = false;
    
    int fil_bus, col_bus;
    int fil_ob, col_ob;
    int busca_salida=0;
    bool primeraVez, segundaVez;
    vector< vector< unsigned char> > mapaBusqueda;

    stack<Action>  pilaRecorrido, pila_vacia;
    vector< vector< unsigned char> > mapaAuxiliar;
    vector< vector< int> > mapaRecorrido, mapaRadial, mapaPotencial;
    pair<int,int> par_anterior;

    bool mismo_camino, buscaSalida, salida;
    bool encerradoCamino = false;
    int num_iteracion, nSalida;
    unsigned char ultimo_giro = '.';
};

#endif
