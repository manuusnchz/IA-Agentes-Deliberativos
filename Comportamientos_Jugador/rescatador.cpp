#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0 (sensores);
		break;
	case 1:
		// accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoRescatadorNivel_2 (sensores);
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
	if( c == 'C') return 2;
	else if( i == 'C') return 1;
	else if(d== 'C') return 3;
	else return 0;

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

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	Action accion;

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	SituarSensorEnMapaR(mapaResultado,mapaCotas,sensores);

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
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}