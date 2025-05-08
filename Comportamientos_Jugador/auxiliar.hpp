#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>

#include "comportamientos/comportamiento.hpp"

struct EstadoA {
  int f;
  int c;
  int brujula;
  bool zapatillas;
  
  
  bool operator<(const EstadoA& otroEstado) const {
    if (f != otroEstado.f) {
        return f < otroEstado.f;
    }
    if (c != otroEstado.c) {
        return c < otroEstado.c;
    }
    if (brujula != otroEstado.brujula) {
        return brujula < otroEstado.brujula;
    }
    // Compara otros miembros si son relevantes para la unicidad del estado
    return zapatillas < otroEstado.zapatillas; // Si todos los miembros son iguales, no es menor
}

  bool operator==(const EstadoA &st) const
  {
    return f == st.f && c == st.c && brujula == st.brujula and zapatillas ==
    st.zapatillas;
  }

	bool operator!=(const EstadoA &other) const {
		return !(*this == other);
	}
};

struct NodoA{
EstadoA estado;
int coste_total;
int coste_acumulado;
list<Action> secuencia;
bool operator==(const NodoA &node) const{
  return estado == node.estado;
}
};

struct CompararNodoA{
  bool operator()(const NodoA &a, const NodoA & b) const{
    return a.coste_total > b.coste_total;
  }
};

class ComportamientoAuxiliar : public Comportamiento
{

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    //Inicializar variables de los niveles 1 y 4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45Izq = 0;

    for(int i = 0 ; i < 500 ; i++){
      for(int j = 0 ; j < 500 ; j++){
        matrizAux[i][j]=1;
      }
    }

  }
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan = false;
  }
  ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
  ~ComportamientoAuxiliar() {}

  bool CasillaTransitableAuxiliarD(const EstadoA &st,
    const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);

    list<Action> AlgoritmoAEstrella(
      const EstadoA &origen,
      const EstadoA &destino,
      const vector<vector<unsigned char>> &terreno,
      const vector<vector<unsigned char>> &altura) ;
      
  
  Action think(Sensores sensores);


 bool CasillaTransitableAuxiliar(const EstadoA &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);

    int Heuristica(const EstadoA &origen, const EstadoA &destino);
 

  void AnularMatrizA(vector<vector<unsigned char>> &m);
  bool Find(const NodoA &st, const list<NodoA> &lista);


  void PintaPlan(const list<Action> &plan, bool zap);
  int costeTerreno(char terrenoDestino, int cotaOrigen, int cotaDestino, Action accion);

  EstadoA applyA(Action accion, const EstadoA &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);


  EstadoA NextCasillaAuxiliar(const EstadoA &st);
  int interact(Action accion, int valor);

  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);

  Action ComportamientoAuxiliarNivel_E(Sensores sensores);


  
  list<Action> AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final,const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura );


  EstadoA NextCasillaAuxiliarD(const EstadoA &st);
  void VisualizaPlan(const EstadoA &st, const list<Action> &plan);
  

private:
  Action last_action;
  bool tiene_zapatillas;
  int giro45Izq;

  int matrizAux[500][500];

  EstadoA origen;
  EstadoA destino;
  

  //Variables para nivel E
  list<Action> plan;
  bool hayPlan;
};

#endif
