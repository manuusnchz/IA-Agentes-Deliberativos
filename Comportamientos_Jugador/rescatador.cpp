#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"
#include <queue>	// For priority_queue
#include <set>		// For set
#include <map>		// For unordered_map
#include <iostream> // For cout

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0(sensores);
		break;
	case 1:
		accion = ComportamientoRescatadorNivel_1(sensores);
		break;
	case 2:
		accion = ComportamientoRescatadorNivel_2(sensores);
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


/*FUNCIONES COMUNES NIVEL 0 Y 1*/
char ViablePorAlturaR(char casilla, int dif, bool zap)
{
	if (abs(dif) <= 1 or (zap and abs(dif) <= 2))
	{
		return casilla;
	}
	else
	{
		return 'P';
	}
}

void SituarSensorEnMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores)
{
	m[sensores.posF][sensores.posC] = sensores.superficie[0];

	int pos = 1;
	switch (sensores.rumbo)
	{
	case norte:
		m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[1];
		m[sensores.posF - 1][sensores.posC] = sensores.superficie[2];
		m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[3];
		break;

	case noreste:
		m[sensores.posF - 1][sensores.posC] = sensores.superficie[1];
		m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC + 1] = sensores.superficie[3];
		break;

	case este:
		m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[1];
		m[sensores.posF][sensores.posC + 1] = sensores.superficie[2];
		m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[3];
		break;

	case sureste:
		m[sensores.posF][sensores.posC + 1] = sensores.superficie[1];
		m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[2];
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[3];
		break;

	case sur:
		m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[1];
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[2];
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[3];
		break;

	case suroeste:
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[1];
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[3];
		break;

	case oeste:
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[1];
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[2];
		m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[3];
		break;

	case noroeste:
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[1];
		m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[2];
		m[sensores.posF - 1][sensores.posC] = sensores.superficie[3];
		break;
	}
}

/*FUNCIONES NIVEL 0*/
int VeoCasillaInteresanteR(char i, char c, char d, bool zap)
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
	else if (i == 'C')
		return 1;
	else if (d == 'C')
		return 3;

	else
		return 0;
	
}

/*FUNCIONES NIVEL 1*/
int VeoCasillaInteresanteR1(char i, char c, char d, bool zap, int ti, int tc, int td)
{
	int minimo = 6000;
	int salida = 0;

	vector<int> valores(3, -1);

	// Prioridad 1: Casillas no exploradas y que sean caminos
	if (c == 'S' or c == 'C' or c == 'X' or c == 'D')
	{
		valores[0] = tc;
		if (tc < minimo)
		{
			minimo = tc;
			salida = 2;
		}
	}
	if (i == 'S' or i == 'C' or i == 'X' or i == 'D')
	{
		valores[1] = ti;
		if (ti < minimo)
		{
			minimo = ti;
			salida = 1;
		}
	}
	if (d == 'S' or d == 'C' or d == 'X' or d == 'D')
	{
		valores[2] = td;
		if (td < minimo)
		{
			minimo = td;
			salida = 3;
		}
	}
	
	return salida;
}

pair<int, int> Funcion_delante(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores)
{
	pair<int, int> salida;

	switch (sensores.rumbo)
	{
	case norte:
		return make_pair(sensores.posF - 1, sensores.posC);
		break;

	case noreste:

		return make_pair(sensores.posF - 1, sensores.posC + 1);
		break;

	case este:

		return make_pair(sensores.posF, sensores.posC + 1);
		break;

	case sureste:

		return make_pair(sensores.posF + 1, sensores.posC + 1);
		break;

	case sur:

		return make_pair(sensores.posF + 1, sensores.posC);
		break;

	case suroeste:

		return make_pair(sensores.posF + 1, sensores.posC - 1);
		break;

	case oeste:

		return make_pair(sensores.posF, sensores.posC - 1);
		break;

	case noroeste:

		return make_pair(sensores.posF - 1, sensores.posC - 1);
		break;
	}
}

/*FUNCIONES NIVEL 2*/
bool CasillaTransitableRescatadorRUN(char casilla_intermedia,
									 char casilla_final, int dif_alt, bool zapatillas)
{
	bool check1 = casilla_intermedia != 'P' && casilla_intermedia != 'M' && casilla_intermedia != 'B';
	bool check2 = casilla_final != 'P' && casilla_final != 'M' && casilla_final != 'B';

	bool check3 = abs(dif_alt) <= 1;
	if (!check3)
	{
		if (zapatillas && abs(dif_alt) <= 2)
		{
			check3 = true;
		}
	}
	return check1 && check2 && check3;
}

bool ComportamientoRescatador::CasillaTransitableRescatador(const EstadoR &st,
															const vector<vector<unsigned char>> &terreno,
															const vector<vector<unsigned char>> &altura)
{

	EstadoR next = NextCasillaRescatador(st);

	// Comprobar límites del mapa
	if (next.f < 0 || next.f >= terreno.size() ||
		next.c < 0 || next.c >= terreno[0].size())
	{
		return false;
	}

	// Comprobar obstáculos
	if (terreno[next.f][next.c] == 'P' || terreno[next.f][next.c] == 'M' || terreno[next.f][next.c] == 'B')
	{
		return false;
	}

	// Comprobar diferencia de altura
	int dif_altura = abs(altura[next.f][next.c] - altura[st.f][st.c]);

	if (dif_altura <= 1)
	{
		return true;
	}
	else if (dif_altura <= 2 && st.zapatillas)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ComportamientoRescatador::VisualizaPlan(const EstadoR &st, const list<Action> &plan)
{
	AnularMatrizR(mapaConPlan);
	EstadoR cst = st;
	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
		case RUN:
		{
			// Simular el primer paso de RUN
			EstadoR estado_intermedio = cst;
			switch (estado_intermedio.brujula)
			{
			case 0:
				estado_intermedio.f--;
				break; // Norte
			case 1:
				estado_intermedio.f--;
				estado_intermedio.c++;
				break; // Noreste
			case 2:
				estado_intermedio.c++;
				break; // Este
			case 3:
				estado_intermedio.f++;
				estado_intermedio.c++;
				break; // Sureste
			case 4:
				estado_intermedio.f++;
				break; // Sur
			case 5:
				estado_intermedio.f++;
				estado_intermedio.c--;
				break; // Suroeste
			case 6:
				estado_intermedio.c--;
				break; // Oeste
			case 7:
				estado_intermedio.f--;
				estado_intermedio.c--;
				break; // Noroeste
			}

			// Marcar la casilla intermedia si está dentro de los límites
			if (estado_intermedio.f >= 0 && estado_intermedio.f < mapaConPlan.size() &&
				estado_intermedio.c >= 0 && estado_intermedio.c < mapaConPlan[0].size())
			{
				// Marca el paso intermedio 
				mapaConPlan[estado_intermedio.f][estado_intermedio.c] = 3; 
			}

			// Simular el segundo paso de RUN desde el estado intermedio
			EstadoR estado_final = estado_intermedio;
			
			// La brújula no cambia durante RUN, por lo que usamos la del estado intermedio/original
			switch (estado_final.brujula)
			{
			case 0:
				estado_final.f--;
				break; // Norte
			case 1:
				estado_final.f--;
				estado_final.c++;
				break; // Noreste
			case 2:
				estado_final.c++;
				break; // Este
			case 3:
				estado_final.f++;
				estado_final.c++;
				break; // Sureste
			case 4:
				estado_final.f++;
				break; // Sur
			case 5:
				estado_final.f++;
				estado_final.c--;
				break; // Suroeste
			case 6:
				estado_final.c--;
				break; // Oeste
			case 7:
				estado_final.f--;
				estado_final.c--;
				break; // Noroeste
			}

			// Marcar la casilla final si está dentro de los límites
			if (estado_final.f >= 0 && estado_final.f < mapaConPlan.size() &&
				estado_final.c >= 0 && estado_final.c < mapaConPlan[0].size())
			{
				// Marca el paso final
				mapaConPlan[estado_final.f][estado_final.c] = 3; 
			}

			// Actualizar el estado temporal de visualización a la posición final
			
			cst = estado_final;
			
			// La brújula y las zapatillas no cambian con RUN, se mantienen las del estado original cst.

			break;
		}
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
		case TURN_L:
			cst.brujula = (cst.brujula + 6) % 8;
			break;
		}
		it++;
	}
}

void ComportamientoRescatador::PintaPlan(const list<Action> &plan, bool zap)
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

void ComportamientoRescatador::AnularMatrizR(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

EstadoR ComportamientoRescatador::NextCasillaRescatador(const EstadoR &st)
{
	EstadoR siguiente = st;

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

EstadoR ComportamientoRescatador::applyR(Action accion, const EstadoR &st, const vector<vector<unsigned char>> &terreno,
										 const vector<vector<unsigned char>> &altura)
{
	EstadoR next = st;

	char i, f;
	int h;
	switch (accion)
	{
	case WALK:
	{
		if (CasillaTransitableRescatador(st, terreno, altura))
		{
			next = NextCasillaRescatador(st);
		}
		break;
	}
	case RUN:
	{
		// Primero calculamos la casilla intermedia (la que se salta)
		EstadoR estado_intermedio = NextCasillaRescatador(st);

		// Creamos una copia del valor actual de zapatillas
		bool tiene_zapatillas = st.zapatillas;

		// Si la casilla intermedia contiene una 'D', activamos las zapatillas
		if (mapaResultado[estado_intermedio.f][estado_intermedio.c] == 'D')
		{
			tiene_zapatillas = true;
		}

		// Luego calculamos la siguiente casilla (la de destino)
		EstadoR estado_final = NextCasillaRescatador(estado_intermedio);

		// Verificamos solo si la segunda casilla (destino) es transitable
		if (CasillaTransitableRescatadorRUN(mapaResultado[estado_intermedio.f][estado_intermedio.c], 
											mapaResultado[estado_final.f][estado_final.c],
											mapaCotas[st.f][st.c] - mapaCotas[estado_final.f][estado_final.c], tiene_zapatillas))
		{
			next = estado_final;
			next.brujula = st.brujula;			
			next.zapatillas = tiene_zapatillas; 
		}
		break;
	}
	case TURN_SR:
	{
		next.brujula = (next.brujula + 1) % 8;
		break;
	}
	case TURN_L:
	{
		next.brujula = (next.brujula + 6) % 8;
		break;
	}
	}

	return next;
}

int ComportamientoRescatador::costeTerreno(char terrenoDestino,int cotaOrigen,
										   int cotaDestino,Action accion)
{
	int coste = 0;
	int coste_t, coste_a;
	int diferencia  = cotaDestino-cotaOrigen;

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

	case RUN:
		switch (terrenoDestino)
		{
		case 'A':
			coste_t = 150;
			coste_a = 15;
			break;
		case 'T':
			coste_t = 35;
			coste_a = 5;
			break;

		case 'S':
			coste_t = 3;
			coste_a = 2;
			break;

		default:
			coste_t = 1;
			coste_a = 0;
			break;
		}
		break;

	case TURN_L:
		switch (terrenoDestino)
		{
		case 'A':
			coste_t = 30;
			break;
		case 'T':
			coste_t = 5;
			break;

		case 'S':
			coste_t = 1;
			break;

		default:
			coste_t = 1;
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
		
		coste_t -= coste_a;/**/
	}
	if(coste_t < 1) coste_t = 1;

	return coste_t;
}

bool ComportamientoRescatador::AlgoritmoDkjistra(const EstadoR &origen, const EstadoR &destino, list<Action> &plan)
{
	plan.clear();

	priority_queue<NodoR, std::vector<NodoR>, CompararNodoR> frontera;
	set<EstadoR> visitados;
	NodoR currentNodo;

	currentNodo.estado = origen;
	currentNodo.coste_total = 0;
	currentNodo.secuencia.clear();

	bool solution = (origen.f == destino.f && origen.c == destino.c);
	frontera.push(currentNodo);

	while (!frontera.empty() && !solution)
	{
		currentNodo = frontera.top();
		frontera.pop();

		
		
		if (mapaResultado[currentNodo.estado.f][currentNodo.estado.c] == 'D')
		{
			currentNodo.estado.zapatillas = true;
		}



		if (visitados.count(currentNodo.estado))
			continue;
		visitados.insert(currentNodo.estado);

		if (currentNodo.estado.f == destino.f && currentNodo.estado.c == destino.c)
		{
			plan = currentNodo.secuencia;
			return true;
		}

		// ===== Generar sucesores =====

		// WALK
		NodoR walkNode = currentNodo;
		walkNode.estado = applyR(WALK, currentNodo.estado, mapaResultado, mapaCotas);
		if (walkNode.estado != currentNodo.estado && !visitados.count(walkNode.estado))
		{
			walkNode.coste_total += costeTerreno(
				mapaResultado[currentNodo.estado.f][currentNodo.estado.c],
				mapaCotas[currentNodo.estado.f][currentNodo.estado.c],
				mapaCotas[walkNode.estado.f][walkNode.estado.c],
				WALK);
			walkNode.secuencia.push_back(WALK);
			frontera.push(walkNode);
		}

		// RUN
		NodoR runNode = currentNodo;
		runNode.estado = applyR(RUN, currentNodo.estado, mapaResultado, mapaCotas);
		if (runNode.estado != currentNodo.estado && !visitados.count(runNode.estado))
		{
			runNode.coste_total += costeTerreno(
				mapaResultado[currentNodo.estado.f][currentNodo.estado.c],
				mapaCotas[currentNodo.estado.f][currentNodo.estado.c],
				mapaCotas[runNode.estado.f][runNode.estado.c],
				RUN);
			runNode.secuencia.push_back(RUN);
			frontera.push(runNode);
		}

		// TURN_SR
		NodoR srNode = currentNodo;
		srNode.estado = applyR(TURN_SR, currentNodo.estado, mapaResultado, mapaCotas);
		if (srNode.estado != currentNodo.estado && !visitados.count(srNode.estado))
		{
			srNode.coste_total += costeTerreno(
				mapaResultado[currentNodo.estado.f][currentNodo.estado.c],
				mapaCotas[currentNodo.estado.f][currentNodo.estado.c],
				mapaCotas[currentNodo.estado.f][currentNodo.estado.c],
				TURN_SR);
			srNode.secuencia.push_back(TURN_SR);
			frontera.push(srNode);
		}

		// TURN_L
		NodoR lNode = currentNodo;
		lNode.estado = applyR(TURN_L, currentNodo.estado, mapaResultado, mapaCotas);
		if (lNode.estado != currentNodo.estado && !visitados.count(lNode.estado))
		{
			lNode.coste_total += costeTerreno(
				mapaResultado[currentNodo.estado.f][currentNodo.estado.c],
				mapaCotas[currentNodo.estado.f][currentNodo.estado.c],
				mapaCotas[currentNodo.estado.f][currentNodo.estado.c],
				TURN_L);
			lNode.secuencia.push_back(TURN_L);
			frontera.push(lNode);
		}
	}

	return false;
}




Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	Action accion;

	if (sensores.superficie[0] == 'D')
		tiene_zapatillas = true;

	SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);

	if (last_action == WALK)
	{
		matriz[sensores.posF][sensores.posC]++;
	}

	if (matriz[sensores.posF][sensores.posC] >= 4)
	{
		// Está en bucle: aplica una acción distinta
		giro45Izq = rand() % 5;
		accion = TURN_SR;						  // TURN_SR aleatorio
		matriz[sensores.posF][sensores.posC] = 1; // resetea el contador
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

		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

		// Detectamos si hay un auxiliar delante

		if (sensores.agentes[2] == 'a')
		{
			c = 'P';
		}

		int pos = VeoCasillaInteresanteR(i, c, d, tiene_zapatillas);
		switch (pos)
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
	instante++;
	matrizTempR[sensores.posF][sensores.posC] = instante;

	if (sensores.superficie[0] == 'D')
		tiene_zapatillas = true;

	SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);

	if (giro45Izq != 0)
	{
		accion = TURN_SR;
		giro45Izq--;
	}

	else
	{

		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

		// Detectamos si hay un auxiliar delante

		if (sensores.agentes[2] == 'a')
		{
			c = 'P';
		}

		pair<int, int> posicion = Funcion_delante(mapaResultado, mapaCotas, sensores);

		int tc = matrizTempR[posicion.first][posicion.second]; // centro
		Sensores copia_sensores = sensores;
		copia_sensores.rumbo = static_cast<Orientacion>((copia_sensores.rumbo + 7) % 8);
		posicion = Funcion_delante(mapaResultado, mapaCotas, copia_sensores);
		int ti = matrizTempR[posicion.first][posicion.second]; // izquierda
		copia_sensores.rumbo = static_cast<Orientacion>((sensores.rumbo + 1) % 8);
		posicion = Funcion_delante(mapaResultado, mapaCotas, copia_sensores);
		int td = matrizTempR[posicion.first][posicion.second]; // derecha

		int pos = VeoCasillaInteresanteR1(i, c, d, tiene_zapatillas, ti, tc, td);

		switch (pos)
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

	if (sensores.superficie[0] == 'D')
	{
		tiene_zapatillas = true;
	}

	if (!hayPlan)
	{
		// Se construye el estado inicial
		origen.f = sensores.posF;
		origen.c = sensores.posC;
		origen.brujula = sensores.rumbo;
		origen.zapatillas = tiene_zapatillas;

		// Se construye el estado de destino
		destino.f = sensores.destinoF;
		destino.c = sensores.destinoC;
		destino.brujula = 0;
		destino.zapatillas = false; // No nos importa si tiene o no zapatillas en destino

		// Se ejeecuta el algoritmo de Dijisktra
		hayPlan = AlgoritmoDkjistra(origen, destino, plan);
	}
	if (hayPlan)
	{
		EstadoR estadoActual;
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

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
	return IDLE;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}
