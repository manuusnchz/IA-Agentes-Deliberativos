#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include "motorlib/util.h"




Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0 (sensores);
		break;
	case 1:
		accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoAuxiliarNivel_3 (sensores);
		accion = ComportamientoAuxiliarNivel_E(sensores);
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

int VeoCasillaInteresanteA(char i, char c, char d, bool zap){
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
	else if( d == 'C') return 3;
	else if (i == 'C')return 1;
	
	

	
	else return 0;

}

char ViablePorAlturaA(char casilla, int dif){
	if(abs(dif)<=1 ){
		return casilla;
	}
	else{
		return 'P';
	}
}

EstadoA NextCasillaAuxiliar(const EstadoA &st){
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

list<Action> AvanzaASaltosDeCaballo(){
	list<Action> secuencia;
	secuencia.push_back(WALK);
	secuencia.push_back(WALK);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(WALK);
	return secuencia;
}

bool CasillaAccesibleAuxiliar(const EstadoA &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura)
{
	EstadoA next = NextCasillaAuxiliar(st);
	bool check1 = false, check2 = false, check3 = false;
	check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
	check2 = terreno[next.f][next.c] != 'B' or (terreno[next.f][next.c] == 'B' and st.zapatillas);
	check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;
	return check1 and check2 and check3;
}



EstadoA applyA(Action accion, const EstadoA & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura)
{
	EstadoA next = st;
	switch(accion){
	case WALK:
	if (CasillaAccesibleAuxiliar(st,terreno,altura)){
	next = NextCasillaAuxiliar(st);
	}
	break;
	case TURN_SR:
	next.brujula = (next.brujula+1)%8;
	break;
	}
	return next;
}

bool Find (const NodoA & st, const list<NodoA> &lista){
	auto it = lista.begin();
	
	while (it != lista.end() and !((*it) == st)){
		it++;
	}
	
	return (it != lista.end());
}



list<Action> ComportamientoAuxiliar::AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final,const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura ){
	NodoA current_node;
	list<NodoA> frontier;
	list<NodoA> explored;
	list<Action> path;

	current_node.estado = inicio; // Asignar estado a nodo inicial
	frontier.push_back(current_node);
	bool SolutionFound = (current_node.estado.f == final.f and current_node.estado.c == final.c);
	while(!SolutionFound and !frontier.empty()){
		frontier.pop_front();
		explored.push_back(current_node);

		//Se comprueba si estoy en casilla de zapatillas
		if(terreno[current_node.estado.f][current_node.estado.c] == 'D'){
			current_node.estado.zapatillas = true;
		}

		//Genera el hijo resultante de aplicar walk
		NodoA child_WALK = current_node;
		//NO ENCUENTRO applyA
		//child_WALK.estado = apply(WALK,current_node.estado,terreno,altura);
		if(child_WALK.estado.f == final.f && child_WALK.estado.c == final.c){
			//El hijo generado es solucion
			child_WALK.secuencia.push_back(WALK);
			current_node = child_WALK;
			SolutionFound = true;
		}
		else if (!Find(child_WALK, frontier) and !Find(child_WALK, explored)){
			// Se mete en la lista frontier después de añadir a secuencia la acción
			child_WALK.secuencia.push_back(WALK);
			frontier.push_back(child_WALK);
		}
		// Genero el hijo resultante de aplicar la acción TURN_SR
		if (!SolutionFound){
    		NodoA child_TURN_SR = current_node;
    		child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
		
    		if (!Find(child_TURN_SR, frontier) and !Find(child_TURN_SR, explored)){
        		child_TURN_SR.secuencia.push_back(TURN_SR);
        		frontier.push_back(child_TURN_SR);
    		}
		}
		// Paso a evaluar el siguiente nodo en la lista "frontier"
		if (!SolutionFound and !frontier.empty()){
    		current_node = frontier.front();
    			SolutionFound = (current_node.estado.f == final.f and current_node.estado.c == final.c);
		}

	}

	if(SolutionFound) path = current_node.secuencia;

	return path;
}
void AnularMatrizA(vector<vector<unsigned char>> &m)
{
for (int i = 0; i < m[0].size(); i++)
{
for (int j = 0; j < m.size(); j++)
{
m[i][j] = 0;
}
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
			mapaConPlan[cst.f][cst.c] = 2;
			break;
		case TURN_SR:
			cst.brujula = (cst.brujula + 1) % 8;
			break;
		}
		it++;
	}
}



Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	Action accion;

	if(sensores.superficie[2] == 'X' && sensores.agentes[2] == 'r'){
		giro45Izq = 3;
		accion = TURN_SR;
	}

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(last_action == WALK){
		matrizAux[sensores.posF][sensores.posC]++;
	}

	if (matrizAux[sensores.posF][sensores.posC] >= 6) {
		// Está en bucle: aplica una acción distinta
		giro45Izq = rand() % 5;
		accion = TURN_SR;  // TURN_SR aleatorio
		matrizAux[sensores.posF][sensores.posC] = 1; // resetea el contador
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
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1]-sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2]-sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3]-sensores.cota[0]);

		if(sensores.agentes[2] == 'r'){
			c = 'P';
		}
		

		
		int pos = VeoCasillaInteresanteA(i,c,d,tiene_zapatillas);
		switch(pos)
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
	
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_E(Sensores sensores){
	Action accion = IDLE;
	if (!hayPlan){
		// Invocar al método de búsqueda
		EstadoA inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = AnchuraAuxiliar(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio,plan);
		hayPlan = plan.size() != 0 ;
	}
	if (hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}
	
	if (plan.size()== 0){
		hayPlan = false;
	}
	return accion;
}