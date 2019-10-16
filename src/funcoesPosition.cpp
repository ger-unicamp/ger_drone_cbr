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

>
double tamanho(ger_drone_cbr::Position a)
{
	return sqrt((a.x^2)+(a.y^2)+(a.z^2))
}

double distancia(ger_drone_cbr::Position a, ger_drone_cbr::Position b)
{
	return sqrt(((a.x - b.x) ^ 2) + ((a.y - b.y) ^ 2) + ((a.z - b.z) ^ 2);
}