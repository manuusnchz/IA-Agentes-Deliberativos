#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>
#include <climits>

#include "comportamientos/comportamiento.hpp"

struct EstadoR {
  int f;
  int c;
  int brujula;
  bool zapatillas;
  int coste_acumulado;

  bool operator==(const EstadoR& otro) const {
    return f == otro.f && 
           c == otro.c && 
           brujula == otro.brujula && 
           zapatillas == otro.zapatillas;
}

  bool operator<(const EstadoR &st) const {
      if (f != st.f) return f < st.f;
      if (c != st.c) return c < st.c;
      if (brujula != st.brujula) return brujula < st.brujula;
      return zapatillas < st.zapatillas;
  }

  bool operator!=(const EstadoR &otro) const {
    return !(*this == otro);
  }
};

struct NodoR {
  EstadoR estado;
  list<Action> secuencia;
  int coste_total;

  bool operator<(const NodoR &n) const {
      return coste_total < n.coste_total;
  }

  bool operator>(const NodoR& otro) const {
    return coste_total > otro.coste_total;
}
};

struct CompararNodoR{
  bool operator()(const NodoR &a, const NodoR & b) const{
    return a.coste_total > b.coste_total;
  }
};

int calcularCoste(Action accion, 
  const EstadoR &origen,
  const EstadoR &destino,
  const vector<vector<unsigned char>> &terreno,
  const vector<vector<unsigned char>> &altura);

class ComportamientoRescatador : public Comportamiento
{

public:

int costeTerreno(
  char terrenoDestino,
  int cotaOrigen,
  int cotaDestino,
  Action accion);

bool AlgoritmoDkjistra(const EstadoR &origen, const EstadoR &destino, list<Action> &plan);
static bool CasillaTransitableRescatador(const EstadoR &st, 
  const vector<vector<unsigned char>> &terreno,
  const vector<vector<unsigned char>> &altura);

static EstadoR NextCasillaRescatador(const EstadoR &st);

bool CasillaAccesibleAuxiliar(const EstadoR &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura);

EstadoR applyR(Action accion, const EstadoR & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura);

bool CasillaAccesibleRescatador(const EstadoR &st, const vector<vector<unsigned char>> &terreno,
  const vector<vector<unsigned char>> &altura);
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45Izq = 0;

    for(int i = 0 ; i < 500 ; i++){
      for(int j = 0 ; j < 500 ; j++){
        matriz[i][j]=1;
      }
    }

    for(int i = 0 ; i < 500 ; i++){
      for(int j = 0 ; j < 500 ; j++){
        matrizTempR[i][j]=0;
      }
    }
    instante = 0;
  }
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    hayPlan = false;
    tiene_zapatillas = false;
  }
  ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}
  ~ComportamientoRescatador() {}
  void AnularMatrizR(vector<vector<unsigned char>> &m);
  void VisualizaPlan(const EstadoR &st, const list<Action> &plan);
  void PintaPlan(const list<Action> &plan, bool zap);


  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  Action ComportamientoRescatadorNivel_2(Sensores sensores);
  Action ComportamientoRescatadorNivel_3(Sensores sensores);
  Action ComportamientoRescatadorNivel_4(Sensores sensores);

private:
  Action last_action;
  bool tiene_zapatillas;
  int giro45Izq;

  int matriz[500][500];
  int matrizTempR[500][500];
  int instante;

  //Variables para nivel E
  list<Action> plan;
  bool hayPlan;

  EstadoR origen,destino;



  
};

#endif
