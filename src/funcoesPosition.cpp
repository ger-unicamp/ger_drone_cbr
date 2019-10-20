#include <math.h>

#include "ger_drone_cbr/Position.h"

#include "funcoesPosition.h"

bool comparaPosicao(ger_drone_cbr::Position a, ger_drone_cbr::Position b)
{
	if (distancia(a,b)>0.25)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool comparaPosicao(ger_drone_cbr::Position a, ger_drone_cbr::Position b, float proximidade)
{
	if (distancia(a,b)>proximidade)
	{
		return false;
	}
	else
	{
		return true;
	}
}

double tamanho(ger_drone_cbr::Position a)
{
	return sqrt(pow(a.x,2)+pow(a.y,2)+pow(a.z,2));
}

double distancia(ger_drone_cbr::Position a, ger_drone_cbr::Position b)
{
	return sqrt(  pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
}

ger_drone_cbr::Position somaPosicao(ger_drone_cbr::Position a, ger_drone_cbr::Position b)
{
	ger_drone_cbr::Position resultado;

	resultado.x = a.x+b.x;
	resultado.y = a.y+b.y;
	resultado.z = a.z+b.z;
	resultado.yaw = a.yaw+b.yaw;	

	return resultado;
}