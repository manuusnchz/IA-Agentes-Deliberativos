#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"
#include <queue> // For priority_queue
#include <set>   // For set
#include <map> // For unordered_map

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0 (sensores);
		break;
	case 1:
		accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoRescatadorNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoRescatadorNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
{
	return 0;
}

int VeoCasillaInteresanteR(char i, char c, char d, bool zap){
	
	
	
	if(c == 'X') return 2;
	else if(i == 'X') return 1;
	else if(d == 'X') return 3;
	
	else if (!zap){
		if(c == 'D') return 2;
		else if(i == 'D') return 1;
		else if(d == 'D') return 3;
	}

	else if(zap){
		if(c == 'D') return 2;
		else if(i == 'D') return 1;
		else if(d == 'D') return 3;
	}


	if( c == 'C') return 2;
	else if( i == 'C') return 1;
	else if(d== 'C') return 3;

	else return 0;
}


int VeoCasillaInteresanteR1(char i, char c, char d, bool zap, char mi, char mc, char md){
	 
	// Prioridad 1: Casillas no exploradas ('?')
	 if (mc == '?' && c != 'P' && c != 'M') return 2;
	 else if (mi == '?' && i != 'P' && i != 'M') return 1;
	 else if (md == '?' && d != 'P' && d != 'M') return 3;
 
	 // Prioridad 2: Víctimas ('X')
	 if (c == 'X') return 2;
	 else if (i == 'X') return 1;
	 else if (d == 'X') return 3;
	 
	 // Prioridad 3: Zapatillas ('D') si no las tenemos
	 if (!zap) {
		 if (c == 'D') return 2;
		 else if (i == 'D') return 1;
		 else if (d == 'D') return 3;
	 }
	 
	 // Prioridad 4: Caminos ('C') y senderos ('S')
	 if (c == 'C' ) return 2;
	 else if (i == 'C' ) return 1;
	 else if (d == 'C') return 3;
	 
	 if (c == 'S' ) return 2;
	 else if (i == 'S' ) return 1;
	 else if (d == 'S') return 3;

	 
	 // Si no hay nada interesante
	 return 0;
}



char ViablePorAlturaR(char casilla, int dif, bool zap){
	if(abs(dif)<=1 or (zap and abs(dif)<=2)){
		return casilla;
	}
	else{
		return 'P';
	}
}

void SituarSensorEnMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char >> &a, Sensores sensores)
{
	m[sensores.posF][sensores.posC] = sensores.superficie[0];

	int pos = 1;
	switch (sensores.rumbo)
	{
	case norte:
		m[sensores.posF-1][sensores.posC-1] = sensores.superficie[1];
		m[sensores.posF-1][sensores.posC] = sensores.superficie[2];
		m[sensores.posF-1][sensores.posC+1] = sensores.superficie[3];
		
		break;
	
	case noreste:
		m[sensores.posF-1][sensores.posC] = sensores.superficie[1];
		m[sensores.posF-1][sensores.posC+1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC+1] = sensores.superficie[3];
		
		break;
	
	case este:
		m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
		m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
		m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
		
		break;
	
	case sureste:
		m[sensores.posF][sensores.posC+1]   = sensores.superficie[1];
		m[sensores.posF+1][sensores.posC+1] = sensores.superficie[2];
		m[sensores.posF+1][sensores.posC]   = sensores.superficie[3];
		
		break;
	
	case sur:
		m[sensores.posF+1][sensores.posC+1] = sensores.superficie[1];
		m[sensores.posF+1][sensores.posC]   = sensores.superficie[2];
		m[sensores.posF+1][sensores.posC-1] = sensores.superficie[3];
		
		break;
	
	case suroeste:
		m[sensores.posF+1][sensores.posC]   = sensores.superficie[1];
		m[sensores.posF+1][sensores.posC-1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC-1]   = sensores.superficie[3];
		
		break;
	
	case oeste:
		m[sensores.posF+1][sensores.posC-1] = sensores.superficie[1];
		m[sensores.posF][sensores.posC-1]   = sensores.superficie[2];
		m[sensores.posF-1][sensores.posC-1] = sensores.superficie[3];
		
		break;
	
	case noroeste:
		m[sensores.posF][sensores.posC-1]   = sensores.superficie[1];
		m[sensores.posF-1][sensores.posC-1] = sensores.superficie[2];
		m[sensores.posF-1][sensores.posC]   = sensores.superficie[3];
		
		break;
	
	}
}


bool ComportamientoRescatador::CasillaTransitableRescatador(const EstadoR &st, 
	const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura) {
EstadoR next = NextCasillaRescatador(st);

// Comprobar límites del mapa
if (next.f < 0 || next.f >= terreno.size() || 
next.c < 0 || next.c >= terreno[0].size()) {
return false;
}

// Comprobar obstáculos
if (terreno[next.f][next.c] == 'P' || terreno[next.f][next.c] == 'M') {
return false;
}

// Comprobar bosque sin zapatillas
if (terreno[next.f][next.c] == 'B' && !st.zapatillas) {
return false;
}

// Comprobar diferencia de altura
int dif_altura = abs(altura[next.f][next.c] - altura[st.f][st.c]);
if (dif_altura > 1 && !st.zapatillas) {
return false;
}

return true;
}

EstadoR ComportamientoRescatador::NextCasillaRescatador(const EstadoR &st) {
EstadoR siguiente = st;

switch (st.brujula) {
case 0: // Norte
siguiente.f--;
break;
case 1: // Noreste
siguiente.f--;
siguiente.c++;
break;
case 2: // Este
siguiente.c++;
break;
case 3: // Sureste
siguiente.f++;
siguiente.c++;
break;
case 4: // Sur
siguiente.f++;
break;
case 5: // Suroeste
siguiente.f++;
siguiente.c--;
break;
case 6: // Oeste
siguiente.c--;
break;
case 7: // Noroeste
siguiente.f--;
siguiente.c--;
break;
}

return siguiente;
}


EstadoR applyR(Action accion, const EstadoR &st, 
	const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura) {
EstadoR next = st;

switch(accion) {
case WALK:
case RUN:
  if (ComportamientoRescatador::CasillaTransitableRescatador(st, terreno, altura)) {
	  next = ComportamientoRescatador::NextCasillaRescatador(st);
  }
  break;
  
case TURN_SR:
  next.brujula = (next.brujula + 1) % 8;
  break;
  
case TURN_L:
  next.brujula = (next.brujula + 7) % 8; // Equivalente a -1 mod 8
  break;
}

return next;
}

int calcularCoste(Action accion, const EstadoR &origen, const EstadoR &destino,
	const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura) {
int coste = 0;
int dif_altura = abs(altura[destino.f][destino.c] - altura[origen.f][origen.c]);

switch (accion) {
case WALK:
coste = 10;
if (terreno[destino.f][destino.c] == 'B') coste += 5;
if (dif_altura > 1 && !origen.zapatillas) return INT_MAX; // No transitable
coste += dif_altura;
break;

case RUN:
coste = 50;
if (terreno[destino.f][destino.c] == 'B') coste += 10;
if (dif_altura > 1 && !origen.zapatillas) return INT_MAX; // No transitable
coste += dif_altura * 2;
break;

case TURN_SR:
case TURN_L:
coste = 2;
break;

default:
coste = 0;
}

return coste;
}

//Implemento el algoritmo de Dikjstyra

list<Action> DijkstraRescatador(const EstadoR &inicio, const EstadoR &final,
	const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura) {
		priority_queue<NodoR, vector<NodoR>, greater<NodoR>> frontier;
		map<EstadoR, int> mejores_costes;
		list<Action> plan_vacio;
	
		NodoR inicial;
		inicial.estado = inicio;
		inicial.estado.coste_acumulado = 0;
		inicial.coste_total = 0;
		frontier.push(inicial);
		mejores_costes[inicio] = 0; 
	
		while (!frontier.empty()) {
			NodoR current = frontier.top();
			frontier.pop();
	
			//Verificar que es el mejor camino hasta este nodo
			if (current.coste_total > mejores_costes[current.estado]) {
				continue; // <-- AÑADE ESTO PARA PODAR RUTAS INEFICIENTES
			}
			
			// Comprobar si es solución
			if (current.estado.f == final.f && current.estado.c == final.c) {
				return current.secuencia;
			}
	
			// Comprobar si ya hay un camino mejor
			if (mejores_costes.count(current.estado) && 
				mejores_costes[current.estado] < current.coste_total) {
				continue;
			}
	
			// Generar sucesores
			for (Action accion : {WALK, RUN, TURN_SR, TURN_L}) {
				NodoR hijo = current;
				hijo.estado = applyR(accion, current.estado, terreno, altura);
				
				if (!(hijo.estado == current.estado)) { // Si la acción es válida
					int coste = calcularCoste(accion, current.estado, hijo.estado, terreno, altura);
					
					if (coste != INT_MAX) { // Si es transitable
						hijo.estado.coste_acumulado = current.estado.coste_acumulado + coste;
						hijo.coste_total = hijo.estado.coste_acumulado;
						hijo.secuencia.push_back(accion);
	
						// Solo añadir si es mejor que lo conocido
						if (!mejores_costes.count(hijo.estado) || 
							hijo.coste_total < mejores_costes[hijo.estado]) {
							mejores_costes[hijo.estado] = hijo.coste_total;
							frontier.push(hijo);
						}
					}
				}
			}
		}
		return plan_vacio;

}

void ComportamientoRescatador::AnularMatrizR(vector<vector<unsigned char>> &m) {
    for (auto &fila : m) {
        fill(fila.begin(), fila.end(), 0);
    }
}


void ComportamientoRescatador::VisualizaPlan(const EstadoR &st, const list<Action> &plan)
{
	ComportamientoRescatador::AnularMatrizR(mapaConPlan);
	EstadoR cst = st;
	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
		case WALK:
			switch (cst.brujula)
			{
			case 0:
				cst.f--;
				break;
			case 1:
				cst.f--;
				cst.c++;
				break;
			case 2:
				cst.c++;
				break;
			case 3:
				cst.f++;
				cst.c++;
				break;
			case 4:
				cst.f++;
				break;
			case 5:
				cst.f++;
				cst.c--;
				break;
			case 6:
				cst.c--;
				break;
			case 7:
				cst.f--;
				cst.c--;
				break;
			}
			mapaConPlan[cst.f][cst.c] = 2;
			break;
		case TURN_SR:
			cst.brujula = (cst.brujula + 1) % 8;
			break;
		}
		it++;
	}
}





Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	//HOLA

	Action accion;

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaR(mapaResultado,mapaCotas,sensores);

	if(last_action == WALK){
		matriz[sensores.posF][sensores.posC]++;
	}

	if (matriz[sensores.posF][sensores.posC] >= 4) {
		// Está en bucle: aplica una acción distinta
		giro45Izq = rand() % 5;
		accion = TURN_SR;  // TURN_SR aleatorio
		matriz[sensores.posF][sensores.posC] = 1; // resetea el contador
		return accion;
	}



	if(sensores.superficie[0] == 'X'){
		accion = IDLE;
	}

	else if(giro45Izq != 0){
		accion = TURN_SR;
		giro45Izq--;
	}

	else{
		
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1]-sensores.cota[0],tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2]-sensores.cota[0],tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3]-sensores.cota[0],tiene_zapatillas);

		//Detectamos si hay un auxiliar delante

		if(sensores.agentes[2] == 'a'){
			c = 'P';
		}

		int pos = VeoCasillaInteresanteR(i,c,d,tiene_zapatillas);
		switch(pos)
		{
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45Izq = 1;
				accion = TURN_L;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				accion = TURN_L;
				break;
					
						
					
				
		}
	}

	last_action = accion;
	return accion;
	
}


Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{

	Action accion;

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaR(mapaResultado,mapaCotas,sensores);

	if(last_action == WALK){
		matriz[sensores.posF][sensores.posC]++;
	}

	if (matriz[sensores.posF][sensores.posC] >= 6) {
		// Está en bucle: aplica una acción distinta
		giro45Izq = rand() % 5;
		accion = TURN_SR;  // TURN_SR aleatorio
		matriz[sensores.posF][sensores.posC] = 1; // resetea el contador
		return accion;
	}


	else if(giro45Izq != 0){
		accion = TURN_SR;
		giro45Izq--;
	}

	else{
		
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1]-sensores.cota[0],tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2]-sensores.cota[0],tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3]-sensores.cota[0],tiene_zapatillas);

		//Detectamos si hay un auxiliar delante

		if(sensores.agentes[2] == 'a'){
			c = 'P';
		}
		char mi = mapaResultado[sensores.posF][sensores.posC-1]; // izquierda
		char mc = mapaResultado[sensores.posF-1][sensores.posC]; // centro
		char md = mapaResultado[sensores.posF][sensores.posC+1]; // derecha
		
		int pos = VeoCasillaInteresanteR1(i, c, d, tiene_zapatillas, mi, mc, md);

		
		switch(pos)
		{
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45Izq = 1;
				accion = TURN_L;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				accion = TURN_L;
				break;
					
						
					
				
		}
	}

	last_action = accion;
	return accion;

}


Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
	Action accion = IDLE;
    
    if (!hayPlan) {
        // Definir estado inicial y final
        EstadoR inicio, fin;
        inicio.f = sensores.posF;
        inicio.c = sensores.posC;
        inicio.brujula = sensores.rumbo;
        
		
		if(sensores.superficie[0] == 'D') inicio.zapatillas = true;
		else inicio.zapatillas = false;
        
		
		inicio.coste_acumulado = 0;
        
        fin.f = sensores.destinoF;
        fin.c = sensores.destinoC;
        
        // Calcular plan con Dijkstra
        plan = DijkstraRescatador(inicio, fin, mapaResultado, mapaCotas);
        VisualizaPlan(inicio, plan);
        hayPlan = plan.size() != 0;
    }
    
    if (hayPlan && plan.size() > 0) {
        accion = plan.front();
        plan.pop_front();
    }
    
    if (plan.size() == 0) {
        hayPlan = false;
    }
    
    return accion;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}
  