#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include "motorlib/util.h"
#include <queue>
#include <set>
#include <map>

using namespace std;

Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0(sensores);
		break;
	case 1:
		accion = ComportamientoAuxiliarNivel_1(sensores);
		break;
	case 2:
		accion = ComportamientoAuxiliarNivel_2(sensores);
		break;
	case 3:
		accion = ComportamientoAuxiliarNivel_3(sensores);

		break;
	case 4:
		// accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

/*NUEVO INTENTO NIVEL 3*/

bool ComportamientoAuxiliar::Find(const NodoA &st, const list<NodoA> &lista)
{
	auto it = lista.begin();

	while (it != lista.end() and !((*it) == st))
	{
		it++;
	}

	return (it != lista.end());
}

bool ComportamientoAuxiliar::CasillaTransitableAuxiliar(const EstadoA &st,
														const vector<vector<unsigned char>> &terreno,
														const vector<vector<unsigned char>> &altura)
{

	EstadoA next = NextCasillaAuxiliar(st);

	// Comprobar límites del mapa
	if (next.f < 0 || next.f >= terreno.size() ||
		next.c < 0 || next.c >= terreno[0].size())
	{
		return false;
	}

	// Comprobar obstáculos
	if (terreno[next.f][next.c] == 'P' || terreno[next.f][next.c] == 'M' )
	{
		return false;
	}

	if (terreno[next.f][next.c] == 'B' && !st.zapatillas){
		return false;
	}

	// Comprobar diferencia de altura
	int dif_altura = abs(altura[next.f][next.c] - altura[st.f][st.c]);

	if (dif_altura <= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoA &st, const list<Action> &plan)
{
	AnularMatrizA(mapaConPlan);
	EstadoA cst = st;
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
			mapaConPlan[cst.f][cst.c] = 1;
			break;
		case TURN_SR:
			cst.brujula = (cst.brujula + 1) % 8;
			break;

		}
		it++;
	}
}

void ComportamientoAuxiliar::PintaPlan(const list<Action> &plan, bool zap)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == WALK)
		{
			cout << "W ";
		}
		else if (*it == RUN)
		{
			cout << "R ";
		}
		else if (*it == TURN_SR)
		{
			cout << "r ";
		}
		else if (*it == TURN_L)
		{
			cout << "L ";
		}
		else if (*it == CALL_ON)
		{
			cout << "C ";
		}
		else if (*it == CALL_OFF)
		{
			cout << "c ";
		}
		else if (*it == IDLE)
		{
			cout << "I ";
		}
		else
		{
			cout << "-_ ";
		}
		it++;
	}
	cout << "( longitud " << plan.size();
	if (zap)
		cout << "[Z]";
	cout << ")\n";
}

void ComportamientoAuxiliar::AnularMatrizA(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

EstadoA ComportamientoAuxiliar::NextCasillaAuxiliar(const EstadoA &st)
{
	EstadoA siguiente = st;

	switch (st.brujula)
	{
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

EstadoA ComportamientoAuxiliar::applyA(Action accion, const EstadoA &st, const vector<vector<unsigned char>> &terreno,
									   const vector<vector<unsigned char>> &altura)
{
	EstadoA next = st;

	char i, f;
	int h;
	switch (accion)
	{
	case WALK:
	{
		if (CasillaTransitableAuxiliar(st, terreno, altura))
		{
			next = NextCasillaAuxiliar(st);
		}
		break;
	}
	case TURN_SR:
	{
		next.brujula = (next.brujula + 1) % 8;
		break;
	}
	}
	return next;
}

int ComportamientoAuxiliar::costeTerreno(char terrenoDestino, int cotaOrigen,
										 int cotaDestino, Action accion)
{
	int coste = 0;
	int coste_t, coste_a;
	int diferencia = cotaDestino - cotaOrigen;

	switch (accion)
	{
	case WALK:
		switch (terrenoDestino)
		{
		case 'A':
			coste_t = 100;
			coste_a = 10;
			break;
		case 'T':
			coste_t = 20;
			coste_a = 5;
			break;

		case 'S':
			coste_t = 2;
			coste_a = 1;
			break;

		default:
			coste_t = 1;
			coste_a = 0;
			break;
		}
		break;

	case TURN_SR:
		switch (terrenoDestino)
		{
		case 'A':
			coste_t = 16;
			break;
		case 'T':
			coste_t = 3;
			break;

		case 'S':
			coste_t = 1;
			break;

		default:
			coste_t = 1;
			break;
		}
		break;
	}

	if (diferencia > 0)
	{
		coste_t += coste_a;
	}
	else if (diferencia < 0)
	{

		coste_t -= coste_a; 
	}
	if (coste_t < 1)
		coste_t = 1;

	return coste_t;
}

void SituarSensorEnMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores)
{
	// Actualiza la casilla actual
	m[sensores.posF][sensores.posC] = sensores.superficie[0];

	// Actualiza las casillas adyacentes según la orientación
	switch (sensores.rumbo)
	{
	case norte:
		m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[1]; // Izquierda
		m[sensores.posF - 1][sensores.posC] = sensores.superficie[2];	  // Centro
		m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[3]; // Derecha
		break;

	case noreste:
		m[sensores.posF - 1][sensores.posC] = sensores.superficie[1];	  // Frente izquierda
		m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[2]; // Frente
		m[sensores.posF][sensores.posC + 1] = sensores.superficie[3];	  // Frente derecha
		break;

	case este:
		m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[1]; // Izquierda
		m[sensores.posF][sensores.posC + 1] = sensores.superficie[2];	  // Centro
		m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[3]; // Derecha
		break;

	case sureste:
		m[sensores.posF][sensores.posC + 1] = sensores.superficie[1];	  // Frente izquierda
		m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[2]; // Frente
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[3];	  // Frente derecha
		break;

	case sur:
		m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[1]; // Izquierda
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[2];	  // Centro
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[3]; // Derecha
		break;

	case suroeste:
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[1];	  // Frente izquierda
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[2]; // Frente
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[3];	  // Frente derecha
		break;

	case oeste:
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[1]; // Izquierda
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[2];	  // Centro
		m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[3]; // Derecha
		break;

	case noroeste:
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[1];	  // Frente izquierda
		m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[2]; // Frente
		m[sensores.posF - 1][sensores.posC] = sensores.superficie[3];	  // Frente derecha
		break;
	}
}

int ComportamientoAuxiliar::Heuristica(const EstadoA &origen, const EstadoA &destino) {
	int dist = abs(origen.f - destino.f) + abs(origen.c - destino.c); // Manhattan
	return dist;
}


list<Action> ComportamientoAuxiliar::AlgoritmoAEstrella(
	const EstadoA &origen,
	const EstadoA &destino,
	const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura)
{
	struct Nodo {
		EstadoA estado;
		list<Action> secuencia;
		int coste;
		int prioridad; // f = g + h
		bool operator>(const Nodo &n) const {
			return prioridad > n.prioridad;
		}
	};

	priority_queue<Nodo, vector<Nodo>, greater<Nodo>> frontera;
	set<EstadoA> explorados;

	Nodo inicio;
	inicio.estado = origen;
	inicio.secuencia = {};
	inicio.coste = 0;
	inicio.prioridad = Heuristica(origen, destino);

	frontera.push(inicio);

	while (!frontera.empty()) {
		Nodo actual = frontera.top();
		frontera.pop();

		if (actual.estado.f == destino.f && actual.estado.c == destino.c) {
			return actual.secuencia;
		}

		if (explorados.count(actual.estado)) continue;
		explorados.insert(actual.estado);

		// ACTUALIZACIÓN DE ZAPATILLAS SI ESTAMOS EN CASILLA 'D'
		bool tieneZapatillasAhora = actual.estado.zapatillas;
		if (terreno[actual.estado.f][actual.estado.c] == 'D') {
			tieneZapatillasAhora = true;

			cout <<"Se le ponen las zpatillas" << endl;
		}


		// Probar acciones: WALK + GIROS
		for (Action a : {TURN_SR,WALK}) {
			EstadoA sig = applyA(a, actual.estado, terreno, altura);

			// Actualizar estado de zapatillas para el nuevo nodo
			sig.zapatillas = tieneZapatillasAhora;

			if (sig.f == actual.estado.f && sig.c == actual.estado.c && a == WALK)
				continue; // Si no se mueve, no vale

			if (explorados.count(sig)) continue;



			int nuevoCoste = actual.coste + costeTerreno(
				terreno[actual.estado.f][actual.estado.c],
				altura[actual.estado.f][actual.estado.c],
				altura[sig.f][sig.c],
				a
			);


			list<Action> nuevaSec = actual.secuencia;
			nuevaSec.push_back(a);

			int heur = Heuristica(sig, destino);

			frontera.push({sig, nuevaSec, nuevoCoste, nuevoCoste + heur});
		}
	}

	return {}; // No se encontró camino
} 





















int VeoCasillaInteresanteA(char i, char c, char d, bool zap)
{
	if (c == 'X')
		return 2;
	else if (i == 'X')
		return 1;
	else if (d == 'X')
		return 3;

	else if (!zap)
	{
		if (c == 'D')
			return 2;
		else if (i == 'D')
			return 1;
		else if (d == 'D')
			return 3;
	}

	else if (zap)
	{
		if (c == 'D')
			return 2;
		else if (i == 'D')
			return 1;
		else if (d == 'D')
			return 3;
	}

	if (c == 'C')
		return 2;
	else if (d == 'C')
		return 3;
	else if (i == 'C')
		return 1;

	else
		return 0;
}

int VeoCasillaInteresanteA1(char i, char c, char d, bool zap, char mc, char mi, char md)
{

	// Prioridad 1: Casillas no exploradas ('?')
	if (mc == '?' && c != 'P' && c != 'M')
		return 2;
	if (mi == '?' && i != 'P' && i != 'M')
		return 1;
	if (md == '?' && d != 'P' && d != 'M')
		return 3;

	// Prioridad 2: Víctimas ('X')
	if (c == 'X')
		return 2;
	if (i == 'X')
		return 1;
	if (d == 'X')
		return 3;

	// Prioridad 3: Zapatillas ('D') si no las tenemos
	if (!zap)
	{
		if (c == 'D')
			return 2;
		if (i == 'D')
			return 1;
		if (d == 'D')
			return 3;
	}

	// Prioridad 4: Caminos ('C') y senderos ('S')
	if (c == 'C' || c == 'S' || (zap && c == 'B'))
		return 2;
	if (i == 'C' || i == 'S' || (zap && i == 'B'))
		return 1;
	if (d == 'C' || d == 'S' || (zap && d == 'B'))
		return 3;

	// Si no hay nada interesante
	return 0;
}

char ViablePorAlturaA(char casilla, int dif)
{
	if (abs(dif) <= 1)
	{
		return casilla;
	}
	else
	{
		return 'P';
	}
}

////////////////////////////////////////////////////

/*int ComportamientoAuxiliar::costeTerreno(char terrenoDestino, int cotaOrigen, int cotaDestino, Action accion,bool zapas)
{
	int coste = 0;
	int coste_t, coste_a;
	int diferencia  = cotaDestino-cotaOrigen;

	if (!tiene_zapatillas && terrenoDestino == 'B') {
		return 9999;  // Valor muy alto para indicar que no se puede pasar
	}

	// Si lleva zapatillas y el terreno es 'B', lo tratamos como 'C'
	char terrenoEvaluado = (zapas && terrenoDestino == 'B') ? 'C' : terrenoDestino;

	switch (accion)
	{
	case WALK:
		switch (terrenoEvaluado)
		{
		case 'A':
			coste_t = 100;
			coste_a = 10;
			break;
		case 'T':
			coste_t = 20;
			coste_a = 5;
			break;

		case 'S':
			coste_t = 2;
			coste_a = 1;
			break;

		default:
			coste_t = 1;
			coste_a = 0;
			break;
		}
		break;

	case TURN_SR:
		switch (terrenoDestino)
		{
		case 'A':
			coste_t = 16;
			break;
		case 'T':
			coste_t = 3;
			break;

		case 'S':
			coste_t = 1;
			break;

		default:
			coste_t = 1;
			break;
		}
		break;


	}

	if (diferencia == 1)
	{
		coste_t += coste_a;
	}
	else if (diferencia == -1)
	{

		coste_t -= coste_a;
	}
	if(coste_t < 1) coste_t = 1;

	return coste_t;

	/*
	if (cotaDestino - cotaOrigen > 0)
	{
		int dif1 = cotaDestino - cotaOrigen;
		coste_t += dif1;
	}
	else if (cotaOrigen - cotaDestino > 0)
	{
		int dif2 = cotaOrigen - cotaDestino;
		coste_t -= dif2;
	}


	return coste_t;
}

EstadoA ComportamientoAuxiliar::applyA(Action accion, const EstadoA &st, const vector<vector<unsigned char>> &terreno,
			   const vector<vector<unsigned char>> &altura)
{
	EstadoA next = st;
	switch (accion)
	{
	case WALK:
		if (CasillaAccesibleAuxiliar(st, terreno, altura))
		{
			next = NextCasillaAuxiliar(st);
		}
		break;
	case TURN_SR:
		next.brujula = (next.brujula + 1) % 8;
		break;
	}
	return next;
}

EstadoA ComportamientoAuxiliar::NextCasillaAuxiliar(const EstadoA &st)
{
	EstadoA siguiente = st;
	switch (st.brujula)
	{
	case norte:
		siguiente.f = st.f - 1;
		break;
	case noreste:
		siguiente.f = st.f - 1;
		siguiente.c = st.c + 1;
		break;
	case este:
		siguiente.c = st.c + 1;
		break;
	case sureste:
		siguiente.f = st.f + 1;
		siguiente.c = st.c + 1;
		break;
	case sur:
		siguiente.f = st.f + 1;
		break;
	case suroeste:
		siguiente.f = st.f + 1;
		siguiente.c = st.c - 1;
		break;
	case oeste:
		siguiente.c = st.c - 1;
		break;
	case noroeste:
		siguiente.f = st.f - 1;
		siguiente.c = st.c - 1;
	}
	return siguiente;
}



bool ComportamientoAuxiliar::CasillaAccesibleAuxiliar(const EstadoA &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura)
{
	EstadoA next = NextCasillaAuxiliar(st);
	bool check1 = false, check2 = false, check3 = false;
	check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
	check2 = (terreno[next.f][next.c] != 'B') or (st.zapatillas);
	check3 = (abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1) ;
	return check1 and check2 and check3;
}



void ComportamientoAuxiliar::AnularMatrizA(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}


bool ComportamientoAuxiliar::Find(const NodoA &st, const list<NodoA> &lista)
{
	auto it = lista.begin();

	while (it != lista.end() and !((*it) == st))
	{
		it++;
	}

	return (it != lista.end());
}


void ComportamientoAuxiliar::VisualizaPlan(const EstadoA &st, const list<Action> &plan)
{
	AnularMatrizA(mapaConPlan);
	EstadoA cst = st;
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



void ComportamientoAuxiliar::PintaPlan(const list<Action> &plan, bool zap)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == WALK)
		{
			cout << "W ";
		}
		else if (*it == RUN)
		{
			cout << "R ";
		}
		else if (*it == TURN_SR)
		{
			cout << "r ";
		}
		else if (*it == TURN_L)
		{
			cout << "L ";
		}
		else if (*it == CALL_ON)
		{
			cout << "C ";
		}
		else if (*it == CALL_OFF)
		{
			cout << "c ";
		}
		else if (*it == IDLE)
		{
			cout << "I ";
		}
		else
		{
			cout << "-_ ";
		}
		it++;
	}
	cout << "( longitud " << plan.size();
	if (zap)
		cout << "[Z]";
	cout << ")\n";
}


int ComportamientoAuxiliar::Heuristica(const EstadoA &origen, const EstadoA &destino) {
	int dist = abs(origen.f - destino.f) + abs(origen.c - destino.c); // Manhattan
	return dist;
}


list<Action> ComportamientoAuxiliar::AlgoritmoAEstrella(
	const EstadoA &origen,
	const EstadoA &destino,
	const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura)
{
	struct Nodo {
		EstadoA estado;
		list<Action> secuencia;
		int coste;
		int prioridad; // f = g + h
		bool operator>(const Nodo &n) const {
			return prioridad > n.prioridad;
		}
	};

	priority_queue<Nodo, vector<Nodo>, greater<Nodo>> frontera;
	set<EstadoA> explorados;

	Nodo inicio;
	inicio.estado = origen;
	inicio.secuencia = {};
	inicio.coste = 0;
	inicio.prioridad = Heuristica(origen, destino);

	frontera.push(inicio);

	while (!frontera.empty()) {
		Nodo actual = frontera.top();
		frontera.pop();

		if (actual.estado.f == destino.f && actual.estado.c == destino.c) {
			return actual.secuencia;
		}

		if (explorados.count(actual.estado)) continue;
		explorados.insert(actual.estado);

		// ACTUALIZACIÓN DE ZAPATILLAS SI ESTAMOS EN CASILLA 'D'
		bool tieneZapatillasAhora = actual.estado.zapatillas;
		if (terreno[actual.estado.f][actual.estado.c] == 'D') {
			tieneZapatillasAhora = true;

			cout <<"Se le ponen las zpatillas" << endl;
		}


		// Probar acciones: WALK + GIROS
		for (Action a : {TURN_SR,WALK}) {
			EstadoA sig = applyA(a, actual.estado, terreno, altura);

			// Actualizar estado de zapatillas para el nuevo nodo
			sig.zapatillas = tieneZapatillasAhora;

			if (sig.f == actual.estado.f && sig.c == actual.estado.c && a == WALK)
				continue; // Si no se mueve, no vale

			if (explorados.count(sig)) continue;



			int nuevoCoste = actual.coste + costeTerreno(
				terreno[actual.estado.f][actual.estado.c],
				altura[actual.estado.f][actual.estado.c],
				altura[sig.f][sig.c],
				a,tieneZapatillasAhora
			);


			list<Action> nuevaSec = actual.secuencia;
			nuevaSec.push_back(a);

			int heur = Heuristica(sig, destino);

			frontera.push({sig, nuevaSec, nuevoCoste, nuevoCoste + heur});
		}
	}

	return {}; // No se encontró camino
} */

////////////////////////////////////////////////////////////////

list<Action> ComportamientoAuxiliar::AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
	NodoA current_node;
	list<NodoA> frontier;
	list<NodoA> explored;
	list<Action> path;

	current_node.estado = inicio; // Asignar estado a nodo inicial
	frontier.push_back(current_node);
	bool SolutionFound = (current_node.estado.f == final.f and current_node.estado.c == final.c);
	while (!SolutionFound and !frontier.empty())
	{
		frontier.pop_front();
		explored.push_back(current_node);

		// Se comprueba si estoy en casilla de zapatillas
		if (terreno[current_node.estado.f][current_node.estado.c] == 'D')
		{
			current_node.estado.zapatillas = true;
		}

		// Genera el hijo resultante de aplicar walk
		NodoA child_WALK = current_node;
		// NO ENCUENTRO applyA
		// child_WALK.estado = apply(WALK,current_node.estado,terreno,altura);
		if (child_WALK.estado.f == final.f && child_WALK.estado.c == final.c)
		{
			// El hijo generado es solucion
			child_WALK.secuencia.push_back(WALK);
			current_node = child_WALK;
			SolutionFound = true;
		}
		else if (!Find(child_WALK, frontier) and !Find(child_WALK, explored))
		{
			// Se mete en la lista frontier después de añadir a secuencia la acción
			child_WALK.secuencia.push_back(WALK);
			frontier.push_back(child_WALK);
		}
		// Genero el hijo resultante de aplicar la acción TURN_SR
		if (!SolutionFound)
		{
			NodoA child_TURN_SR = current_node;
			child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);

			if (!Find(child_TURN_SR, frontier) and !Find(child_TURN_SR, explored))
			{
				child_TURN_SR.secuencia.push_back(TURN_SR);
				frontier.push_back(child_TURN_SR);
			}
		}
		// Paso a evaluar el siguiente nodo en la lista "frontier"
		if (!SolutionFound and !frontier.empty())
		{
			current_node = frontier.front();
			SolutionFound = (current_node.estado.f == final.f and current_node.estado.c == final.c);
		}
	}

	if (SolutionFound)
		path = current_node.secuencia;

	return path;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	Action accion;

	if (sensores.superficie[2] == 'X' && sensores.agentes[2] == 'r')
	{
		giro45Izq = 3;
		accion = TURN_SR;
	}

	if (sensores.superficie[0] == 'D')
		tiene_zapatillas = true;

	if (last_action == WALK)
	{
		matrizAux[sensores.posF][sensores.posC]++;
	}

	if (matrizAux[sensores.posF][sensores.posC] >= 6)
	{
		// Está en bucle: aplica una acción distinta
		giro45Izq = rand() % 5;
		accion = TURN_SR;							 // TURN_SR aleatorio
		matrizAux[sensores.posF][sensores.posC] = 1; // resetea el contador
		return accion;
	}

	if (sensores.superficie[0] == 'X')
	{
		accion = IDLE;
	}

	else if (giro45Izq != 0)
	{
		accion = TURN_SR;
		giro45Izq--;
	}

	else
	{
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);

		if (sensores.agentes[2] == 'r')
		{
			c = 'P';
		}

		int pos = VeoCasillaInteresanteA(i, c, d, tiene_zapatillas);
		switch (pos)
		{
		case 2:
			accion = WALK;
			break;
		case 1:
			giro45Izq = 6;
			accion = TURN_SR;
			break;
		case 3:
			accion = TURN_SR;
			break;
		case 0:
			giro45Izq = 5;
			accion = TURN_SR;
			break;
		}
	}

	last_action = accion;
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
	Action accion;

	if (sensores.superficie[0] == 'D')
		tiene_zapatillas = true;

	SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);

	if (sensores.superficie[2] == 'X' && sensores.agentes[2] == 'r')
	{
		giro45Izq = 3;
		accion = TURN_SR;
	}

	if (last_action == WALK)
	{
		matrizAux[sensores.posF][sensores.posC]++;
	}

	if (matrizAux[sensores.posF][sensores.posC] >= 4)
	{
		// Está en bucle: aplica una acción distinta
		giro45Izq = rand() % 5;
		accion = TURN_SR;							 // TURN_SR aleatorio
		matrizAux[sensores.posF][sensores.posC] = 1; // resetea el contador
		return accion;
	}

	else if (giro45Izq != 0)
	{
		accion = TURN_SR;
		giro45Izq--;
	}

	else
	{
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);

		if (sensores.agentes[2] == 'r')
		{
			c = 'P';
		}
		char mi = mapaResultado[sensores.posF][sensores.posC - 1]; // izquierda
		char mc = mapaResultado[sensores.posF - 1][sensores.posC]; // centro
		char md = mapaResultado[sensores.posF][sensores.posC + 1]; // derecha

		int pos = VeoCasillaInteresanteA1(i, c, d, tiene_zapatillas, mi, mc, md);
		switch (pos)
		{
		case 2:
			accion = WALK;
			break;
		case 1:
			giro45Izq = 6;
			accion = TURN_SR;
			break;
		case 3:
			accion = TURN_SR;
			break;
		case 0:
			giro45Izq = 5;
			accion = TURN_SR;
			break;
		}
	}

	last_action = accion;
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
	return IDLE;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{



	
	Action accion = IDLE;

	if (sensores.superficie[0] == 'D')
	{
		tiene_zapatillas = true;
	}

	if (!hayPlan)
	{
		// Estado origen
		EstadoA origen;
		origen.f = sensores.posF;
		origen.c = sensores.posC;
		origen.brujula = sensores.rumbo;
		origen.zapatillas = tiene_zapatillas;

		// Estado destino
		EstadoA destino;
		destino.f = sensores.destinoF;
		destino.c = sensores.destinoC;
		destino.brujula = 0; // La orientación final no es crítica normalmente
		destino.zapatillas = false;

		// Calcular plan con A*
		plan = AlgoritmoAEstrella(origen, destino, mapaResultado, mapaCotas);
		hayPlan = !plan.empty();
	}

	if (hayPlan)
	{
		EstadoA estadoActual;
		estadoActual.f = sensores.posF;
		estadoActual.c = sensores.posC;
		estadoActual.brujula = sensores.rumbo;
		estadoActual.zapatillas = tiene_zapatillas;

		VisualizaPlan(estadoActual, plan);
	}

	if (hayPlan && !plan.empty())
	{
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.empty())
	{
		hayPlan = false;
	}
	return accion;
}


	

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
	Action accion = IDLE;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_E(Sensores sensores)
{
	Action accion = IDLE;
	if (!hayPlan)
	{
		// Invocar al método de búsqueda
		EstadoA inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = AnchuraAuxiliar(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}
	if (hayPlan and plan.size() > 0)
	{
		accion = plan.front();
		plan.pop_front();
	}

	if (plan.size() == 0)
	{
		hayPlan = false;
	}
	return accion;
}