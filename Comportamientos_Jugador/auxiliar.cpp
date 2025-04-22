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
		// accion = ComportamientoAuxiliarNivel_1 (sensores);
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

int VeoCasillaInteresanteA(char i, char c, char d){
	if(c == 'X') return 2;
	else if(i == 'X') return 1;
	else if(d == 'X') return 3;
	
	if(c == 'D') return 2;
	else if(i == 'D') return 1;
	else if(d == 'D') return 3;
	
	if( c == 'C') return 2;
	else if( d == 'C') return 3;
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

list<Action> AvanzaASaltosDeCaballo(){
	list<Action> secuencia;
	secuencia.push_back(WALK);
	secuencia.push_back(WALK);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(WALK);
	return secuencia;
}



Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	Action accion;

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

		int pos = VeoCasillaInteresanteA(i,c,d);
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
	plan = AvanzaASaltosDeCaballo();
	hayPlan = true;
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