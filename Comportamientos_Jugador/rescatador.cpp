#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"
#include <queue> // For priority_queue
#include <set>   // For set
#include <map> // For unordered_map
#include <iostream> // For cout

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
		//accion = ComportamientoRescatadorNivel_2 (sensores);
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
/*

////////////////////////////////////////////////
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
mapaConPlan[cst.f][cst.c] = 3;
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
if (zap) cout << "[Z]";
cout << ")\n";
}

void AnularMatrizR(vector<vector<unsigned char>> &m)
{
for (int i = 0; i < m[0].size(); i++)
{
for (int j = 0; j < m.size(); j++)
{
m[i][j] = 0;
}
}
}

bool CasillaAccesibleAuxiliar(const EstadoR &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoR next = NextCasillaAuxiliar(st);
	bool check1 = false, check2 = false, check3 = false;
	check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
	check2 = terreno[next.f][next.c] != 'B' or (terreno[next.f][next.c] == 'B' and
	st.zapatillas);
	check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;
	return check1 and check2 and check3;
	}

EstadoR NextCasillaAuxiliar(const EstadoR &st){
	EstadoR siguiente = st;
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

EstadoR applyA(Action accion, const EstadoR & st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoR next = st;
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




	#include <queue>
	#include <set>
	#include <limits>
	
	list<Action> pathFindingDijkstra(EstadoR origen, EstadoR destino,
		const vector<vector<unsigned char>> &terreno,
		const vector<vector<unsigned char>> &altura) {
set<EstadoR> visitados;
priority_queue<NodoR, vector<NodoR>, greater<NodoR>> abiertos;

origen.coste_acumulado = 0;
abiertos.push({origen, {}, 0});

while (!abiertos.empty()) {
NodoR actual = abiertos.top();
abiertos.pop();

if (visitados.count(actual.estado)) continue;
visitados.insert(actual.estado);

// Objetivo alcanzado
if (actual.estado.f == destino.f && actual.estado.c == destino.c) {
return actual.secuencia;
}

// Avanzar si es accesible
if (CasillaAccesibleAuxiliar(actual.estado, terreno, altura)) {
EstadoR sig = NextCasillaAuxiliar(actual.estado);
sig.zapatillas = actual.estado.zapatillas;
sig.coste_acumulado = actual.estado.coste_acumulado + 1;

if (!visitados.count(sig)) {
list<Action> nuevaSec = actual.secuencia;
nuevaSec.push_back(WALK);
abiertos.push({sig, nuevaSec, sig.coste_acumulado});
}
}

// Girar derecha
{
EstadoR sig = actual.estado;
sig.brujula = (sig.brujula + 1) % 8;
sig.coste_acumulado = actual.estado.coste_acumulado + 1;

if (!visitados.count(sig)) {
list<Action> nuevaSec = actual.secuencia;
nuevaSec.push_back(TURN_SR);
abiertos.push({sig, nuevaSec, sig.coste_acumulado});
}
}

// Girar izquierda
{
EstadoR sig = actual.estado;
sig.brujula = (sig.brujula + 7) % 8;
sig.coste_acumulado = actual.estado.coste_acumulado + 1;

if (!visitados.count(sig)) {
list<Action> nuevaSec = actual.secuencia;
nuevaSec.push_back(TURN_L);
abiertos.push({sig, nuevaSec, sig.coste_acumulado});
}
}
}

// Si no hay camino
return {};
}

	
/////////////////////////////////////////////
*/

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


int VeoCasillaInteresanteR1(char i, char c, char d, bool zap,int ti, int tc, int td){
	int minimo =6000;
	int salida = 0; 

	vector<int> valores(3,-1);
	
	// Prioridad 1: Casillas no exploradas y que sean caminos
	if (c == 'S' or c == 'C' or c == 'X' or c=='D'){
		valores[0] = tc;
		if(tc < minimo){
			minimo = tc;
			salida = 2;
			
		}
	}
	if (i == 'S' or i == 'C' or i == 'X' or i=='D'){
		valores[1] = ti;
		if(ti < minimo){
			minimo = ti;
			salida = 1;
		}
	}
	if (d== 'S' or d == 'C' or d == 'X' or d=='D'){
		valores[2] = td;
		if(td < minimo){
			minimo = td;
			salida = 3;
		}
	}
	for(int i = 0 ; i < 3 ; i++){
		cout << valores[i]<< " ";
	}
	cout << " salida = " << salida << endl;
	// Si no hay nada interesante
	return salida;
	

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




void ComportamientoRescatador::AnularMatrizR(vector<vector<unsigned char>> &m) {
    for (auto &fila : m) {
        fill(fila.begin(), fila.end(), 0);
    }
}

pair<int,int> Funcion_delante(vector<vector<unsigned char>> &m, vector<vector<unsigned char >> &a, Sensores sensores)
{
	pair<int,int> salida;

	switch (sensores.rumbo)
	{
	case norte:
		return make_pair(sensores.posF-1,sensores.posC);
			
		break;
	
	case noreste:

		return make_pair(sensores.posF-1,sensores.posC+1);
		
		break;
	
	case este:

		return make_pair(sensores.posF,sensores.posC+1);
		
		break;
	
	case sureste:

		return make_pair(sensores.posF+1,sensores.posC+1);
		
		break;
	
	case sur:

		return make_pair(sensores.posF+1,sensores.posC);
		
		break;
	
	case suroeste:
	
		return make_pair(sensores.posF+1,sensores.posC-1);
		
		break;
	
	case oeste:

		return make_pair(sensores.posF,sensores.posC-1);
		
		break;
	
	case noroeste:

		return make_pair(sensores.posF-1,sensores.posC-1);
		
		break;
	
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
	instante++;
	matrizTempR[sensores.posF][sensores.posC] = instante;

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaR(mapaResultado,mapaCotas,sensores);

	

	


	if(giro45Izq != 0){
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
	

		pair<int,int> posicion = Funcion_delante(mapaResultado,mapaCotas,sensores);
		
		int tc = matrizTempR[posicion.first][posicion.second]; // centro
		Sensores copia_sensores = sensores;
		copia_sensores.rumbo = static_cast<Orientacion>( (copia_sensores.rumbo+7)%8);
		posicion = Funcion_delante(mapaResultado,mapaCotas,copia_sensores);
		int ti = matrizTempR[posicion.first][posicion.second]; // izquierda
		copia_sensores.rumbo = static_cast<Orientacion>( (sensores.rumbo+1)%8);
		posicion = Funcion_delante(mapaResultado,mapaCotas,copia_sensores);
		int td = matrizTempR[posicion.first][posicion.second]; // derecha
		
		int pos = VeoCasillaInteresanteR1(i, c, d, tiene_zapatillas,ti,tc,td);

		
		switch(pos)
		{
			case 2:
				accion = WALK;
				break;
			case 1:
			cout<< "GIRO CASO 1"<< endl;
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
	/*
	Action accion = IDLE;
	if (!hayPlan){
		
	}
	if (hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size()== 0){
		hayPlan = false;
	}
	return accion;
		
     */   
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}
  